#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/gstclock.h>
#include <gst/gstelement.h>
#include "gst_encode.h"
#include <iostream>

using namespace std;

#define  USE_H264_ENCODE_FILE	(0)

#define  USE_UDPSINK			(0)
#define  USE_DISPLAYSINK		(0)
#define  USE_FAKESINK			(0)

void * thread(void * a);
int gst_test(int argc, char *argv[]);
static gboolean bus_call(GstBus *bus, GstMessage *message, gpointer user_data);

typedef struct _CustomData {
	GstElement *pipeline, *source, *qtdemux, *h264parse, *h264dec, *videoconvert, *x264enc, *h264parse1, *appsink, *glimagesink, *rtph264pay, *udpsink;

	GstBus *bus;
	gboolean playing;
	GMainLoop *loop;

	GstStateChangeReturn ret;

	GstCaps *caps_enc_to_parse;
	GstCaps *caps_dec_to_videocovert;

	int height;
	int width;
	int framerate;
	char format[30];

} CustomData;

FILE *fd;
void gst_main()
{
	pthread_t id;
	fd = 	fopen("./test.264", "wb");
	int ret = pthread_create(&id, NULL, thread, NULL);
	if (ret != 0)
	{
		cout << "线程创建错误！" << endl;
		exit(-1);
	}
//	pthread_join(id, NULL);
}

void * thread(void * a)
{
	int argc = 0;
	char * argv = NULL;
	gst_test( argc,  &argv);

	return NULL;
}

