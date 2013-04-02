#include "libs/helpers.h"
 
int main(int argc, char **argv){

	if(argc < 2){
		printf("Usage: %s <file>\n", argv[0]);
		return 0;
	}

	GMainLoop *loop;
	GstElement *pipeline, *file_src, *decodebin2, *queuev, *ffmpegcolorspace, *videoscale, *videorate, *video_caps, *ffmpegcolorspace2, *video_sink;
	GstBus *bus;

	gst_init(&argc, &argv);

	pipeline = create_pipeline();
	bus = get_bus(pipeline);
	file_src = create_file_src(argv[1]);
	decodebin2 = create_decodebin();
	queuev = create_queue("queuev");
	ffmpegcolorspace = create_ffmpegcolorspace();
	videoscale = create_videoscale();
	videorate = create_videorate();
	video_caps = create_video_caps();
	ffmpegcolorspace2 = create_ffmpegcolorspace();
	video_sink = create_video_sink();

	gst_bin_add_many(pipeline, file_src, decodebin2, queuev, ffmpegcolorspace, videoscale, videorate, video_caps, ffmpegcolorspace2, video_sink, NULL);
	gst_element_link_many(file_src, decodebin2, NULL);
	gst_element_link_many(queuev, ffmpegcolorspace, videoscale, videorate, video_caps, ffmpegcolorspace2, video_sink, NULL);

	g_signal_connect(decodebin2, "pad-added", G_CALLBACK(on_pad_added), pipeline);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	loop = g_main_loop_new(NULL, FALSE);
	gst_bus_add_watch(bus, async_bus_cb, loop);
	g_main_loop_run(loop);

	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(bus);

	return 0;
}