#include "Grid.h"

/*---------------------------------------
Grid constructor - builds the hit map
based off of size of field
---------------------------------------*/
Grid::Grid(lidar_handler *lidar) {

	prv_total_scans_mapped = 0;
	prv_lidar_ref = lidar;

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

void Grid::print_hit_map() {
	string map = "";
	for (int j = 0; j < prv_hit_map.size(); j++) {
		for (int i = prv_hit_map[j].size() - 1; i > 0; i--) {
			map.append(to_string(prv_hit_map[i][j]));
			string test = to_string(prv_hit_map[i][j]);
			for (int w = 0; w < 3 - test.length(); w++) {
				map.append(" ");
			}
			map.append(" ");
		}
		map.append("\n");
	}
	cout << map;
}

void Grid::update_hit_map() {
	if ((*prv_lidar_ref).data_is_ready()) {
		/*get reference to data*/
		prv_data_packet = (*prv_lidar_ref).get_data();

		for (int i = 0; i < LIDAR_DATA_POINTS; i++) {
			//map each point

			double angle = get<0>((*prv_data_packet)[i]);
			double distance = get<1>((*prv_data_packet)[i]);

			if (angle < 0) {
				angle = 360 - abs(angle);
			}
			double x_loc = (cos(angle*3.141592 / 180.0) * distance);
			double y_loc = (sin(angle*3.141592 / 180.0) * distance);
			//cout << "Angle of: " << angle << " x location: " << x_loc << " y location: " << y_loc << endl;
			
			int x_index = floor(x_loc / MAP_RESOLUTION_M);
			int y_index = floor(y_loc / MAP_RESOLUTION_M);
			if (x_index < 0 || x_index > prv_hit_map.size() - 1 || y_index < 0 || y_index > prv_hit_map[0].size() - 1) {
				continue;
			}

			if (prv_hit_map[floor(x_loc / MAP_RESOLUTION_M)][floor(y_loc / MAP_RESOLUTION_M)] < 999) {
				prv_hit_map[floor(x_loc / MAP_RESOLUTION_M)][floor(y_loc / MAP_RESOLUTION_M)] += 1;
			}

		}
		//cout << "finished processing data." << endl;
		prv_total_scans_mapped++;

	}
	else {
		//cout << "data isn't ready yet..." << endl;
	}
}
