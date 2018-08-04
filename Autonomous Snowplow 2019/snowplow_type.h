#pragma once

#include "macro_defs.h"
#include <vector>
#include <tuple>
#include <atomic>

using namespace std;

/*---------------------------------------------------
Various project config options - change as necessary.
Certain macro definitions as well as other options 
used here can be found in macro_defs.cpp

Note: If field is defined as custom you can change
      the width and length in the fied macros section
---------------------------------------------------*/
#define LIDAR_IN_USE      TIM_551
#define DECAWAVE_COM_PORT ( "\\\\.\\COM3" )
#define IMU_COM_PORT      ( "\\\\.\\COM6" )
#define FIELD             CUSTOM

/*---------------------------------------
lidar macros
---------------------------------------*/
#if ( LIDAR_IN_USE == TIM_551 )
	#define LIDAR_IP_ADDR     ( "169.254.185.233" )
	#define LIDAR_PORT        ( 2111 )
	#define LIDAR_START_ANGLE ( -45.0 )
	#define LIDAR_ANG_RES     ( 1.0 )
	#define LIDAR_DATA_POINTS ( 271 )
	#define LIDAR_MSG_VLD     ( "RSSI1" )

#elif ( LIDAR_IN_USE == LMS_511 )
	/*---------------------------------------
	These macros needs to be updated
	with the actual lms 511 info
	---------------------------------------*/
	#define LIDAR_IP_ADDR     ( "169.254.185.233" )
	#define LIDAR_PORT        ( 2112 )
	#define LIDAR_START_ANGLE ( -5.0 )
	#define LIDAR_ANG_RES     ( 0.5 )
	#define LIDAR_DATA_POINTS ( 271 )
	#define LIDAR_MSG_VLD     ( "RSSI1" )

#endif

/*---------------------------------------
Field macros - set custom width and
length under field == custom
---------------------------------------*/
#if( FIELD == SINGLE_I )
#define FIELD_WIDTH_M    ( 4.0 )
#define FIELD_LENGTH_M   ( 15.0 )
#elif( FIELD == TRIPLE_I )
#define FIELD_WIDTH_M    ( 4.0 )
#define FIELD_LENGTH_M   ( 15.0 )
#elif( FIELD == CUSTOM )
#define FIELD_WIDTH_M    ( 4.0 )
#define FIELD_LENGTH_M   ( 4.0 )
#endif

#define MAP_RESOLUTION_M ( 0.1 )
#define MAP_OBJ_THRESH   ( 100 )

/*---------------------------------------
Set these to 1 to compile the timing code
and see output in the console
---------------------------------------*/
#define LIDAR_TIMING ( 0 )
#define MAIN_TIMING  ( 0 )

/*---------------------------------------
Project specific types
---------------------------------------*/
typedef vector<tuple<double, double>>     lidar_data_packet;
typedef vector<vector<int>>               lidar_hit_map;
typedef struct {
	double x;
	double y;
} snowplow_nav_point;