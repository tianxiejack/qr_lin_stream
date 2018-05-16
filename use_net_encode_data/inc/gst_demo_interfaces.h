#ifndef _BITSINTERFACESAPI_H_
#define _BITSINTERFACESAPI_H_

typedef enum
{
    VCODEC_FRAME_TYPE_I_FRAME,
    /**< I-frame or Key Frame */

    VCODEC_FRAME_TYPE_P_FRAME,
    /**< P-frame */

    VCODEC_FRAME_TYPE_B_FRAME,
    /**< B-frame */

} VCODEC_FRAME_TYPE_E;

typedef enum
{
    VCODEC_TYPE_H264,
    /**< H264 */
    VCODEC_TYPE_MPEG4,
    /**< MPEG4 */
    VCODEC_TYPE_MJPEG,
    /**< MJPEG */
    VCODEC_TYPE_MPEG2,
    /**< MPEG2 */
    VCODEC_TYPE_MAX,
    /**< Max Codec Type */
} VCODEC_TYPE_E;
 
typedef struct
{
	UINT flag;	// 0xFFFF0001
	UINT size;
	UINT chnId;	// no use in current version
	UINT strmId;
	VCODEC_TYPE_E codecType;
	VCODEC_FRAME_TYPE_E frameType;
	UINT timestamp;
	UINT reserved;
} DATAHEADER;

void demoInterfacesDestroy(void);
int demoInterfacesPut(DATAHEADER *pPrm, BYTE* pData);
void demoInterfacesCreate(char *ip_addr, Uint32 port);
#endif
