/***************************************
File name: bitsInterfaces.c
Auther:	Aloysa
CopyRight@ JiaYue/LianShu/QianRun Graduate 2016

***************************************/
 /*
 modification history
 --------------------

 */

/* include files */
#include <app_global_def.h>
#include <app_bitsInterfaces.h>
#include "gst_ring_buffer.h"
#include <stdio.h>

/***************************************/
/*  extensional define  */
/***************************************/

/***************************************/
/* module  define  */
/***************************************/
#define MAX_RECV_FD_NUM 5

#define MAX_RECV_BUF_LEN 0x200000	// 2M
#define MAX_SEND_BUF_LEN 0x200000	// 2M

/***************************************/
/*  module typedef  */
/***************************************/
typedef struct  _mux_port_handle
{
    int devId;
    int devskId;    // for TCP accept socket
    char ipAddrHost[255];

    //socket
    struct sockaddr_in  sinlocal;
    struct sockaddr_in  sinremote;

    //buffer
    int recvLen;
    int sendLen;
    UInt8*  sendBuf;
    UInt8*  recvBuf;

} mux_port_handle, *MPORT_Handle;

typedef struct  _mux_port_tsk
{
	int  (* TskCreate)();
	int  (* TskDestroy)();

	OSA_ThrHndl tskRecvHndl;
	Bool tskRecvLoop;
	Bool istskRecvStopDone;

} mux_port_tsk, *MPORT_Tsk;

typedef struct _mux_port_obj
{
	mux_port_handle pPort;

	mux_port_tsk pTsk;

} mux_port_obj, *MPORT_Obj;


/***************************************/
/* module local  */
/***************************************/
static BOOL bInit = FALSE;
static mux_port_obj interobj_net;

/***************************************/
/*  */
/***************************************/
static int serverOpen(MPORT_Handle pPort, Uint32 port)
{
		OSA_assert(pPort != NULL);

		int status = SDK_SOK;
		int sockAddrLen = sizeof(struct sockaddr);

		pPort->ipAddrHost[0] = 0;
		gethostname(pPort->ipAddrHost, 255);

		pPort->sinlocal.sin_family   = AF_INET;
		pPort->sinlocal.sin_port    = htons(port);
		pPort->sinlocal.sin_addr.s_addr   = htonl(INADDR_ANY);
		bzero(&(pPort->sinlocal.sin_zero),8);

		/** < alloc mem buffer */
		pPort->recvBuf = (UInt8*)SDK_MEM_MALLOC(MAX_RECV_BUF_LEN);
		OSA_assert(pPort->recvBuf != NULL);

		/** < open port and config */
		/** tcp link */
		pPort->devId = socket(AF_INET, SOCK_STREAM, 0);
		OSA_assert(pPort->devId > 0);
		OSA_printf(" Binding interface 0.0.0.0:%d(htons %d)", port, pPort->sinlocal.sin_port);
		status = bind(pPort->devId, (struct sockaddr *)&pPort->sinlocal, sockAddrLen);
		//OSA_assert(status == SDK_SOK);
		if(status == SDK_SOK)
		{
			status = listen(pPort->devId, MAX_RECV_FD_NUM);
			//OSA_assert(status == SDK_SOK);
		}

		#if 0
		int optVal = 0;
		int optLen = sizeof(optVal);
		getsockopt(pPort->devId, SOL_SOCKET, SO_RCVBUF, (char*)&optVal, &optLen);
		OSA_printf("sock recv buf is %d\n", optVal);
		getsockopt(pPort->devId, SOL_SOCKET, SO_SNDBUF, (char*)&optVal, &optLen);
		OSA_printf("sock snd buf is %d\n", optVal);
		#endif

		if(status != SDK_SOK)
		{
			close(pPort->devId);
			pPort->devId = 0;
			SDK_MEM_FREE(pPort->recvBuf);
			pPort->recvBuf = NULL;
			return status;
		}

		return SDK_SOK;
}

/***************************************/
/*  */
/***************************************/
static int serverClose(MPORT_Handle pPort)
{
	OSA_assert(pPort != NULL);

	/** < close port */
	if(pPort->devskId > 0)
	{
		close(pPort->devskId);
		pPort->devskId = 0;
	}
	close(pPort->devId);

	/** < free mem buffer */
	if(pPort->sendBuf != NULL)
		SDK_MEM_FREE(pPort->sendBuf);
	if(pPort->recvBuf != NULL)
		SDK_MEM_FREE(pPort->recvBuf);

    return SDK_SOK;
}

