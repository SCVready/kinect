/**
 * @author Alejandro Solozabal
 *
 * @file cKinect.hpp
 *
 */

#ifndef CKINECT_H_
#define CKINECT_H_

//// Includes ////
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "global_parameters.hpp"
#include <libfreenect/libfreenect.h>
#include <libfreenect/libfreenect_sync.h>
#include <syslog.h>
#include "log.hpp"
#include "common.hpp"


//// Defines ////

#define MAX_TILT_WAIT 	10	// Seconds to wait until the kinect's tilting is complete

//// Class ////

class cKinect {
public:

    //// Functions ////

    /** @brief Constructor */
    cKinect();

    /** @brief Destructor */
    virtual ~cKinect();

    /** @brief Initializer */
    int init();

    /** @brief Deinitializer */
    int deinit();

    /** @brief Run kinect image capture */
    int start();

    /** @brief Stop kinect image capture */
    int stop();

    /** @brief To get depth frame */
    int get_depth_frame(uint16_t *depth_frame, uint32_t *timestamp);

    /** @brief To get video frame */
    int get_video_frame(uint16_t *video_frame, uint32_t *timestamp);

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

    /** @brief Check if kinect is running */
    bool is_kinect_running();

private:

    //// Variables ////

    // Freenect context strucutres
    freenect_context* kinect_ctx;
    freenect_device* kinect_dev;

    // Thread
    pthread_t process_event_thread;

    // Flags
    bool is_kinect_initialize;
    volatile bool running;

    // Frame pointers
    static uint16_t* temp_depth_frame_raw;
    static uint16_t* temp_video_frame_raw;

    // Frame timestamp
    static uint32_t temp_depth_frame_timestamp;
    static uint32_t temp_video_frame_timestamp;

    // Thread sinc
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