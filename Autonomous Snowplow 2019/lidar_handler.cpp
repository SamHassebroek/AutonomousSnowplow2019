#include "lidar_handler.h"

/*--------------------------------------------
Constructor
--------------------------------------------*/
lidar_handler::lidar_handler(double x, double y) {
	/*--------------------------------------------
	Set x and y location of the lidar when program 
	starts, initialize other variables
	--------------------------------------------*/
	prv_x_position = x;
	prv_y_position = y;
	prv_data_points = 0;
	prv_bad_scan = FALSE;

}

/*--------------------------------------------
performs scan -- update
--------------------------------------------*/
void lidar_handler::perform_scan() {

#if LIDAR_TIMING
	auto start_time = clock();
#endif

	prv_bad_scan = false;

	//setting up the command
	const char* last_scan;
	string last_scan_string = "";
	last_scan_string += (char)2;
	last_scan_string += "sRN";
	last_scan_string += (char)32;
	last_scan_string += "LMDscandata";
	last_scan_string += (char)3;
	last_scan = last_scan_string.c_str();

	//setting local variables
	long SUCCESSFUL;
	WSADATA WinSockData;
	WORD DLLVersion;
	DLLVersion = MAKEWORD(2, 1);

	//starting up the socket stuff, shutfown if it fails
	if (WSAStartup(DLLVersion, &WinSockData) != 0) {
		prv_bad_scan = true;
		return;
	}

	//setting up variables for receiving data
	string RESPONSE;
	string CONVERTER;
	char MESSAGE[5000];
	memset(&MESSAGE, 0, sizeof(MESSAGE));

	//setting up address information
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr(LIDAR_IP_ADDR);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(LIDAR_PORT);

	//declaring the socket object
	SOCKET sock = socket(AF_INET, SOCK_STREAM, NULL);

	//Set a timeout
	int timeout = 80;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int));

	//attempt to connect to the lidar and if not successful, shutdown
	if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		prv_bad_scan = true;
		return;
	}

	//send the command for last scan
	send(sock, last_scan, (int)strlen(last_scan), 0);

	//receive the data to the message array and set flag
	SUCCESSFUL = recv(sock, MESSAGE, sizeof(MESSAGE), NULL);

	cout << MESSAGE << endl;


	//close the socket
	closesocket(sock);

	//turn the char array into a string, return bad scan if no message
	CONVERTER = MESSAGE;

	//report a bad scan if there is no data in the message
	if (CONVERTER.length() == 0) {
		prv_bad_scan = true;
		return;
	}

	//split the data by spaces into an array 
	istringstream iss(CONVERTER);
	vector<string> results((istream_iterator<string>(iss)),istream_iterator<string>());

	//check for correctness (we only want to read good, whole messages)
	if (results.size() > 25) {
		prv_data_points = convert_to_decimal(results[25]);
		cout << "data points: " << prv_data_points << endl;
		//checking end of scan
		if (results.size() < (prv_data_points + 27) /*|| convert_to_decimal(results[25 + prv_data_points + 2]) != 0*/) {
			cout << "error string: " << results[25 + prv_data_points + 2]  << endl;
			prv_bad_scan = true;
			return;
		}
	}
	else {
		prv_bad_scan = true;
		return;
	}
	//set raw hex data to the newest scan
	prv_raw_hex_data = results;

#if LIDAR_TIMING
	//calculate run time
	double duration = (std::clock() - start_time) / (double)CLOCKS_PER_SEC * 1000.0;

	//print run time
	std::cout << "Execution time: " << duration << " ms\n";
#endif

}

void lidar_handler::analyze_scan() {
	//dont input new data if it was a bad scan
	if (!prv_bad_scan) {
		//clear out old data
		prv_raw_data.clear();
		//set start angle to fill in data
		double start_angle = LIDAR_START_ANGLE;
		//iterate through all points
		for (int i = 0; i < prv_data_points; i++) {
			//calculate distance
			double distance = convert_to_decimal(prv_raw_hex_data[26 + i])/1000.0;
			//push distance with its angle into the vector
			prv_raw_data.push_back(make_tuple(start_angle, distance));

			//increment start angle
			start_angle += LIDAR_ANG_RES;
		}
		//new data is ready for the grid
		prv_raw_data_ready = true;
	}
	else {
		cout << "bad scan" << endl;
	}
}

//for hex conversion
int lidar_handler::convert_to_decimal(string num) {
	int total;
	stringstream ss;
	ss << hex << num;
	ss >> total;
	return total;
}


lidar_data_packet* lidar_handler::get_data() {
	prv_raw_data_ready = false;
	return (&prv_raw_data);
}

//referencing raw angle, distance data
bool lidar_handler::data_is_ready() {
	if (prv_raw_data_ready) {
		
		return true;
	}
	else {
		return false;
	}
}

double lidar_handler::get_x_loc() {
	return prv_x_position;
}

double lidar_handler::get_y_loc() {
	return prv_y_position;
}

int lidar_handler::get_num_data_points() {
	return prv_data_points;
}