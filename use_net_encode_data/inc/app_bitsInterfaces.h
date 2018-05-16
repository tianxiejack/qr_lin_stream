#ifndef _BITSINTERFACES_H_
#define _BITSINTERFACES_H_

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

typedef struct 
{
    UInt32              reserved;
    /**< Used internally, USER MUST NOT MODIFY THIS VALUE */

    UInt32            chnId;
    /**< Encoder/Decoder channel ID 0..(VENC_CHN_MAX-1)  */

    UInt32           strmId;
    /**< Encoder stream ID, not valid for decoder, 0..(VENC_STRM_MAX-1)  */

    VCODEC_TYPE_E       codecType;
    /**< Video compression format */

    VCODEC_FRAME_TYPE_E frameType;
    /**< Compressed frame type */

    UInt32              bufSize;
    /**< Size of buffer, in bytes */

    UInt32              filledBufSize;
    /**< Actual size of bistream in buffer, in bytes */

    UInt32              mvDataOffset;
    /**< Actual offset to mv data bistream in buffer, in bytes */

    UInt32              mvDataFilledSize;
    /**< Actual size of mv data bistream in buffer, in bytes */

    Void               *bufVirtAddr;
    /**< User Virtual Space Buffer Address, can be used with write() or fwrite() APIs */

    Void               *bufPhysAddr;
    /**< User System Physical Buffer Address, can be used with EDMA APIs */

    UInt32              timestamp;
    /**< Capture or Display Frame timestamp, in msec */

    UInt32              upperTimeStamp;
    /**< Original Capture time stamp:Upper 32 bit value*/

    UInt32              lowerTimeStamp;
    /**< Original Capture time stamp: Lower 32 bit value*/

    UInt32              encodeTimestamp;
    /**< Time stamp after encode is complete */

    UInt32              numTemporalLayerSetInCodec;
    /**< numTemporalLayer which has been configured in 
        * the create time of h264 encoder */

    UInt32              temporalId;
    /**< SVC TemporalId */

    UInt32              fieldId;
    /**< 0: Even field or Frame based, 1: Odd Field */

    UInt32              frameWidth;
    /**< Width of frame, in pixels */

    UInt32              frameHeight;
    /**< Height of frame, in lines */
          
} VCODEC_BITSBUF_S;


#define BITS_CHN_MAX	10

/**
    \brief Video Bitstream Buffer List

    List of Video Bitstream Buffer's allows user to efficient exchange
    multiple frames with VENC, VDEC sub-system with less overhead
*/
typedef struct {

    UInt32              numBufs;
    /**< Number of valid frame's in bitsBuf[]. MUST be <= BITS_CHN_MAX */

    VCODEC_BITSBUF_S    bitsBuf[BITS_CHN_MAX];
    /**< Bistream information list */

} VCODEC_BITSBUF_LIST_S;

void bitsInterfacesCreate(int port);
void bitsInterfacesDestroy(void);

int bitsInterfacesRelease(VCODEC_BITSBUF_LIST_S *pBitsBufList);
int bitsInterfacesGet(VCODEC_BITSBUF_LIST_S *pBitsBufList, UInt32 timeout);

#endif
