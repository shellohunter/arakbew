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
#define sleep(ms)
#else
#include "playback.h"
#endif /* WINDOWS */

Playback *g_playback_h = NULL;

void vPrintPbcStt(E_PBC_STT ePBCStt)
{
    switch(ePBCStt)
    {
        case E_PBC_IDLE:
            WEBKOK_PB_INFO(("E_PBC_IDLE\n"));
            break;
        case E_PBC_STOP:
            WEBKOK_PB_INFO(("E_PBC_STOP\n"));
            break;
        case E_PBC_PLAY:
            WEBKOK_PB_INFO(("E_PBC_PLAY\n"));
            break;
        case E_PBC_PAUSE:
            WEBKOK_PB_INFO(("E_PBC_PAUSE\n"));
            break;
        case E_PBC_BUFFERING:
            WEBKOK_PB_INFO(("E_PBC_BUFFERING\n"));
            break;
        case E_PBC_PB_ERR:
            WEBKOK_PB_INFO(("E_PBC_PB_ERR\n"));
            break;
        default:
            WEBKOK_PB_INFO(("E_PBC_OTHERS\n"));
            break;
    }
}

void vPfnPbNotifyCb(PB_NOTIFY_T *tNotifyInf)
{
    WEBKOK_PB_INFO(("<--------------vPfnPbNotifyCb-------------------->\n"));
    switch(tNotifyInf->eNfyTp)
    {
        case E_UOP_RET:
            WEBKOK_PB_INFO(("E_UOP_RET\n", tNotifyInf->u.tUopInf.eUop, tNotifyInf->u.tUopInf.eRet));
            break;
        case E_PBC_NOTIFY:
            WEBKOK_PB_INFO(("E_PBC_NOTIFY\n"));
            vPrintPbcStt(tNotifyInf->u.ePbcStt);
            break;
        case E_TIME_UPDATE:
            WEBKOK_PB_INFO(("E_TIME_UPDATE: %d/%d\n", tNotifyInf->u.tTmInf.u4CurTime, tNotifyInf->u.tTmInf.u4TtTime));
            break;
        default:
            break;
    }
}

void vPbWaitPbcStt(E_PBC_STT ePBCStt)
{
    E_PBC_STT eCurPbcStt;
    //UINT32 u4CurTm, u4TtTm;
    
    while(1)
    {
        sleep(1);
        eCurPbcStt = g_playback_h->eGetPBCStt();
        #if 1
        if (eCurPbcStt == ePBCStt) 
        {
            PB_TIME_INF_T tTmInf;
            g_playback_h->mrGetPbTimeInfo(tTmInf);        
            vPrintPbcStt(ePBCStt);
            break;
        }
        #else
        if ((eCurPbcStt == ePBCStt) && (ePBCStt == E_PBC_PLAY))
        {
            PB_TIME_INF_T tTmInf;
            g_playback_h->mrGetPbTimeInfo(tTmInf);
            if (tTmInf.u4CurTime >= 1)
            {
                vPrintPbcStt(ePBCStt);
                break;
            }
        }
        else if ((eCurPbcStt == ePBCStt) && (ePBCStt != E_PBC_PLAY))
        {
            vPrintPbcStt(ePBCStt);
            break;
        }
        #endif
        else if (eCurPbcStt == E_PBC_PB_ERR)
        {
            vPrintPbcStt(E_PBC_PB_ERR);
            break;
        }
    }
}

