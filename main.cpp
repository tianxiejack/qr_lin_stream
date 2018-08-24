#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "UsageEnvironment.hh"
#include <iostream>
#include <stdio.h>

#define USE_NET_RECEIVE		(1)			//1:网络接收；0:本地接收
#define ACCESS_CONTROL		(0)			//1:使用用户密码验证，用户名admin，密码admin12345；0:不使用用户密码验证
#define USE_STREAM_NAME	(1)			//1:使用字节流的名字；0:不使用字节流的名字

#if USE_NET_RECEIVE
	#include <app_global_def.h>
	#include <app_bitsInterfaces.h>
	#include "gst_ring_buffer.h"
#else
	#include "gst_encode.h"
#endif

using namespace std;
UsageEnvironment* env;

// To make the second and subsequent client for each stream reuse the same
// input stream as the first client (rather than playing the file from the
// start for each client), change the following "False" to "True":
Boolean reuseFirstSource = False;

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName); // fwd


int main(int argc, char** argv)
{

	printf(" Build Timestamp: date %s %s\r\n", __DATE__, __TIME__);
	int streamIndex = 0;
	if(argc>=2)
		streamIndex = atol(argv[1]);

#if USE_NET_RECEIVE
		int i, runstat = -1;
		int localPort = 17000 ;
		localPort += streamIndex;

		if((argc <= 1) || (runstat == -1) || (localPort < 15000 || localPort > 20000))
		{
				printf(" Usage: a.out [socket port](15000-20000) \r\n");
				printf(" Example: ./a.out 17000 \r\n");
				printf(" \n");
				return 0;
		}

		bitsInterfacesCreate(localPort);
#else
		create_ring_buffer();
		gst_main();
#endif

  // Begin by setting up our usage environment:
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  env = BasicUsageEnvironment::createNew(*scheduler);

  UserAuthenticationDatabase* authDB = NULL;

#if ACCESS_CONTROL
  // To implement client access control to the RTSP server, do the following:
  authDB = new UserAuthenticationDatabase;
  /* authDB->addUserRecord("username1", "password1"); // replace these with real strings */
  authDB->addUserRecord("admin", "admin12345");
  // Repeat the above with each <username>, <password> that you wish to allow
  // access to the server.
#endif

  // Create the RTSP server.  Try first with the default port number (554),
    // and then with the alternative port number (8554):
    RTSPServer* rtspServer;
    portNumBits rtspServerPortNum = 8550;
    rtspServerPortNum += streamIndex;
    rtspServer = RTSPServer::createNew(*env, rtspServerPortNum, authDB);
    if (rtspServer == NULL) {
      rtspServer = RTSPServer::createNew(*env, rtspServerPortNum, authDB);
    }
    if (rtspServer == NULL) {
      *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
      exit(1);
    }

  char const* descriptionString
    = "Session streamed by \"testOnDemandRTSPServer\"";

  // Set up each of the possible streams that can be served by the
  // RTSP server.  Each such stream is implemented using a
  // "ServerMediaSession" object, plus one or more
  // "ServerMediaSubsession" objects for each audio/video substream.


  	  OutPacketBuffer::maxSize = 600000;
#if USE_STREAM_NAME
      std::string streamName = "stream";
      char strTmp[8] = "";
      sprintf(strTmp, "%d", streamIndex);
      streamName += strTmp;
#else
  	  std::string streamName = "";
#endif
      ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName.c_str(), streamName.c_str(), "Live H264 Stream");
      H264LiveServerMediaSession *liveSubSession = H264LiveServerMediaSession::createNew(*env, true);
      sms->addSubsession(liveSubSession);
      rtspServer->addServerMediaSession(sms);

      announceStream(rtspServer, sms, streamName.c_str());


  // Also, attempt to create a HTTP server for RTSP-over-HTTP tunneling.
  // Try first with the default HTTP port (80), and then with the alternative HTTP
  // port numbers (8000 and 8080).
  if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080)) {
    *env << "\n(We use port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-HTTP tunneling.)\n";
  } else {
    *env << "\n(RTSP-over-HTTP tunneling is not available.)\n";
  }

  env->taskScheduler().doEventLoop(); // does not return

#if USE_NET_RECEIVE
  bitsInterfacesDestroy();
#endif

  return 0; // only to prevent compiler warning
}

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName) {
  char* url = rtspServer->rtspURL(sms);
  UsageEnvironment& env = rtspServer->envir();
  env << "Play using the URL \"" << url << "\"\n";
  delete[] url;
}
