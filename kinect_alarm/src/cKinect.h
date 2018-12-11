/**
  * @file cKinect.h
  * @author Alejandro Solozabal
  * @title cKinect class
  * @brief Class for initialize, configurre and gather photos from kinect
  */

#ifndef CKINECT_H_
#define CKINECT_H_

//// Includes ////
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "FreeImage.h"
#include "common.h"
#include "libfreenect.h"
#include "libfreenect_sync.h"

//// Defines ////

#define LOCAL_PATH		"/home/pi/detections"
#define MAX_TILT_WAIT 	10	// Seconds to wait until the kinect's tilting is complete
#define DEPTH_WIDTH		640 // Depth image's width resolution
#define DEPTH_HEIGHT 	480 // Depth image's height resolution
#define VIDEO_WIDTH		640 // Video image's width resolution
#define VIDEO_HEIGHT	480 // Video image's height resolution

//// Class ////

class cKinect {
public:

	//// Functions ////

	/** @brief Constructor */
	cKinect();

	/** @brief Destructor */
	virtual ~cKinect();

	/** @brief Initializer */
	bool init();

	/** @brief Deinitializer */
	bool deinit();

	/** @brief Run kinect image capture */
	int run();

	/** @brief Stop kinect image capture */
	int stop();

	/** @brief To get depth frame */
	int get_depth_frame(uint16_t *depth_frame);

	/** @brief To get video frame */
	int get_video_frame(uint16_t *video_frame);

	/**
	  * @brief To get change kinect's tilt
	  * @param tilt_angle Wanted kinect's tilt angle, range [-61,61]
	  */
	bool change_tilt(double tilt_angle);

	/**
	  * @brief To get change kinect's led color
	  * @param color Wanted color
	  */
	void change_led_color(freenect_led_options color);

private:

	//// Variables ////

	freenect_context* kinect_ctx;
	freenect_device* kinect_dev;
	pthread_t process_event_thread;
	bool is_kinect_initialize;
	uint16_t buffer_depth[DEPTH_WIDTH*DEPTH_HEIGHT];
	uint16_t buffer_video[VIDEO_WIDTH*VIDEO_HEIGHT];
	static volatile bool done_depth;
	static volatile bool done_video;
	volatile bool running;
	static uint16_t* temp_depth_frame_raw;
	static uint16_t* temp_video_frame_raw;
	static pthread_mutex_t depth_lock;
	static pthread_mutex_t video_lock;
	static pthread_cond_t depth_ready;
	static pthread_cond_t video_ready;

	//// Functions ////

	static void video_cb(freenect_device* dev, void* data, uint32_t timestamp);
	static void depth_cb(freenect_device* dev, void* data, uint32_t timestamp);
	void *kinect_process_events(void);
	static void *kinect_process_events_helper(void *context);
};

#endif /* CKINECT_H_ */
