#include <ctime>
#include <thread>
#include "snowplow_type.h"
#include "lidar_handler.h"
//#include "local_handler.h"
//#include "orientation_handler.h"
#include "Grid.h"

/*----------------------------------------------------------------
local variables
----------------------------------------------------------------*/
unsigned long long              main_loop_iterations = 0;


int main() {

#if MAIN_TIMING
	auto current_time = clock();
#endif

	/*---------------------------------------
	construct lidar object
	---------------------------------------*/
	lidar_handler Lidar;
	
	
	Grid Grid(&Lidar);


	/*---------------------------------------
	main snowplow execution loop
	---------------------------------------*/
	while (1) {

		/*---------------------------------------
		perform scan and check for success
		---------------------------------------*/
		if (!Lidar.perform_scan()) {
			cout << "Error performing scan. Trying again..." << endl;
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
		if (main_loop_iterations % 50 == 0) {
			cout << "=====================================================" << endl;
			Grid.print_hit_map();
		}

#if MAIN_TIMING
		//calculate run time
		double duration = (std::clock() - current_time) / (double)CLOCKS_PER_SEC * 1000.0;
		current_time = clock();
		//print run time
		if (i != 0) {//don't print first reading
			std::cout << "Main loop execution time: " << duration << " ms\n";
		}
#endif

		main_loop_iterations++;
	}

}