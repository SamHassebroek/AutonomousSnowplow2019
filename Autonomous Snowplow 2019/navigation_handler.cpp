#include "navigation_handler.h"

navigation_handler::navigation_handler(atomic<double> * orientation, atomic<double> * x_pos,
	atomic<double> * y_pos) {
	 
	prv_ori_ref = orientation;
	prv_x_ref   = x_pos;
	prv_y_ref   = y_pos;

#if( NAV_POINT_METHOD == MANUAL )
	/*---------------------------------------
	Set the first manual prv_target
	---------------------------------------*/
	cout << "Enter an x coordinate: ";
	cin >> prv_target.x;
	cout << "Enter a y coordinate: ";
	cin >> prv_target.y;

#elif( NAV_POINT_METHOD == LIST )
//need functionality to grab list and set first point
#endif

	/*---------------------------------------
	Find goal orientation
	---------------------------------------*/
	prv_goal_orientation = ( 180.0 * atan2( prv_target.y - *prv_y_ref, prv_target.x - *prv_x_ref ) ) / M_PI;
	prv_goal_orientation -= 90;
	if (prv_goal_orientation < 0) {
		prv_goal_orientation = 360 + prv_goal_orientation;
	}
	cout << "Goal orientation: " << prv_goal_orientation << endl;

}

drive_operation navigation_handler::update() {
	/*---------------------------------------------
	check to see if the plow is at the prv_target.
	get coords first so they stay the same for this
	series of calculations
	---------------------------------------------*/
	double cur_x = *prv_x_ref;
	double cur_y = *prv_y_ref;
	if( ( ( cur_x > ( prv_target.x - NAV_POINT_THRESH_M ) ) 
	 &&   ( cur_x < ( prv_target.x + NAV_POINT_THRESH_M ) ) )
	 && ( ( cur_y > ( prv_target.y - NAV_POINT_THRESH_M ) )
	 &&   ( cur_y < ( prv_target.y + NAV_POINT_THRESH_M ) ) ) ) {

#if( NAV_POINT_METHOD == MANUAL )
		/*---------------------------------------
		Get the next target
		---------------------------------------*/
		cout << "Reached target. Enter a new point" << endl;
		cout << "Enter an x coordinate: ";
		cin >> prv_target.x;
		cout << "Enter a y coordinate: ";
		cin >> prv_target.y;

#elif( NAV_POINT_METHOD == LIST )
		//need functionality to grab next target from list
		//if reached final point return stop for drive op

#endif
		prv_goal_orientation = ( 180.0 * atan2( prv_target.y - cur_y, prv_target.x - cur_x ) ) / M_PI;
		if (prv_goal_orientation < 0) {
			prv_goal_orientation = 360 + prv_goal_orientation;
		}
		prv_goal_orientation -= 90;

	}
	else {
		/*---------------------------------------------
		calculate orientation boundaries
		---------------------------------------------*/
		bool modified = false;
		double lower_bound = prv_goal_orientation - ORI_THRESH_D;
		if (lower_bound < 0) {
			lower_bound += 360.0;
			modified = true;
		}
		double higher_bound = prv_goal_orientation + ORI_THRESH_D;
		if (higher_bound > 360) {
			higher_bound -= 360;
			modified = true;
		}

		/*---------------------------------------------
		Check if orientation is within its tolerance.
		If not return drive command
		---------------------------------------------*/
		double cur_ori = *prv_ori_ref;
		if( modified && ( ( cur_ori > ( lower_bound ) )
	     || ( cur_ori < ( higher_bound ) ) ) ) {
			cout << "current orientation works." << endl;
		}
		else if (!modified && ( ( cur_ori > ( lower_bound ) )
	     && ( cur_ori < ( higher_bound ) ) ) ) {
			cout << "current orientation works." << endl;
		}
		else {
			/*---------------------------------------------
			Depending on whether the current orientation is
			closer to the lower_bound or higher_bound the
			drive operation will be chosen
			---------------------------------------------*/
			if( modified ) {
				if( abs( lower_bound - cur_ori ) > abs( higher_bound - cur_ori ) ) {
					cout << "Need to turn right" << endl;
					return RIGHT;
				}
				else {
					cout << "Need to turn left" << endl;
					return LEFT;
				}
			}
		}

	}

	/*
	first check if we are on the prv_target
	if we are get next prv_target or finish
	if not check orientation and distance
	set speed an drive operation based off of that
	*/
	return STOP;
}