static GstPadProbeReturn enc_buffer(GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
{

	CustomData *pData = (CustomData *)user_data;
	gint iBufSize = 0;
	gchar * pbuffer = NULL;
	gint x,y;
	GstMapInfo map;
	guint16 *ptr, t;
	GstBuffer *buffer;

	buffer = GST_PAD_PROBE_INFO_BUFFER(info);

	if(buffer == NULL)
	{
		return GST_PAD_PROBE_OK;
	}


	if (gst_buffer_map(buffer, &map, GST_MAP_READ))
	{
		//do something here.
		iBufSize = gst_buffer_get_size(buffer);

		put_ring_buffer((char *)map.data, iBufSize);
		fwrite(map.data, 1, iBufSize, fd);

#if 0

		g_print ("encode size = %d, appsink0_buffer = %02x %02x %02x %02x %02x\n",
					iBufSize, map.data[0], map.data[1], map.data[2], map.data[3], map.data[4]);
		g_print ("encode size = %d, appsink0_buffer = %02x %02x %02x %02x %02x\n",
			iBufSize, map.data[0], map.data[1], map.data[2], map.data[3], map.data[4]);

		g_print ("encode size = %d, appsink0_buffer = %02x %02x %02x %02x %02x\n",
			iBufSize, map.data[0], map.data[1], map.data[2], map.data[3], map.data[4]);
		for (int i=0; i<iBufSize; i++)
		{
			g_print("[%d]%02x ",i,map.data[i]);
		}
		g_print("\n");
#endif

		gst_buffer_unmap(buffer, &map);
	}
	GST_PAD_PROBE_INFO_DATA(info) = buffer;

	return GST_PAD_PROBE_OK;
}


CustomData g_customData;

int gst_test(int argc, char *argv[])
{
	CustomData* p_customData = &g_customData;
	p_customData->framerate = 30;
	p_customData->width = 720;
	p_customData->height = 576;

	/* Initialize GStreamer */
	gst_init (&argc, &argv);

	/* Create the elements */

#if USE_H264_ENCODE_FILE
	p_customData->source = gst_element_factory_make ("filesrc", NULL);
	p_customData->qtdemux = gst_element_factory_make("qtdemux", NULL);
	p_customData->h264parse = gst_element_factory_make ("h264parse", NULL);
	p_customData->h264dec = gst_element_factory_make ("avdec_h264", NULL);
#else
	p_customData->source = gst_element_factory_make ("videotestsrc", NULL);
#endif

	p_customData->videoconvert = gst_element_factory_make ("videoconvert", NULL);

#if USE_DISPLAYSINK
	p_customData->glimagesink = gst_element_factory_make ("xvimagesink", NULL);
#else
	p_customData->x264enc = gst_element_factory_make ("x264enc", NULL);
	p_customData->h264parse1 = gst_element_factory_make ("h264parse", NULL);

#if	USE_UDPSINK
	p_customData->rtph264pay = gst_element_factory_make ("rtph264pay", NULL);
	p_customData->udpsink = gst_element_factory_make ("udpsink", NULL);
#else
	p_customData->appsink = gst_element_factory_make ("fakesink", NULL);
#endif /*USE_UDPSINK*/

#endif /*USE_DISPLAYSINK*/

	/* Create the empty pipeline */
	p_customData->pipeline = gst_pipeline_new ("test-pipeline");

	if (!p_customData->pipeline		||
		!p_customData->source			||
#if USE_H264_ENCODE_FILE
	!p_customData->qtdemux		||
		!p_customData->h264parse	||
		!p_customData->h264dec		||
#endif
		!p_customData->videoconvert ||
#if USE_DISPLAYSINK
		!p_customData->glimagesink
#else
		!p_customData->x264enc			||
		!p_customData->h264parse1	||
#if USE_UDPSINK
		!p_customData->rtph264pay	||
		!p_customData->udpsink
#else
		!p_customData->appsink
#endif /* USE_UDPSINK */

#endif /* USE_DISPLAYSINK */
		){
		g_printerr ("Not all elements could be created.\n");
		return -1;
	}

	/* Build the pipeline */

	gst_bin_add_many (
					GST_BIN(p_customData->pipeline),
					p_customData->source,

#if USE_H264_ENCODE_FILE
					//p_customData->qtdemux ,
					p_customData->h264parse ,
					p_customData->h264dec,
#endif
					p_customData->videoconvert ,
#if USE_DISPLAYSINK
					p_customData->glimagesink,
#else
					p_customData->x264enc,
					p_customData->h264parse1 ,
#if USE_UDPSINK
					p_customData->rtph264pay ,
					p_customData->udpsink ,
#else
					p_customData->appsink ,
#endif /* USE_UDPSINK */
#endif /* USE_DISPLAYSINK */
					NULL);



	if (gst_element_link_many (p_customData->source,
#if USE_H264_ENCODE_FILE
		//p_customData->qtdemux ,
		p_customData->h264parse ,
		p_customData->h264dec,
#endif /* USE_H264_ENCODE_FILE */

		p_customData->videoconvert ,

#if USE_DISPLAYSINK
		p_customData->glimagesink,
#else
		p_customData->x264enc,
#endif /* USE_DISPLAYSINK */
	NULL) != TRUE)
	{
		g_printerr ("Elements could not be linked.\n");
		gst_object_unref (p_customData->pipeline);
		return -1;
	}

#if (!USE_DISPLAYSINK)
	if (gst_element_link_many (
				p_customData->h264parse1,
#if USE_UDPSINK
				p_customData->rtph264pay,
				p_customData->udpsink,
#else
				p_customData->appsink,
#endif /* USE_UDPSINK */
	 NULL) != TRUE)
	{
		g_printerr ("Elements could not be linked.\n");
		gst_object_unref (p_customData->pipeline);
		return -1;
	}
#endif

	/* Modify the source's properties */
#if USE_H264_ENCODE_FILE
	g_object_set (G_OBJECT(p_customData->source), "location", "E:\\gstreamer\\1.0\\x86\\bin\\src.h264", NULL);
	g_print ("USE: filesrc:%s. \n", "E:\\gstreamer\\1.0\\x86\\bin\\src.h264");
#else

#if 1
	int pattern = 18; //18
	g_object_set (G_OBJECT(p_customData->source), "is-live", 1, NULL);
	g_object_set (G_OBJECT(p_customData->source), "do-timestamp", 1, NULL);
	g_object_set (G_OBJECT(p_customData->source), "pattern", pattern, NULL);
	g_print ("USE: videotestsrc:pattern=%d .\n", pattern);
#else
	g_object_set (G_OBJECT(p_customData->source), "device", "/dev/video1", NULL);
#endif

#endif

#if USE_DISPLAYSINK
	//do nothing
	g_print ("USE: glimagesink, Local playing .\n");
#else

//	g_object_set (G_OBJECT(p_customData->x264enc), "byte-stream" , 1, NULL);
//	g_object_set (G_OBJECT(p_customData->x264enc), "key-int-max" , 25, NULL);  //bitrate
//	g_object_set (G_OBJECT(p_customData->x264enc), "bitrate" , 10000, NULL);

	p_customData->caps_enc_to_parse = gst_caps_new_simple("video/x-h264",
		//"width", G_TYPE_INT, p_customData->width,
		//"height", G_TYPE_INT, p_customData->height,
		"stream-format", G_TYPE_STRING, "byte-stream",
	   //  "framerate", GST_TYPE_FRACTION, 30, 1,
		NULL);

	if(!gst_element_link_filtered(p_customData->x264enc, p_customData->h264parse1, p_customData->caps_enc_to_parse))
	{
		g_warning ("Failed to link element1 and element2!");
		g_printerr ("Elements filtered failed.\n");
		gst_object_unref (p_customData->pipeline);
		return -1;
	}
	gst_caps_unref(p_customData->caps_enc_to_parse);

	GstPad *h264parse_pad = NULL;
	h264parse_pad = gst_element_get_static_pad(p_customData->x264enc,"src");
	gst_pad_add_probe(h264parse_pad, GST_PAD_PROBE_TYPE_BUFFER, (GstPadProbeCallback) enc_buffer, p_customData, (GDestroyNotify)NULL);
	gst_object_unref(h264parse_pad);

#if USE_UDPSINK
	int port = 17000;
	g_object_set (G_OBJECT(p_customData->rtph264pay), "config-interval" , 1, NULL);
	g_object_set (G_OBJECT(p_customData->udpsink), "host", "192.168.3.116", NULL);
	g_object_set (G_OBJECT(p_customData->udpsink), "port", port, NULL);
	g_object_set (G_OBJECT(p_customData->udpsink), "sync", false, NULL);
	g_print ("UDP:  \n HostIP:192.168.3.116 \n   Port:%d \n ( Use VLC play method: Open *.sdp file) \n", port);
#elif USE_APPSINK
	g_object_set (G_OBJECT(p_customData->appsink), "drop", 1, NULL);
#else
	//do nothing
#endif

#endif /* USE_DISPLAYSINK */

	p_customData->loop = g_main_loop_new(NULL, FALSE);
	   /* Start playing   */
	p_customData->ret = gst_element_set_state (p_customData->pipeline, GST_STATE_PLAYING);
	if (p_customData->ret == GST_STATE_CHANGE_FAILURE) {
		g_printerr ("Unable to set the pipeline to the playing state.\n");
		gst_object_unref (p_customData->pipeline);
		return -1;
	}

	/* Wait until error or EOS */
	p_customData->bus = gst_element_get_bus(p_customData->pipeline);
	gst_bus_add_watch(p_customData->bus, bus_call, p_customData->loop);

	g_main_loop_run(p_customData->loop);

	/* Free resources */
	gst_object_unref (p_customData->bus);
	gst_element_set_state (p_customData->pipeline, GST_STATE_NULL);
	gst_object_unref (p_customData->pipeline);
	return 0;
}


/* bus messages processing, similar to all gstreamer examples  */
gboolean bus_call(GstBus *bus, GstMessage *message, gpointer user_data)
{
	GMainLoop * loop = (GMainLoop *)user_data;

	switch(GST_MESSAGE_TYPE(message))
	{
	case GST_MESSAGE_ERROR:
		{
			GError *err = NULL;
			gchar *name, *debug = NULL;
			name = gst_object_get_path_string (message->src);
			gst_message_parse_error (message, &err, &debug);
			g_printerr ("ERROR: from element %s: %s\n", name, err->message);
			if (debug != NULL)
				g_printerr ("Additional debug info:\n%s\n", debug);
			g_error_free (err);
			g_free (debug);
			g_free (name);
			g_main_loop_quit (loop);
			break;
		}
	case GST_MESSAGE_WARNING:
		{
			GError *err = NULL;
			gchar *name, *debug = NULL;
			name = gst_object_get_path_string (message->src);
			gst_message_parse_warning (message, &err, &debug);
			g_printerr ("ERROR: from element %s: %s\n", name, err->message);
			if (debug != NULL)
				g_printerr ("Additional debug info:\n%s\n", debug);
			g_error_free (err);
			g_free (debug);
			g_free (name);
			break;
		}
	case GST_MESSAGE_EOS:
		g_print ("Got EOS\n");
		g_main_loop_quit (loop);
		break;

	default:
		break;
	}

	return TRUE;
}
