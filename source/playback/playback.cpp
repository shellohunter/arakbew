/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

#ifdef WINDOWS
#include "win32/playback.h"
#else
#include "playback.h"
#include "shared.h"
#endif /* WINDOWS */

using namespace std;

#if WEBKOK_PB_MGR_DBG
pthread_mutex_t g_dbg_mutex = PTHREAD_MUTEX_INITIALIZER;
void vWebKokPbDbg(const char * format, ...)
{
    char arucString[512] = "[WEB_KOK_PB] ";
    va_list argptr;

    pthread_mutex_lock(&g_dbg_mutex);

    va_start(argptr, format);
    vsnprintf((char *)(arucString + 13), (sizeof(arucString) -13), format, argptr);
    va_end(argptr);
    printf((const char *)arucString);
    //printf("\n");

    pthread_mutex_unlock(&g_dbg_mutex);
}
#endif

IMTK_PB_CB_ERROR_CODE_T webkok_pb_nfy_fct(IMTK_PB_CTRL_EVENT_T eEventType, void* pvAppCbTag, uint32_t u4Data)
{
    if (NULL == pvAppCbTag)
    {
        return IMTK_PB_CB_ERROR_CODE_NOT_OK;
    }

    (*(Playback *)pvAppCbTag).mrPbCtrlEventCb(eEventType, u4Data);

    return IMTK_PB_CB_ERROR_CODE_OK;
}

Playback::Playback(const char * pThreadName, PFN_PB_NOTIFY_CB pfnCb) : PThread(pThreadName, FALSE)
{
    WEBKOK_PB_INFO(("Create playback engine\n"));

    vInit();

    IMTK_PB_HANDLE_T h_handle = IMTK_NULL_HANDLE;
    IMTK_PB_ERROR_CODE_T tRet = IMTK_PB_ERROR_CODE_OK;

    IMtkPb_Ctrl_Init();

    IMtkPb_Misc_Init(); // for adjust valume

    tRet = IMtkPb_Ctrl_Open(&h_handle, IMTK_PB_CTRL_BUFFERING_MODEL_URI, IMTK_PB_CTRL_LIB_MASTER, NULL);
    if (tRet != IMTK_PB_ERROR_CODE_OK)
    {
        WEBKOK_PB_ERROR(("Open engine fail\n"));
        m_hPlaybackHandle = IMTK_NULL_HANDLE;
        return;
    }

    m_hPlaybackHandle = h_handle;
    WEBKOK_PB_INFO(("Engine handle: %d \n", (UINT32)h_handle));

    WEBKOK_PB_INFO(("RegCallback\n"));
    m_pfnNotifyCb = pfnCb;

    tRet = IMtkPb_Ctrl_RegCallback(m_hPlaybackHandle, (VOID *)this, webkok_pb_nfy_fct);
    if (tRet != IMTK_PB_ERROR_CODE_OK)
    {
        WEBKOK_PB_ERROR(("RegCallbback fail\n"));
        return;
    }

    /*create thread for Playback.*/
    vCreateTask();
}

Playback::~Playback()
{
    WEBKOK_PB_INFO(("~Playback\n"));
    if (m_pcFilePath != NULL)
    {
        free(m_pcFilePath);
        m_pcFilePath = NULL;
    }
    vInit();
/*
    IMtkPb_Ctrl_Close(m_hPlaybackHandle);
    IMtkPb_Misc_Terminate();
    IMtkPb_Ctrl_Terminate();
    m_hPlaybackHandle = IMTK_NULL_HANDLE;
*/
}

