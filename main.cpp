#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "UsageEnvironment.hh"
#include "gst_encode.h"
#include <iostream>


using namespace std;

UsageEnvironment* env;

// To make the second and subsequent client for each stream reuse the same
// input stream as the first client (rather than playing the file from the
// start for each client), change the following "False" to "True":
Boolean reuseFirstSource = False;

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName); // fwd


int main(int argc, char** argv) {

	create_ring_buffer();
	gst_main();

  // Begin by setting up our usage environment:
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  env = BasicUsageEnvironment::createNew(*scheduler);

  UserAuthenticationDatabase* authDB = NULL;
  // Create the RTSP server:
  RTSPServer* rtspServer = RTSPServer::createNew(*env, 8554, authDB);
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


#if 0
  // A H.264 video elementary stream:
  {
    char const* streamName = "live";
    ServerMediaSession* sms
      = ServerMediaSession::createNew(*env, streamName, streamName,
				      descriptionString);
    sms->addSubsession(H264LiveVideoServerMediaSubsession
		       ::createNew(*env, reuseFirstSource));
    rtspServer->addServerMediaSession(sms);

    announceStream(rtspServer, sms, streamName);
  }
  #endif
#if 1
      std::string streamName = "usb1";
      ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName.c_str(), streamName.c_str(), "Live H264 Stream");
      H264LiveServerMediaSession *liveSubSession = H264LiveServerMediaSession::createNew(*env, true);
      sms->addSubsession(liveSubSession);
      rtspServer->addServerMediaSession(sms);

      announceStream(rtspServer, sms, streamName.c_str());
#endif
#if 0
  // A H.264 video elementary stream:
  {
    char const* streamName = "h264ESVideoTest";
    char const* inputFileName = "1test.264";
    ServerMediaSession* sms
      = ServerMediaSession::createNew(*env, streamName, streamName,
                      descriptionString);
    sms->addSubsession(H264FILE_VideoServerMediaSubssion
               ::createNew(*env, inputFileName, reuseFirstSource));//修改为自己实现的servermedia  H264LiveVideoServerMediaSubssion
    rtspServer->addServerMediaSession(sms);

    announceStream(rtspServer, sms, streamName);
  }
#endif


  // Also, attempt to create a HTTP server for RTSP-over-HTTP tunneling.
  // Try first with the default HTTP port (80), and then with the alternative HTTP
  // port numbers (8000 and 8080).
  if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080)) {
    *env << "\n(We use port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-HTTP tunneling.)\n";
  } else {
    *env << "\n(RTSP-over-HTTP tunneling is not available.)\n";
  }

  env->taskScheduler().doEventLoop(); // does not return

  return 0; // only to prevent compiler warning
}

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName) {
  char* url = rtspServer->rtspURL(sms);
  UsageEnvironment& env = rtspServer->envir();
  env << "Play using the URL \"" << url << "\"\n";
  delete[] url;
}
