#include "lidar_handler.h"

lidar_handler::lidar_handler(double x_pos, double y_pos) {
	cout << "*** If connecting to the LiDAR takes a few seconds there may be an error... ***" << endl;
	prv_x_position = x_pos;
	prv_y_position = y_pos;
	prv_orientation = 0.0;
}

/*--------------------------------------------
Connects to LIDAR, gets data from last scan,
makes sure it's an error free message and
makes it readable for other functions
--------------------------------------------*/
bool lidar_handler::perform_scan() {

#if LIDAR_TIMING
	auto start_time = clock();
#endif

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
		cout << "Socket startup failed" << endl;
		return false;
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

	//Set a timeout, not that important as long as it doesnt cut off the lidar
	int timeout = 80;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int));

	//attempt to connect to the lidar and if not successful, shutdown
	if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		cout << "Could not connect to LIDAR" << endl;
		return false;
	}

	//send the command for last scan
	send(sock, last_scan, (int)strlen(last_scan), 0);

	//receive the data to the message array and set flag
	SUCCESSFUL = recv(sock, MESSAGE, sizeof(MESSAGE), NULL);

	//close the socket
	closesocket(sock);

	//turn the char array into a string, return bad scan if no message
	CONVERTER = MESSAGE;

	//report a bad scan if there is no data in the message
	if (CONVERTER.length() == 0) {
		cout << "No data in message received from LIDAR" << endl;
		return false;
	}

	//split the data by spaces into an array 
	istringstream iss(CONVERTER);
	vector<string> results((istream_iterator<string>(iss)),istream_iterator<string>());

	/*------------------------------------------------
	Check the data returned by the LIDAR to make
	sure it's not just part of a message, it needs
	to be a whole message. To check  completeness,
	based off of the LIDAR in use, a certain value
	should always be present at the end of the string
	------------------------------------------------*/
	if ( results.size() < 25 ||
		 convert_to_decimal(results[25]) != LIDAR_DATA_POINTS ||
		 results[25 + LIDAR_DATA_POINTS + 2] != LIDAR_MSG_VLD)
	{
		cout << "LIDAR message invalid" << endl;
		return false;
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
	//clear out old data
	prv_raw_data.clear();
	//set start angle to fill in data
	double start_angle = LIDAR_START_ANGLE;
	//iterate through all points
	for (int i = 0; i < LIDAR_DATA_POINTS; i++) {
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