MRESULT mrPbPlaybackSongTst(UCHAR *pcArguPath)
{
    IMTK_PB_CTRL_GET_MEDIA_INFO_T tMediaInfo;
    IMTK_PB_CTRL_GET_CUR_AUD_TRACK_INFO_T tAudTrkInf;
    UINT16 u2Trk = 0, u2TrkNum = 0;
    UINT8 u1Cnt = 0, u1Cnt1 = 0;
    MRESULT mresult = MRESULT_OK;
    
    memset((VOID *)&tMediaInfo, 0, sizeof(IMTK_PB_CTRL_GET_MEDIA_INFO_T));
    memset((VOID *)&tAudTrkInf, 0, sizeof(IMTK_PB_CTRL_GET_CUR_AUD_TRACK_INFO_T));

    mresult = g_playback_h->mrPlay((char *)pcArguPath);
    if (mresult != MRESULT_OK)
    {
        return mresult;
    }
    vPbWaitPbcStt(E_PBC_PLAY);
    sleep(10);
    
    mresult = g_playback_h->mrPause();
    vPbWaitPbcStt(E_PBC_PAUSE);
    sleep(5);

    mresult = g_playback_h->mrResume();
    vPbWaitPbcStt(E_PBC_PLAY);
    sleep(5);

    mresult = g_playback_h->mrMute(TRUE);
    sleep(5);
    mresult = g_playback_h->mrMute(FALSE);
    sleep(5);

    mresult = g_playback_h->mrGetMediaInfo(&tMediaInfo);
    WEBKOK_PB_INFO(("MediaInfo: MediaType[%d], TotalDuration[%d], FileSize[%d], AvgBitrate[%d], AudTrkNum[%d]\n", 
                   tMediaInfo.eMediaType, tMediaInfo.u4TotalDuration, (UINT32)tMediaInfo.u8Size, (UINT32)tMediaInfo.u4AvgBitrate, (UINT16)tMediaInfo.u2AudioTrackNum));

    u2TrkNum = (UINT16)tMediaInfo.u2AudioTrackNum;

    if (u2TrkNum > 0)
    {
        while(1)
        {
            mresult = g_playback_h->mrSetAudioTrack(u2Trk);
            sleep(10);
            mresult = g_playback_h->mrGetAudioTrack(&tAudTrkInf);
            WEBKOK_PB_INFO(("AudTrkInf: Codec[%d], ChNum[%d], SampleRate[%ld], BitRate[%ld]\n", tAudTrkInf.eAudEnc, tAudTrkInf.eChNum, (long int)tAudTrkInf.eSampleRate, (long int)tAudTrkInf.u4InstBitRate));
            u2Trk++;
            WEBKOK_PB_INFO(("AudTrkInf: u2Trk[%x], u2TrkNum[%x]\n", u2Trk, u2TrkNum));
            if ((u2Trk > u2TrkNum) ||
                 (g_playback_h->eGetPBCStt() == E_PBC_STOP) ||
                 (g_playback_h->eGetPBCStt() == E_PBC_PB_ERR))
            {
                break;
            }
        }
    }
    else
    {
        mresult = g_playback_h->mrSetAudioTrack(0);
        sleep(10);
        mresult = g_playback_h->mrSetAudioTrack(1);
        sleep(10);
    }
    
    if ((g_playback_h->eGetPBCStt() == E_PBC_STOP) ||
         (g_playback_h->eGetPBCStt() == E_PBC_PB_ERR))
    {
        return MRESULT_FAIL;
    }
    
    for (u1Cnt = 0; u1Cnt < 20; u1Cnt++)
    {
        mresult = g_playback_h->mrVolume(FALSE);
        //sleep(1);
        for (u1Cnt1 = 50; u1Cnt1 > 0; u1Cnt1--);
    }
    /*
    sleep(8);
    for (u1Cnt = 0; u1Cnt < 50; u1Cnt++)
    {
        mresult = g_playback_h->mrVolume(TRUE);
        //sleep(1);
        for (u1Cnt1 = 50; u1Cnt1 > 0; u1Cnt1--);
    }
    */
    sleep(2);
    
    mresult = g_playback_h->mrSetVideoWindow(2140, 2140, 5710, 5710);
    sleep(8);
    mresult = g_playback_h->mrSetVideoWindow(0, 0, 10000, 10000);
    sleep(2);
    
    mresult = g_playback_h->mrStop(TRUE);
    if (mresult != MRESULT_OK)
    {
        return mresult;
    }
    vPbWaitPbcStt(E_PBC_STOP);
    
    sleep(5);
    mresult = g_playback_h->mrPlay();
    if (mresult != MRESULT_OK)
    {
        return mresult;
    }
    //vPbWaitPbcStt(E_PBC_PLAY);
    sleep(5);
    
    mresult = g_playback_h->mrStop(TRUE);
    if (mresult != MRESULT_OK)
    {
        return mresult;
    }
    vPbWaitPbcStt(E_PBC_STOP);
    sleep(5);

    return MRESULT_OK;
}

int main(int argc, char* argv[])
{
    char *pcArguCMD = NULL;
    char *pcArguPath1 = NULL;
    char *pcArguPath2 = NULL;
    MRESULT mresult;
    
    pcArguCMD = strdup(argv[1]);
    pcArguPath1 = strdup(argv[2]);
    pcArguPath2 = strdup(argv[3]);
    
    if (g_playback_h != NULL)
    {
        WEBKOK_PB_ERROR(("delete Playback class\n"));    
        delete g_playback_h;
    }
    
    if (strcmp(pcArguCMD, "play") == 0)
    {
        WEBKOK_PB_INFO(("new Playback class\n"));
        g_playback_h = new Playback("Playback_1", vPfnPbNotifyCb);
        
        //WEBKOK_PB_ERROR(("111111111111111111111111111111111111111111111111111\n")); 
        //mresult = mrPbPlaybackSongTst((UCHAR *)pcArguPath1);   
        
        WEBKOK_PB_ERROR(("222222222222222222222222222222222222222222222222222\n")); 
        //mresult = mrPbPlaybackSongTst((UCHAR *)pcArguPath2);   
        g_playback_h->mrPlay((char *)pcArguPath2);
        sleep(5);
        //g_playback_h->mrPlay((char *)pcArguPath2);
        g_playback_h->mrStop(TRUE);
        vPbWaitPbcStt(E_PBC_STOP);
        sleep(5);
    }

//==============================================

    WEBKOK_PB_INFO(("end process\n"));
    if (g_playback_h != NULL)
    {
        mresult = g_playback_h->mrStop(TRUE);
        if(mresult != MRESULT_OK)
        {
            WEBKOK_PB_ERROR((" stop fail \n"));
        }
        
        g_playback_h->vTerminate();

        delete g_playback_h;
        g_playback_h = NULL;
    }
  
    WEBKOK_PB_INFO((" Terminate return!\n"));

    if (pcArguCMD != NULL) free(pcArguCMD);
    if (pcArguPath1 != NULL) free(pcArguPath1);    
    if (pcArguPath2 != NULL) free(pcArguPath2);    

    //sleep(5);
    WEBKOK_PB_INFO((" Test Over!\n"));

    return 0;
}