VOID Playback::vPfnNotifyCbProxy(E_NOTIFY_TYPE eNfyTp, void *pvData)
{
    if (m_pfnNotifyCb == NULL)
    {
        return;
    }

    PB_NOTIFY_T tNotifyInf;
    memset((void *)&tNotifyInf, 0, sizeof(PB_NOTIFY_T));

    switch(eNfyTp)
    {
        case E_UOP_RET:
        {
            tNotifyInf.eNfyTp = E_UOP_RET;
            tNotifyInf.u.ePbcStt = *((E_PBC_STT *)pvData);
            break;
        }
        case E_PBC_NOTIFY:
        {
            tNotifyInf.eNfyTp = E_PBC_NOTIFY;
            tNotifyInf.u.tUopInf.eUop = ((PB_UOP_RET_T *)pvData)->eUop;
            tNotifyInf.u.tUopInf.eRet = ((PB_UOP_RET_T *)pvData)->eRet;
            break;
        }
        case E_TIME_UPDATE:
        {
            tNotifyInf.eNfyTp = E_TIME_UPDATE;
            tNotifyInf.u.tTmInf.u4CurTime = ((PB_TIME_INF_T *)pvData)->u4CurTime;
            tNotifyInf.u.tTmInf.u4TtTime = ((PB_TIME_INF_T *)pvData)->u4TtTime;
            break;
        }
    default:
        break;
    }

    m_pfnNotifyCb(&tNotifyInf);

    return;
}

MRESULT Playback::mrPbCtrlEventCb(IMTK_PB_CTRL_EVENT_T eEventType, uint32_t u4Data)
{
    switch (eEventType)
    {
        case IMTK_PB_CTRL_EVENT_GET_BUF_READY:
            WEBKOK_PB_INFO((" evt get buffer ready callback!\n"));
            break;
        case IMTK_PB_CTRL_EVENT_CUR_TIME_UPDATE:
            u4Data /=1000;
            if (u4Data != m_tTmInf.u4CurTime)
            {
                WEBKOK_PB_INFO(("evt Cur_Time:<%d>\n", u4Data));
                m_tTmInf.u4CurTime = u4Data;
                vPfnNotifyCbProxy(E_TIME_UPDATE, &m_tTmInf);
            }
            if (m_ePBCStt != E_PBC_PLAY)
            {
                m_ePBCStt = E_PBC_PLAY;
                vPfnNotifyCbProxy(E_PBC_NOTIFY, &m_ePBCStt);
            }
            break;
        case IMTK_PB_CTRL_EVENT_TOTAL_TIME_UPDATE:
            u4Data /=1000;
            if (u4Data != m_tTmInf.u4TtTime)
            {
                WEBKOK_PB_INFO(("evt Tt_Time: <%d>\n", u4Data));
                m_tTmInf.u4TtTime = u4Data;
                vPfnNotifyCbProxy(E_TIME_UPDATE, &m_tTmInf);
            }
            break;
        case IMTK_PB_CTRL_EVENT_EOF:
            WEBKOK_PB_INFO((" evt EOF callback!\n"));
            break;
        case IMTK_PB_CTRL_EVENT_EOS:
            WEBKOK_PB_INFO((" evt EOS callback!\n"));
            m_fgIsEosCb = TRUE;
            m_ePBCStt = E_PBC_STOP;
            vPfnNotifyCbProxy(E_PBC_NOTIFY, &m_ePBCStt);
            break;
        case IMTK_PB_CTRL_EVENT_BOS:
            WEBKOK_PB_INFO((" evt BOS callback!\n"));
            break;
        case IMTK_PB_CTRL_EVENT_PLAY_DONE:
            WEBKOK_PB_INFO((" evt PLAY_DONE callback!\n"));
            //m_ePBCStt = E_PBC_PLAY;
            break;
        case IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR:
            m_ePBCStt = E_PBC_PB_ERR;
            vPfnNotifyCbProxy(E_PBC_NOTIFY, &m_ePBCStt);
            WEBKOK_PB_INFO((" evt PLAYBACK_ERROR: %d \n", u4Data));
            break;
        default:
            //WEBKOK_PB_INFO((" evt default: %d \n", eEventType));
            break;
    }

    return MRESULT_OK;
}

