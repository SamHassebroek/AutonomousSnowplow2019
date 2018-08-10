#pragma once

#include "SerialPort.h"
#include "snowplow_type.h"

class motor_interface {

public:
									motor_interface( drive_data_pkt * drive_pkt );

private:
	drive_data_pkt *				prv_drive_pkt_ref;
	Serial *                        prv_serial_connection;

public:
	void							send_pkt_to_motors();

};