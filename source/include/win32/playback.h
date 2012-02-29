#ifndef PLAYBACK_HPP
#define PLAYBACK_HPP

#ifdef WINDOWS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "pthread.h"
#include "ptasking.h"


/* const hack */
#define IMTK_NULL_HANDLE                    0
#define IMTK_PB_CTRL_BUFFERING_MODEL_URI    0
#define IMTK_PB_CTRL_LIB_MASTER             0
#define IMTK_PB_ERROR_CODE_OK               0
#define IMTK_PB_CB_ERROR_CODE_NOT_OK        0
#define IMTK_PB_CB_ERROR_CODE_OK            0
#define IMTK_PB_CTRL_UNKNOWN                0
#define IMTK_PB_CTRL_EVENT_GET_BUF_READY    0
#define IMTK_PB_CTRL_EVENT_CUR_TIME_UPDATE  1
#define IMTK_PB_CTRL_EVENT_TOTAL_TIME_UPDATE 2
#define IMTK_PB_CTRL_EVENT_EOF              3
#define IMTK_PB_CTRL_EVENT_EOS              4
#define IMTK_PB_CTRL_EVENT_BOS              5
#define IMTK_PB_CTRL_EVENT_PLAY_DONE        6
#define IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR   7
#define IMTK_PB_CTRL_BUF_SIZE_TYPE_BYTE     0
#define IMTK_PB_CTRL_READY                  0
#define IMTK_PB_CTRL_PAUSED                 0
#define IMTK_PB_MISC_AUD_CHANNEL_ALL        0


/* data structure hack */
#define IMTK_PB_ERROR_CODE_T                int
#define IMTK_PB_CB_ERROR_CODE_T             int
#define IMTK_PB_HANDLE_T                    int
#define IMTK_PB_CTRL_EVENT_T                int
#define IMTK_PB_CTRL_STATE_T                int
#define IMTK_PB_MISC_AUD_CHANNEL            int

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef VOID
#define VOID void
#endif

typedef int BOOL;
typedef unsigned char UCHAR;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;


typedef struct
{
    int eMediaType;
    uint32_t u4TotalDuration; 
    uint32_t u8Size;
    uint32_t u4AvgBitrate;
    uint32_t u2AudioTrackNum;
}IMTK_PB_CTRL_GET_MEDIA_INFO_T;


typedef struct
{
    int eAudEnc;
    int eChNum;
    long int eSampleRate;
    long int u4InstBitRate;
}IMTK_PB_CTRL_GET_CUR_AUD_TRACK_INFO_T;


typedef struct
{
    unsigned u4X;
    unsigned u4Y;
    unsigned u4W;
    unsigned u4H;
}IMTK_PB_CTRL_RECT_T;

typedef struct
{
    struct{
        struct
        {
            uint32_t    u4URI_len;
            uint8_t*    pu1URI;
            int         eBufSizeType;
            struct {
            uint32_t    u4Bytes;
            }uBufSize;
            uint32_t    u4Bytes;
            uint32_t    u4KeepBufThreshold;
            uint32_t    u4ReBufThreshold;
        }tUriInfo;
    }uBufferModelParam;

} IMTK_PB_CTRL_ENGINE_PARAM_T;


/* function hack */

#define IMtkPb_Ctrl_Init()
#define IMtkPb_Misc_Init()
#define IMtkPb_Ctrl_Open(a,b,c,d)           0
#define IMtkPb_Ctrl_Close(a)                0
#define IMtkPb_Ctrl_RegCallback(a,b,c)      0
#define IMtkPb_Misc_Terminate()             0
#define IMtkPb_Ctrl_Terminate()             0
#define IMtkPb_Ctrl_Play(a,b)               0
#define IMtkPb_Ctrl_SetEngineParam(a,b)     0
#define IMtkPb_Ctrl_Pause(a)                0
#define IMtkPb_Ctrl_Stop(a)                 0
#define IMtkPb_Ctrl_GetState(a, b)              
#define IMtkPb_Ctrl_GetMediaInfo(a, b)      0
#define IMtkPb_Misc_GetAudioVolume(a,b)     0
#define IMtkPb_Misc_SetAudioVolume(a, b)    0
#define IMtkPb_Ctrl_SetDisplayRectangle(a, b,c)    0
#define IMtkPb_Ctrl_GetCurAudTrackInfo(a, b)    0
#define IMtkPb_Ctrl_SetAudTrack(a, b)    0














#define WEBKOK_PB_MGR_DBG 0
#if WEBKOK_PB_MGR_DBG
extern pthread_mutex_t g_t_dbg_mutex;
extern void vWebKokPbDbgInit();
extern void vWebKokPbDbg(const char * format, ...);
#define WEBKOK_PB_INFO(argu) { vWebKokPbDbg argu; }
#define WEBKOK_PB_ERROR(argu) { vWebKokPbDbg argu; }
#define WEBKOK_PB_ASSERT(cond)                                                    \
        if (!(cond)) {                                                  \
            printf("\n[WEB_KOK_PB] DBG_ASSERT: FILE=%s, FUNCTION=%s, LINE=%d\n",\
                    __FILE__,                                           \
                    __FUNCTION__,                                       \
                    __LINE__);                                          \
            while(1);                                                  \
        }
#else
#define WEBKOK_PB_INFO(argu)
#define WEBKOK_PB_ERROR(argu)
#define WEBKOK_PB_ASSERT(cond)  
#endif

#define PB_AUD_VOL_MAX  100
#define PB_AUD_VOL_MIN   0