VOID Playback::vTerminate()
{
    WEBKOK_PB_INFO(("Playback Terminate\n"));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    IMTK_PB_ERROR_CODE_T tRet;
    UINT32 u4Cnt;

    /*destroy thread for Playback.*/
    vDestroyTask();

    u4Cnt = 0;
    while(1)
    {
        u4Cnt++;
        tRet = IMtkPb_Ctrl_Close(m_hPlaybackHandle);
        if ((tRet == IMTK_PB_ERROR_CODE_OK) || (u4Cnt > 10))
        {
            WEBKOK_PB_ERROR(("wait close, tRet[%d], u4Cnt[%d] \n", tRet, u4Cnt));
            break;
        }
    }
    u4Cnt = 0;
    while(1)
    {
        u4Cnt++;
        tRet = IMtkPb_Misc_Terminate();
        if ((tRet == IMTK_PB_ERROR_CODE_OK) || (u4Cnt > 10))
        {
            WEBKOK_PB_ERROR(("wait misc terminate, tRet[%d], u4Cnt[%d] \n", tRet, u4Cnt));
            break;
        }
    }
    u4Cnt = 0;
    while(1)
    {
        u4Cnt++;
        tRet = IMtkPb_Ctrl_Terminate();
        if ((tRet == IMTK_PB_ERROR_CODE_OK) || (u4Cnt > 10))
        {
            WEBKOK_PB_ERROR(("wait ctrl terminate, tRet[%d], u4Cnt[%d] \n", tRet, u4Cnt));
            break;
        }
    }
}

VOID Playback::vInit()
{
    m_hPlaybackHandle = IMTK_NULL_HANDLE;
    m_pfnNotifyCb = NULL;
    memset((VOID *)&m_tUopQueue, 0, sizeof(PB_UOP_QUEUE_T));
    m_ePBCStt = E_PBC_IDLE;
    m_pcFilePath = NULL;
    m_u1Volume = PB_AUD_VOL_MAX;
    m_tTmInf.u4CurTime = 0;
    m_tTmInf.u4TtTime = 0;
    m_fgIsEosCb = FALSE;
    m_fgTerminate = FALSE;
}

VOID Playback::vCreateTask()
{
    m_fgTerminate = FALSE;
    start();
}

VOID Playback::vDestroyTask()
{
    m_fgTerminate = TRUE;
    stop();
}

VOID Playback::run()
{
    while(1)
    {
        if (m_fgTerminate)
        {
            WEBKOK_PB_INFO(("Eos exit thread!!!\n"));
            break;
        }
        //sleep(1);
    }
}

MRESULT Playback::mrPlay()
{
    WEBKOK_PB_INFO(("cmd-play\n"));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    IMTK_PB_ERROR_CODE_T tRet;

    if ((m_ePBCStt != E_PBC_STOP) && (m_ePBCStt != E_PBC_IDLE))
    {
        WEBKOK_PB_ERROR(("Pbc stt invalid!!! \n"));
        return MRESULT_NOT_IMPL;
    }

    tRet = IMtkPb_Ctrl_Play(m_hPlaybackHandle, 0);
    if (tRet != IMTK_PB_ERROR_CODE_OK)
    {
        WEBKOK_PB_ERROR(("play fail!!! \n"));
        return MRESULT_NOT_IMPL;
    }

    m_ePBCStt = E_PBC_BUFFERING;
    vPfnNotifyCbProxy(E_PBC_NOTIFY, &m_ePBCStt);
    WEBKOK_PB_INFO(("cmd-play --> end \n"));
    return MRESULT_OK;
}

