//only include once and also winsock version
#pragma once
#pragma comment(lib, "ws2_32.lib")

//doesn't give warnings for old stuff
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//standard includes
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

//includes for websockets -- these are in a certain order
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <ctime>

//types
#include "snowplow_type.h"

//winsock version
#define SCK_VERSION2 0x0202

class lidar_handler {

public:
	                           lidar_handler();

private:
	vector<string>             prv_raw_hex_data;
	lidar_data_packet          prv_raw_data;
	lidar_data_packet          prv_safe_data;
	bool                       prv_raw_data_ready;

public:
	bool                       perform_scan();
	void                       analyze_scan();
	int                        convert_to_decimal(string num);
	lidar_data_packet *        get_data();
	bool                       data_is_ready();

};