/***************************************/
/*  */
/***************************************/
static int serverWaitConnect(MPORT_Handle pPort)
{
	OSA_assert(pPort != NULL);

	int cntskId;
	//int sin_size = sizeof(struct sockaddr_in);
	int sockAddrLen = sizeof(struct sockaddr);
	struct linger sockLinger;

	if (pPort->devId < 0)
		return OSA_EFAIL;

	cntskId = accept(pPort->devId, (struct sockaddr *)&pPort->sinremote, (socklen_t*)&sockAddrLen);
	if (cntskId > 0)
	{
			sockLinger.l_onoff = 1;
			sockLinger.l_linger = 0;
			setsockopt(cntskId, SOL_SOCKET, SO_LINGER, (char *)&sockLinger, sizeof (sockLinger));
			#if 0
			int optVal = 0;
			int optLen = sizeof(optVal);
			getsockopt(cntskId, SOL_SOCKET, SO_RCVBUF, (char*)&optVal, &optLen);
			OSA_printf("sock recv buf is %d\n", optVal);
			getsockopt(cntskId, SOL_SOCKET, SO_SNDBUF, (char*)&optVal, &optLen);
			OSA_printf("sock snd buf is %d\n", optVal);
			#endif
	}
	else
	{
			OSA_printf(" %s accept new connect failed.", __func__);
			return OSA_EFAIL;
	}

	if (pPort->devskId > 0)
	{
			close(pPort->devskId);
			OSA_printf(" %s close prev connect and accept new connect.", __func__);
			pPort->devskId = cntskId;
			pPort->recvLen = 0;
	}
	else
	{
			pPort->devskId = cntskId;
			pPort->recvLen = 0;
			OSA_printf(" %s accept new connect.", __func__);
	}

	return OSA_SOK;  
}

/***************************************/
/*  */
/***************************************/
static int lookupSync(MPORT_Handle pPort)
{
		OSA_assert(pPort != NULL);

		int stat = 0, headlen = sizeof(DATAHEADER);
		Uint8 *pCur = pPort->recvBuf;
		DATAHEADER *pHead = NULL;

		while (pPort->recvLen > headlen)
		{
			pHead = (DATAHEADER *)pCur;
			if (pHead->flag == 0xFFFF0001)
			{
					stat = pHead->size;
					break;
			}
			else
			{
					pCur++;
					pPort->recvLen--;
			}
		}
		if (pCur != pPort->recvBuf && pPort->recvLen != 0)
		{
			memcpy(pPort->recvBuf, pCur, pPort->recvLen);
			OSA_printf(" %s warnning: search next head len=%d", __func__, pPort->recvLen);
		}

		return stat;
}