MRESULT Playback::mrPlay(/*string & filePath*/ const char *pcFilePath)
{
    WEBKOK_PB_INFO(("cmd-mrPlay by path: %s\n", (UCHAR *)pcFilePath));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    if ((m_ePBCStt != E_PBC_STOP) && (m_ePBCStt != E_PBC_IDLE))
    {
        if (mrStop(TRUE) != MRESULT_OK)
        {
            WEBKOK_PB_ERROR(("stop error!!! \n"));
            return MRESULT_NOT_IMPL;
        }
    }

    IMTK_PB_ERROR_CODE_T tRet;
    IMTK_PB_CTRL_ENGINE_PARAM_T tParam = {0};

    tParam.uBufferModelParam.tUriInfo.u4URI_len = strlen((char *)pcFilePath);
    tParam.uBufferModelParam.tUriInfo.pu1URI = (uint8_t *)pcFilePath;
    tParam.uBufferModelParam.tUriInfo.eBufSizeType = IMTK_PB_CTRL_BUF_SIZE_TYPE_BYTE;
    tParam.uBufferModelParam.tUriInfo.uBufSize.u4Bytes = ((uint32_t)(20*1024*1024));
    tParam.uBufferModelParam.tUriInfo.u4KeepBufThreshold = ((uint32_t)((20*1024*1024)*5/100));
    tParam.uBufferModelParam.tUriInfo.u4ReBufThreshold = ((uint32_t)((20*1024*1024)*60/100));

    WEBKOK_PB_INFO(("IMtkPb_Ctrl_SetEngineParam \n"));
    tRet = IMtkPb_Ctrl_SetEngineParam(m_hPlaybackHandle, &tParam);
    if (tRet != IMTK_PB_ERROR_CODE_OK)
    {
        WEBKOK_PB_ERROR(("invalid Param \n"));
        return MRESULT_INV_ARG;
    }

    WEBKOK_PB_INFO(("IMtkPb_Ctrl_Play \n"));
    tRet = IMtkPb_Ctrl_Play(m_hPlaybackHandle, 0);
    if (tRet != IMTK_PB_ERROR_CODE_OK)
    {
        return MRESULT_NOT_IMPL;
    }

    if (m_pcFilePath != NULL)
    {
        free(m_pcFilePath);
        m_pcFilePath = NULL;
    }
    m_pcFilePath = strdup(pcFilePath);

    m_ePBCStt = E_PBC_BUFFERING;
    vPfnNotifyCbProxy(E_PBC_NOTIFY, &m_ePBCStt);
    WEBKOK_PB_INFO(("cmd-play by path --> end \n"));
    return MRESULT_OK;
}


MRESULT Playback::mrPause()
{
    WEBKOK_PB_INFO(("cmd-pause\n"));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    IMTK_PB_ERROR_CODE_T tRet = IMTK_PB_ERROR_CODE_OK;
    IMTK_PB_CTRL_STATE_T tState = IMTK_PB_CTRL_UNKNOWN;
    UINT32 u4Cnt = 0;

    tRet = IMtkPb_Ctrl_Pause(m_hPlaybackHandle);
    if (tRet != IMTK_PB_ERROR_CODE_OK)
    {
        WEBKOK_PB_ERROR(("pause fail !!! \n"));
        return MRESULT_NOT_IMPL;
    }

    while(1)
    {
        u4Cnt++;
        IMtkPb_Ctrl_GetState(m_hPlaybackHandle, &tState);
        if ((tState == IMTK_PB_CTRL_PAUSED) || (u4Cnt > 100))
        {
            WEBKOK_PB_ERROR(("wait pause, tState[%d], u4Cnt[%d] \n", (UINT32)tState, u4Cnt));
            break;
        }
    }

    m_ePBCStt = E_PBC_PAUSE;
    vPfnNotifyCbProxy(E_PBC_NOTIFY, &m_ePBCStt);
    return MRESULT_OK;
}

MRESULT Playback::mrResume()
{
    WEBKOK_PB_INFO(("cmd-resume\n"));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    IMTK_PB_ERROR_CODE_T tRet;

    tRet = IMtkPb_Ctrl_Play(m_hPlaybackHandle, 0);
    if (tRet != IMTK_PB_ERROR_CODE_OK)
    {
        WEBKOK_PB_ERROR(("resume fail !!! \n"));
        return MRESULT_NOT_IMPL;
    }

    m_ePBCStt = E_PBC_BUFFERING;
    vPfnNotifyCbProxy(E_PBC_NOTIFY, &m_ePBCStt);
    return MRESULT_OK;
}

