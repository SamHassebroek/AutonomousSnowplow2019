#pragma once

//general includes
#include <iostream>
#include <vector>
#include "lidar_handler.h"
#include <ctime>

//types
#include "snowplow_type.h"

using namespace std;

class Grid {

public:
									Grid(lidar_handler *lidar);

private:
	unsigned long long				prv_total_scans_mapped;
	lidar_hit_map					prv_hit_map;
	lidar_data_packet *				prv_data_packet;
	lidar_handler *					prv_lidar_ref;

public:
	void							print_hit_map();
	void							update_hit_map();

};