/***************************************/
/*  */
/***************************************/
static void* recvierTask(void *pPrm)
{
	MPORT_Obj pPortObj = NULL;
	MPORT_Handle pPort = NULL;
	MPORT_Tsk pTsk = NULL;

	struct timeval timeout;
	fd_set rd_set;
	int result = 0, pause = 0;
	int recvLen = 0, surplusLen = 0;
	int rngstat = SDK_EFAIL, rngId = 0, rngLen = 0, headlen = sizeof(DATAHEADER);
	unsigned int sockAddrLen = sizeof(struct sockaddr);
	DATAHEADER *pHead = NULL;
	int icnt=0;
	unsigned char msgBuf[8];

	OSA_assert(pPrm != NULL);
	pPortObj    = (MPORT_Obj)pPrm;
	pPort   = &(pPortObj->pPort);
	pTsk    = &(pPortObj->pTsk);

	OSA_printf(" %s start.", __func__);
	while (pTsk->tskRecvLoop == TRUE)
	{
		if (pause)
		{
				OSA_waitMsecs(200);
				continue;
		}

		timeout.tv_sec   = 0;
		timeout.tv_usec  = 500000;  // 500ms

		FD_ZERO(&rd_set);
		if (pPort->devId > 0)
		{
				FD_SET(pPort->devId, &rd_set);
		}
		if (pPort->devskId > 0)
		{
				FD_SET(pPort->devskId, &rd_set);
		}
		result = select(FD_SETSIZE, &rd_set, NULL, NULL, &timeout);

		if ( result == 0)
		{
				//OSA_printf(" %s select empty...",__func__);
				continue;
		}

		if ( result == -1 )
		{
				OSA_printf(" %s select error, pause tsk.", __func__);
				pause = 1;
				continue;
		}

		/*************************************/
		recvLen = 0;
		/** < tcp link */
		if (FD_ISSET(pPort->devId , &rd_set))
		{
				// accept new connect
				serverWaitConnect(pPort);
				rngLen = 0;
		}
		if (pPort->devskId > 0 && FD_ISSET(pPort->devskId, &rd_set) && (pPort->recvBuf != NULL) )
		{
				recvLen = recv(pPort->devskId,	pPort->recvBuf + pPort->recvLen,	MAX_RECV_BUF_LEN - pPort->recvLen, 0);
				if (recvLen <= 0)
				{
						close(pPort->devskId);
						pPort->devskId = 0;
						pPort->recvLen = 0;
						rngLen = 0;
						OSA_printf(" %s connect closed by remote.", __func__);
						continue;
				}
		}

		if(recvLen == 0)
			continue;

#if (DEBUG_NET)
		int i = 0;
		printf(" [DEBUG:] %s recv len %d: ", __func__, recvLen);
		for (i = pPort->recvLen; i < pPort->recvLen + recvLen; i++)
		{
			if (i % 16 == 0)
				printf("\n");
			printf("%02x ", pPort->recvBuf[i]);
		}
		printf("\r\n");
#endif

		/*************************************/
		pPort->recvLen += recvLen;	//统计接收到的数据长度
		if (pPort->recvLen == MAX_RECV_BUF_LEN)
		{
				OSA_printf(" %s recv buf is full, clean.", __func__);
				pPort->recvLen = 0;
		}

		while (pPort->recvLen >= headlen)    //长度大于数据头，数据还没取完，继续往里面写数据
		{
				// get data len
				if(rngLen == 0)
				{
					rngLen = lookupSync(pPort);  //同步一帧数据的帧头，返回要读取一帧数据的长度:rngLen。
					if (rngLen <= 0)
						break;
					//OSA_printf(" 1 lookupsync len=%d rngId=%d rnglen=%d.", pPort->recvLen, rngId, rngLen);
				}
				if(rngLen > pPort->recvLen)
				{
					break;
				}

				if(rngLen <= pPort->recvLen)
				{
					//发送数据到 ring buffer中
					put_ring_buffer(  (char*)(pPort->recvBuf+headlen), (rngLen-headlen) );
					char * temp =  (char*)(pPort->recvBuf+headlen);
					//printf("------%02x %02x %02x %02x \n", temp[0], temp[1], temp[2], temp[3]);

					surplusLen = pPort->recvLen - rngLen;

					if(surplusLen)   //剩余长度
					{
						memcpy(pPort->recvBuf, (pPort->recvBuf + rngLen), surplusLen);
						pHead = (DATAHEADER *)(pPort->recvBuf);
						//OSA_printf(" 4 datanext len=%d pktlen=%d.", surplusLen, pHead->size);
					}
					pPort->recvLen = surplusLen;  // recvlen不断减少
					rngLen = 0;
				}

			}	//end---while (pPort->recvLen >= headlen)
		}   //end ----while (pTsk->tskRecvLoop == TRUE)

		OSA_printf(" %s exit.", __func__);
		pTsk->istskRecvStopDone = TRUE;
		return NULL;
}

/***************************************/
/* module  API  */
/***************************************/
void bitsInterfacesCreate(int port)
{
	if(bInit == TRUE)
		return ;

	memset(&interobj_net, 0, sizeof(interobj_net));

	MPORT_Tsk pTsk = &interobj_net.pTsk;
	int status = SDK_SOK;

	if(serverOpen(&interobj_net.pPort, port) != OSA_SOK)
	{
		OSA_printf(" %s create failed.", __func__);
		return ;
	}

	/** < create sender ringbuf */
	// 初始化 ring buffer
	 create_ring_buffer();

	/** < create receive task loop */
	pTsk->tskRecvLoop = TRUE;
	pTsk->istskRecvStopDone = FALSE;
	status = OSA_thrCreate(&pTsk->tskRecvHndl,  recvierTask,  OSA_THR_PRI_DEFAULT,  0,   &interobj_net  );
	OSA_assert(status == OSA_SOK);

	OSA_printf(" %s done.\r\n", __func__);
	bInit = TRUE;

	return ;
}

/***************************************/
/*   */
/***************************************/
void bitsInterfacesDestroy(void)
{
	if(bInit != TRUE)
		return ;

	MPORT_Tsk pTsk = &interobj_net.pTsk;
	//int status = SDK_SOK;

	bInit = FALSE;

	/** < stop receive task loop */
	pTsk->tskRecvLoop = FALSE;
	while (1)
	{
	    OSA_waitMsecs(40);
	    if (pTsk->istskRecvStopDone)
	        break;
	}

	OSA_thrDelete(&pTsk->tskRecvHndl);

	serverClose(&interobj_net.pPort);

	// 删除 ring buffer 分配的空间  ，调用封装好的函数
	 destory_ring_buffer();

	OSA_printf(" %s done.\r\n", __func__);
	return ;
}