MRESULT Playback::mrStop(BOOL fgSync)
{
    WEBKOK_PB_INFO(("cmd-stop\n"));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    IMTK_PB_ERROR_CODE_T tRet = IMTK_PB_ERROR_CODE_OK;
    IMTK_PB_CTRL_STATE_T tState = IMTK_PB_CTRL_UNKNOWN;
    UINT32 u4Cnt = 0;

    tRet = IMtkPb_Ctrl_Stop(m_hPlaybackHandle);
    if (tRet != IMTK_PB_ERROR_CODE_OK)
    {
        WEBKOK_PB_ERROR(("stop fail !!! \n"));
        return MRESULT_NOT_IMPL;
    }
    WEBKOK_PB_INFO(("cmd-stop --> done-1\n"));
    while(1)
    {
        u4Cnt++;
        IMtkPb_Ctrl_GetState(m_hPlaybackHandle, &tState);
        if ((tState == IMTK_PB_CTRL_READY) || (u4Cnt > 100))
        {
            WEBKOK_PB_ERROR(("wait stop, tState[%d], u4Cnt[%d] \n", (UINT32)tState, u4Cnt));
            break;
        }
    }

    m_ePBCStt = E_PBC_STOP;
    vPfnNotifyCbProxy(E_PBC_NOTIFY, &m_ePBCStt);
    WEBKOK_PB_INFO(("cmd-stop --> done-2\n"));
    return MRESULT_OK;
}

MRESULT Playback::mrMute(BOOL fgMute, IMTK_PB_MISC_AUD_CHANNEL eChannel)
{
    IMTK_PB_ERROR_CODE_T tRet;

    if (fgMute)
    {
        WEBKOK_PB_INFO(("cmd-mute: ON \n"));
        tRet = IMtkPb_Misc_GetAudioVolume(eChannel, &m_u1Volume);
        if (tRet != IMTK_PB_ERROR_CODE_OK)
        {
            WEBKOK_PB_ERROR(("get volume fail\n"));
            m_u1Volume = PB_AUD_VOL_MAX;
            //return MRESULT_NOT_IMPL;
        }
        tRet = IMtkPb_Misc_SetAudioVolume(eChannel, PB_AUD_VOL_MIN);
        if (tRet != IMTK_PB_ERROR_CODE_OK)
        {
            WEBKOK_PB_ERROR(("volume mute fail\n"));
            return MRESULT_NOT_IMPL;
        }
    }
    else
    {
        WEBKOK_PB_INFO(("cmd-mute: OFF \n"));
        tRet = IMtkPb_Misc_SetAudioVolume(eChannel, m_u1Volume);
        if (tRet != IMTK_PB_ERROR_CODE_OK)
        {
            WEBKOK_PB_ERROR(("set volume fail\n"));
            return MRESULT_NOT_IMPL;
        }
    }

    return MRESULT_OK;
}

MRESULT Playback::mrVocal(BOOL fgEnable)
{
    WEBKOK_PB_INFO(("cmd-vocal\n"));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    return MRESULT_OK;
}

MRESULT Playback::mrVolume(BOOL fgUp, IMTK_PB_MISC_AUD_CHANNEL eChannel)
{
    WEBKOK_PB_INFO(("cmd-volume\n"));

    IMTK_PB_ERROR_CODE_T tRet;

    tRet = IMtkPb_Misc_GetAudioVolume(eChannel, &m_u1Volume);
    if (tRet != IMTK_PB_ERROR_CODE_OK)
    {
        WEBKOK_PB_ERROR(("get volume fail\n"));
        return MRESULT_NOT_IMPL;
    }

    if (fgUp)
    {
        if (m_u1Volume < PB_AUD_VOL_MAX)
            ++m_u1Volume;
    }
    else
    {
        if (m_u1Volume > PB_AUD_VOL_MIN)
            --m_u1Volume;
    }

    WEBKOK_PB_INFO(("set volume: %d \n", m_u1Volume));

    tRet = IMtkPb_Misc_SetAudioVolume(eChannel, m_u1Volume);
    if (tRet != IMTK_PB_ERROR_CODE_OK)
    {
        WEBKOK_PB_ERROR(("set volume fail\n"));
        return MRESULT_NOT_IMPL;
    }

    return MRESULT_OK;
}

