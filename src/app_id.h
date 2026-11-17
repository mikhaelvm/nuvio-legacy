#ifndef NV_APP_ID_H
#define NV_APP_ID_H
/* Package metadata and the runtime must be built with the same identity. */
#define NV_APP_ID_PRODUCTION "com.mikhael.nuvio"
#ifndef NV_APP_ID
#ifdef NV_DTS_DEBUG
#error "NV_DTS_DEBUG requires an isolated NV_APP_ID package identity"
#endif
#define NV_APP_ID NV_APP_ID_PRODUCTION
#define NV_APP_ID_DEFAULT 1
#endif
#define NV_LS_MEDIA_CLIENT "com.webos.media.client." NV_APP_ID
#endif
