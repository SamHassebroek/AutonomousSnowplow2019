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

private:
	static lidar_hit_map			prv_hit_map;
	static double					prv_resolution;
	static unsigned int				prv_tot_scans_mapped;
	static lidar_data_packet *		prv_data_packet;
	static lidar_handler *          prv_lidar_ref;

public:
									Grid(double width, double height, lidar_handler *lidar);
	void							print_hit_map();
	void							update_hit_map();
	void							update_obj_map();

};