MRESULT Playback::mrSetVideoWindow(UINT32 u4X, UINT32 u4Y, UINT32 u4W, UINT32 u4H)
{
    WEBKOK_PB_INFO(("cmd-zoom: X[%d], Y[%d], W[%d], H[%d]\n", u4X, u4Y, u4W, u4H));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    IMTK_PB_ERROR_CODE_T tRet;
    IMTK_PB_CTRL_RECT_T tSrcRect;
    IMTK_PB_CTRL_RECT_T tDstRect;

    tSrcRect.u4W = 10000;
    tSrcRect.u4H = 10000;
    tSrcRect.u4X = 0;
    tSrcRect.u4Y = 0;

    tDstRect.u4W = u4W;
    tDstRect.u4H = u4H;
    tDstRect.u4X = u4X;
    tDstRect.u4Y = u4Y;

    tRet = IMtkPb_Ctrl_SetDisplayRectangle(m_hPlaybackHandle, &tSrcRect, &tDstRect);

    return (tRet != IMTK_PB_ERROR_CODE_OK) ? MRESULT_NOT_IMPL : MRESULT_OK;
}


char * const Playback::mrGetCurrentPath()
{
    WEBKOK_PB_INFO(("Get current path"));

    return m_pcFilePath;
}


E_PBC_STT Playback::eGetPBCStt()
{
    WEBKOK_PB_INFO(("Get current PBC state: %d\n", (UINT32)m_ePBCStt));

    return m_ePBCStt;
}

MRESULT Playback::mrGetPbTimeInfo(PB_TIME_INF_T &tTmInf)
{
    tTmInf.u4CurTime = m_tTmInf.u4CurTime;
    tTmInf.u4TtTime = m_tTmInf.u4TtTime;

    WEBKOK_PB_INFO(("Get Time Info, Cur_Tm[%d], Tt_Tm[%d]\n", m_tTmInf.u4CurTime, m_tTmInf.u4TtTime));
    return MRESULT_OK;
}

MRESULT Playback::mrGetMediaInfo(IMTK_PB_CTRL_GET_MEDIA_INFO_T *ptMediaInfo)
{
    WEBKOK_PB_INFO(("Get Media Info\n"));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    IMTK_PB_ERROR_CODE_T tRet;

    tRet = IMtkPb_Ctrl_GetMediaInfo(m_hPlaybackHandle, ptMediaInfo);

    return (tRet != IMTK_PB_ERROR_CODE_OK) ? MRESULT_NOT_IMPL : MRESULT_OK;
}

MRESULT Playback::mrGetAudioTrack(IMTK_PB_CTRL_GET_CUR_AUD_TRACK_INFO_T *ptAudTrkInf)
{
    WEBKOK_PB_INFO(("Get Current Aud trk info\n"));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    IMTK_PB_ERROR_CODE_T tRet;

    tRet = IMtkPb_Ctrl_GetCurAudTrackInfo(m_hPlaybackHandle, ptAudTrkInf);

    return (tRet != IMTK_PB_ERROR_CODE_OK) ? MRESULT_NOT_IMPL : MRESULT_OK;
}

MRESULT Playback::mrSetAudioTrack(UINT16 u2Trk)
{
    WEBKOK_PB_INFO(("Set Aud Trk: %d \n", u2Trk));
    WEBKOK_PB_ASSERT(IMTK_NULL_HANDLE != m_hPlaybackHandle);

    IMTK_PB_ERROR_CODE_T tRet;

    tRet = IMtkPb_Ctrl_SetAudTrack(m_hPlaybackHandle, u2Trk);

    return (tRet != IMTK_PB_ERROR_CODE_OK) ? MRESULT_NOT_IMPL : MRESULT_OK;
}

MRESULT Playback::mrSetDebugLvl()
{
    return MRESULT_OK;
}

MRESULT Playback::mrGetDebugLvl()
{
    return MRESULT_OK;
}

