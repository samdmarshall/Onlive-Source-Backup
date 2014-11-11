#ifndef __GENERIC_BUFFER_INC__
#define __GENERIC_BUFFER_INC__

typedef void (*t_buffer_notifier_cb)(UINT BufHandle, INT request, UINT UserInfo);

#define MV_GENERIC_BUF_TYPE_XBV         1

typedef struct t_buffer_notifier {
    UINT    m_BufType;
    UINT    m_Flag;
    #define     BUFFER_NOTIFIER_FLAG_LWM   1
    #define     BUFFER_NOTIFIER_FLAG_HWM   2
    
    //! producer register here
    t_buffer_notifier_cb    m_LwmCallback;
    UINT    m_ProducerInfo;
    //! consumer request parameters
    INT     m_Lwm;
    UINT    m_ReqInfo;
    UINT    m_LastTs;

    //! consumer use here
    INT     m_Hwm;
    t_buffer_notifier_cb    m_HwmCallback;
    UINT    m_ConsumerInfo;
}Buffer_Notifier_t;

void MV_GBUF_DataRequest(UINT BufferHandle, UINT uRequest, UINT uReqInfo);

INT MV_GBUF_BufferFeedCheck(UINT BufferHandle);

INT MV_GBUF_RegisterHwmCB_Consumer(UINT BufferHandle, void* pCB, UINT uHwm);

INT MV_GBUF_RegisterLwmCB_Producer(UINT BufferHandle, void* pCB, UINT ProducerInfo);

#endif  //__GENERIC_BUFFER_INC

