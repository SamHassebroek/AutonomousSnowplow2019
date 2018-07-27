#include <ctime>
#include <thread>

#include "snowplow_type.h"
#include "lidar_handler.h"
#include "local_handler.h"
#include "orientation_handler.h"
#include "Grid.h"

/*----------------------------------------------------------------
                          main variables
----------------------------------------------------------------*/
unsigned long long              main_loop_iterations = 0;
unsigned long long              total_failed_scans   = 0;
atomic<double>                  orientation = 0.0;
atomic<double>                  x_position;
atomic<double>                  y_position;

int main() {

#if MAIN_TIMING
	auto current_time = clock();
#endif

	/*---------------------------------------
	construct interfaces
	---------------------------------------*/
	lidar_handler          Lidar(1.0, 1.0);
	grid_handler           Grid(&Lidar);
	decawave_handler       Location(&x_position, &y_position);
	orientation_handler    Orientation(&orientation);//ref to orientation

	/*---------------------------------------
	start orientation and location threads.
	- eventually start stopsign here too
	---------------------------------------*/
	thread location_thread(&decawave_handler::run, Location);
	thread orientation_thread(&orientation_handler::run, Orientation);

	/*---------------------------------------
	main snowplow execution loop
	---------------------------------------*/
	while (1) {

		/*---------------------------------------
		Need to add in functionality to update
		the orientation to the most recent value.
		Also need to update the location to the 
		most recent value. Since this is
		relatively slow maybe use dead reckoning

		Also need to think about the orientation 
		upon the start of the run. Since IMU upon
		startup is 0 then the field(grid) will 
		generate thinking the plow is pointing 
		forward at exactly 90 degress. If it isnt
		then field orientation as a whole could 
		be off. 
		---------------------------------------*/

		/*---------------------------------------
		perform scan and check for success
		---------------------------------------*/
		if (!Lidar.perform_scan()) {
			cout << "Error performing scan. Trying again..." << endl;
			total_failed_scans += 1;
			continue;
		}

		/*---------------------------------------
		analyze scan (change raw hex to angle and
		distance pairs)
		---------------------------------------*/
		Lidar.analyze_scan();

		/*---------------------------------------
		update map of hits
		---------------------------------------*/
		Grid.update_hit_map();

		/*---------------------------------------
		print hit/object map
		---------------------------------------*/
		if (main_loop_iterations % 50 == 0) {
			cout << "=====================================================" << endl;
			Grid.print_obj_map();
			cout << "Total failed scans: " << total_failed_scans << " ";
			cout << "Total scans mapped: " << main_loop_iterations << endl;
		}

#if MAIN_TIMING
		//calculate run time
		double duration = (std::clock() - current_time) / (double)CLOCKS_PER_SEC * 1000.0;
		current_time = clock();
		//don't print first reading
		if (i != 0) {
			std::cout << "Main loop execution time: " << duration << " ms\n";
		}
#endif

		main_loop_iterations++;
	}

}