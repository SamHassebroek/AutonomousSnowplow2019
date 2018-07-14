#include <ctime>
#include <thread>
#include "snowplow_type.h"
#include "lidar_handler.h"
//#include "local_handler.h"
//#include "orientation_handler.h"
#include "Grid.h"

int main() {

#if MAIN_TIMING
	auto current_time = clock();
#endif


	/*lidar we are using with starting coordinates*/
	lidar_handler Lidar(2.5, 0.0);
	
	/*create the grid with a reference to the lidar that it will display hits for*/
	Grid Grid(5.0, 5.0, &Lidar);

	//local_handler local;

	/*start the threads for the lidar and the grid*/
	//std::thread lidar_thread(&lidar_handler::run, lidar);
	//std::thread grid_thread(&Grid::run, Grid_ob);

	//std::thread local_thread(&local_handler::run, local);
	
	//local_thread.join();

	//orientation_handler a;
	//std::thread angle_thread(&orientation_handler::run, a);

	//angle_thread.join();

	//run 
	int i = 0;
	while (1) {


		//perform scan
		Lidar.perform_scan();
		Lidar.analyze_scan();
		Grid.update_hit_map();
		if (i % 50 == 0) {
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

		i++;
	}
		
	return 0;
}