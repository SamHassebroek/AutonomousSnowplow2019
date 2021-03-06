#include "orientation_handler.h"

/*---------------------------------------
constructor for orientation interface.
This takes a reference to an atomic
double so its easy to modify the 
---------------------------------------*/
orientation_handler::orientation_handler(atomic<double> * orientation) {
	prv_ori_ref = orientation;
}

/*---------------------------------------
main orientation execution loop
---------------------------------------*/
void orientation_handler::run() {

	string buildingString = "";
	string mostRecent = "";
	string anotherFuckingString = "";
	const char *portName = IMU_COM_PORT;
	Serial* arduino = new Serial(portName);
	char incomingData[512] = "";
	int dataLength = 255;
	int readResult = 0;
	bool firstOpen = false;

	/*--------------------------------------------------
	this works, but should probably be cleaned up
	--------------------------------------------------*/
	while (true)
	{ 
		readResult = arduino->ReadData(incomingData, dataLength);
		incomingData[readResult] = 0;
		if (firstOpen)
		{
			if (((string)incomingData).find(']') != -1)
			{
				buildingString += incomingData;
				mostRecent = buildingString;
				buildingString = "";
				firstOpen = false;
				string mysubstring = mostRecent.substr(mostRecent.find('[') + 1, mostRecent.find(']') - 1);
				anotherFuckingString = mysubstring;
				double ori = stod(anotherFuckingString);
				if (ori == 360.0) {
					ori = 0.0;
				}
				*prv_ori_ref = ori;
				continue;
			}
		}
		else
		{
			if (((string)incomingData).find('[') != -1)
			{
				firstOpen = true;
			}
		}

		buildingString += incomingData;
	}
}
