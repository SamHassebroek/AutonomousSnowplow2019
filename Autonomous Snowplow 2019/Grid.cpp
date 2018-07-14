#include "Grid.h"




unsigned int Grid::prv_tot_scans_mapped = 0;
double Grid::prv_resolution = 0.10;
lidar_data_packet* Grid::prv_data_packet;
lidar_hit_map Grid::prv_hit_map;
lidar_handler* Grid::prv_lidar_ref;


//constructor
Grid::Grid(double width, double height, lidar_handler *lidar) {
	prv_lidar_ref = lidar;
	for (int i = 0; i < width / prv_resolution; i++) {//x axis
		vector<int> temp;
		for (int j = 0; j < height / prv_resolution; j++) {//y axis
			temp.push_back(0);
		}
		prv_hit_map.push_back(temp);
	}
	
	//cout << "done with grid" << endl;
	print_hit_map();
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
		//cout << "data points: " << prv_data_packet->data_points << endl;
		//cout << "data is ready to process." << endl;

		int tot_data_points = (*prv_lidar_ref).get_num_data_points();
		//cout << "number of data points to map: " << (*prv_data_packet).size() << endl;

		for (int i = 0; i < tot_data_points; i++) {
			//map each point

			double angle = get<0>((*prv_data_packet)[i]);
			double distance = get<1>((*prv_data_packet)[i]);

			if (angle < 0) {
				angle = 360 - abs(angle);
			}
			double x_loc = (cos(angle*3.141592 / 180.0) * distance) + (*prv_lidar_ref).get_x_loc();
			double y_loc = (sin(angle*3.141592 / 180.0) * distance) + (*prv_lidar_ref).get_y_loc();
			//cout << "Angle of: " << angle << " x location: " << x_loc << " y location: " << y_loc << endl;
			
			int x_index = floor(x_loc / prv_resolution);
			int y_index = floor(y_loc / prv_resolution);
			if (x_index < 0 || x_index > prv_hit_map.size() - 1 || y_index < 0 || y_index > prv_hit_map[0].size() - 1) {
				continue;
			}

			if (prv_hit_map[floor(x_loc / prv_resolution)][floor(y_loc / prv_resolution)] < 999) {
				prv_hit_map[floor(x_loc / prv_resolution)][floor(y_loc / prv_resolution)] += 1;
			}

		}
		//cout << "finished processing data." << endl;
		prv_tot_scans_mapped++;

	}
	else {
		//cout << "data isn't ready yet..." << endl;
	}
}