typedef enum
{
    /* General */
    MRESULT_OK                      =   0,   ///< Success.
    MRESULT_FAIL                  =   -1,  ///< Failed.
    MRESULT_INV_HANDLE      =   -2,  ///< The hHandle is invalid.
    MRESULT_INV_ARG                 =   -3,  ///< The parameters are invalid.
    MRESULT_REG_CB_ERROR     =   -4,  ///< Not enough memory resource.
    MRESULT_NOT_INIT                =   -5,  ///< The playback control is not initialized yet.
    MRESULT_NOT_IMPL                =   -6,  ///< Not implement or support this function.
    MRESULT_DRV_ERROR               =   -7,  ///< The target driver has problem.
    MRESULT_SYS_LIB_ERROR           =   -8,  ///< Error in system call or standard library

    /* IMtkPb_Ctrl specific (from -100) */
    MRESULT_GET_BUF_PENDING         =   -100, ///< The buffer is not ready to get.
    MRESULT_NO_BUFFERSINK           =   -101, ///< No buffer sink can be set for this playback
} MRESULT;

/* defined for UOP & UOP Queue */
typedef enum{
  PB_UOP_NONE = 0X00,
  PB_UOP_PLAY,
  PB_UOP_STOP,
  PB_UOP_PAUSE,
  PB_UOP_RESUME,
  PB_UOP_NEXT,
  PB_UOP_PREV,
  PB_UOP_CHANGE_ASTN,
  PB_UOP_SET_VOLUME,
  PB_UOP_ZOOM,  
  PB_UOP_MAX
} PB_UOP_ID_E;

#define PB_UOP_QUEUE_SZ  5
typedef struct _PB_UOP_INF_T
{
    PB_UOP_ID_E e_uop;
    VOID*            pv_uop_param;
    UINT32           u4_uop_param_sz;
    UINT32*         pu4_cmd_ret;
} PB_UOP_INF_T;

typedef struct _PB_UOP_QUEUE_T
{
    PB_UOP_INF_T pt_uop_info[PB_UOP_QUEUE_SZ];
    UINT32            u4_uop_rd_idx;
    UINT32            u4_uop_wr_idx;
} PB_UOP_QUEUE_T;

typedef struct _PB_UOP_RET_T
{
    PB_UOP_ID_E eUop;
    MRESULT        eRet;
} PB_UOP_RET_T;

typedef struct _PB_TIME_INF_T
{
    UINT32 u4CurTime;
    UINT32 u4TtTime;
} PB_TIME_INF_T;

typedef enum
{
    E_PBC_IDLE = 0,
    E_PBC_STOP,
    E_PBC_PLAY,
    E_PBC_PAUSE,
    E_PBC_BUFFERING,
    E_PBC_PB_ERR,
} E_PBC_STT;

typedef enum
{
    E_UOP_RET = 0,
    E_PBC_NOTIFY,
    E_TIME_UPDATE,
} E_NOTIFY_TYPE;

typedef struct _PB_NOTIFY_T
{
    E_NOTIFY_TYPE eNfyTp;
    union
    {
        E_PBC_STT ePbcStt;
        PB_UOP_RET_T tUopInf;
        PB_TIME_INF_T tTmInf;
    }u;
} PB_NOTIFY_T;

/* defined for callback function type */
typedef void (*PFN_PB_NOTIFY_CB)(PB_NOTIFY_T *tNotifyInf);

class Playback : public PThread
{
public:
    Playback(const char * pThreadName, PFN_PB_NOTIFY_CB pfnCb);
    virtual ~Playback();

    VOID vTerminate();
    MRESULT mrPlay();
    MRESULT mrPlay(const char *pcFilePath); // path == (localpath || httpurl)
    MRESULT mrPause();
    MRESULT mrResume();
    MRESULT mrStop(BOOL fgSync = FALSE);
    MRESULT mrMute(BOOL fgMute, IMTK_PB_MISC_AUD_CHANNEL eChannel = IMTK_PB_MISC_AUD_CHANNEL_ALL);
    MRESULT mrVocal(BOOL fgEnable);
    MRESULT mrVolume(BOOL fgUp, IMTK_PB_MISC_AUD_CHANNEL eChannel = IMTK_PB_MISC_AUD_CHANNEL_ALL);
    MRESULT mrSetVideoWindow(UINT32 u4X, UINT32 u4Y, UINT32 u4W, UINT32 u4H);
    //string currentPath();
    E_PBC_STT eGetPBCStt();
    MRESULT mrGetPbTimeInfo(PB_TIME_INF_T &tTmInf);
    MRESULT mrGetMediaInfo(IMTK_PB_CTRL_GET_MEDIA_INFO_T *ptMediaInfo);
    MRESULT mrGetAudioTrack(IMTK_PB_CTRL_GET_CUR_AUD_TRACK_INFO_T *ptAudTrkInf);
    MRESULT mrSetAudioTrack(UINT16 u2Trk);
 
    MRESULT mrSetDebugLvl();
    MRESULT mrGetDebugLvl();
    
    MRESULT mrPbCtrlEventCb(IMTK_PB_CTRL_EVENT_T eEventType, uint32_t u4Data);

private:
    VOID vInit();
    VOID vPfnNotifyCbProxy(E_NOTIFY_TYPE eNfyTp, void *pvData);
    VOID vCreateTask();
    VOID vDestroyTask();
    VOID run();
    
    //string m_sCurrentFile;
    IMTK_PB_HANDLE_T m_hPlaybackHandle;
    PFN_PB_NOTIFY_CB m_pfnNotifyCb;
    PB_UOP_QUEUE_T m_tUopQueue;
    E_PBC_STT  m_ePBCStt;
    char *m_pcFilePath;
    PB_TIME_INF_T m_tTmInf;
    UINT8 m_u1Volume;
    BOOL m_fgIsEosCb;
    BOOL m_fgTerminate;
};

#endif
#endif /* PLAYBACK_HPP */

