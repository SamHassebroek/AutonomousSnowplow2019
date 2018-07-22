#include "Grid.h"

/*---------------------------------------
Grid constructor - builds the hit map
based off of size and resolution of field
---------------------------------------*/
grid_handler::grid_handler(lidar_handler * lidar) {

	prv_total_scans_mapped = 0;
	prv_lidar_ref          = lidar;
	prv_lidar_map_x_idx    = floor((prv_lidar_ref->get_x_pos()) / MAP_RESOLUTION_M);
	prv_lidar_map_y_idx    = floor((prv_lidar_ref->get_y_pos()) / MAP_RESOLUTION_M);

	/*---------------------------------------
	building x-axis
	---------------------------------------*/
	for (int i = 0; i < FIELD_WIDTH_M / MAP_RESOLUTION_M; i++) {
		vector<int> temp;
		/*---------------------------------------
		building y-axis
		---------------------------------------*/
		for (int j = 0; j < FIELD_LENGTH_M / MAP_RESOLUTION_M; j++) {
			temp.push_back(0);
		}
		prv_hit_map.push_back(temp);
	}
}

/*---------------------------------------
Adds lidar hits to the map
---------------------------------------*/
void grid_handler::update_hit_map() {
	if (!prv_lidar_ref->data_is_ready()) {
		return;
	}
	prv_data_packet = prv_lidar_ref->get_data();

	/*---------------------------------------
	Go through and map each data point that 
	the	lidar returned
	---------------------------------------*/
	for (int i = 0; i < LIDAR_DATA_POINTS; i++) {

		/*---------------------------------------------
		Following does this for each point:
		1. get angle and distance pair from lidar data
		2. if angle is negative make it positive
		3. turn that pair into an x, y location
		4. if its outside the map dont map it
		5. count it unless count is at 999
		---------------------------------------------*/
		double angle    = get< 0 >( ( * prv_data_packet )[ i ]);
		double distance = get< 1 >( ( * prv_data_packet )[ i ]);

		if (angle < 0) {
			angle = 360 - abs(angle);
		}

		double x_loc = ( cos( angle * M_PI / 180.0) * distance ) + prv_lidar_ref->get_x_pos();
		double y_loc = ( sin( angle * M_PI / 180.0) * distance ) + prv_lidar_ref->get_y_pos();
			
		int x_index = floor(x_loc / MAP_RESOLUTION_M);
		int y_index = floor(y_loc / MAP_RESOLUTION_M);

		if (x_index < 0 || x_index > prv_hit_map.size() - 1 || y_index < 0 || y_index > prv_hit_map[0].size() - 1) {
			continue;
		}

		if (prv_hit_map[floor(x_loc / MAP_RESOLUTION_M)][floor(y_loc / MAP_RESOLUTION_M)] < 999) {
			prv_hit_map[floor(x_loc / MAP_RESOLUTION_M)][floor(y_loc / MAP_RESOLUTION_M)] += 1;
		}

	}
	prv_total_scans_mapped++;
	prv_lidar_map_x_idx = floor((prv_lidar_ref->get_x_pos()) / MAP_RESOLUTION_M);
	prv_lidar_map_y_idx = floor((prv_lidar_ref->get_y_pos()) / MAP_RESOLUTION_M);
}

/*---------------------------------------
prints the lidar hit map so that 0,0 is
in the bottom left corner
---------------------------------------*/
void grid_handler::print_hit_map() {
	string map = "";
	/*-------------------------------------------------
	  ^^^^^^^^^^      Need to print the map with y
	y ||||||||||      starting at max and x starting at
	  --------->      0 since this is a vector of
	      x           vectors.
	-------------------------------------------------*/ 
	for (int y = prv_hit_map[0].size() - 1; y >= 0; y--) {
		for (int x = 0; x <  prv_hit_map.size(); x++) {

			if (prv_lidar_map_x_idx == x && prv_lidar_map_y_idx == y) {
				map.append("LLLL");
				continue;
			}

			map.append(to_string(prv_hit_map[x][y]));
			string test = to_string(prv_hit_map[x][y]);
			for (int w = 0; w < 3 - test.length(); w++) {
				map.append(" ");
			}
			map.append(" ");
		}
		map.append("\n\n");
	}
	cout << map;
}

/*---------------------------------------
prints the lidar hit map so that 0,0 is
in the bottom left corner
---------------------------------------*/
void grid_handler::print_obj_map() {
	string map = "";
	/*-------------------------------------------------
	^^^^^^^^^^      Need to print the map with y
	y ||||||||||      starting at max and x starting at
	--------->      0 since this is a vector of
	x           vectors.
	-------------------------------------------------*/
	for (int y = prv_hit_map[0].size() - 1; y >= 0; y--) {
		for (int x = 0; x < prv_hit_map.size(); x++) {

			if (prv_lidar_map_x_idx == x && prv_lidar_map_y_idx == y) {
				map.append("L");
				continue;
			}

			if (prv_hit_map[x][y] > 500) {
				map.append("M");
			}
			else {
				map.append(".");
			}

			map.append(" ");
		}
		map.append("\n");
	}
	cout << map;
}
