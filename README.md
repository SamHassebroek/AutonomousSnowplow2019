# Iowa State University Robotics Autonomous Snowplow Software Repository
This repository contains code for the 2019 Autonomous Snowplow competition.
# Main Loop Execution Steps
This is assuming that all variables and interfaces (or objects such as the LiIDAR or Decawave) have been created.
1. Perform a LiDAR scan. The LiDAR is always scanning so this will grab the data for the most recent scan which we can safely assume was a very short time ago (< 67 ms for the TIM551 and < 15 ms for the LMS511) given the rate at which they perform scans. If the scan was unsuccessful then start the loop over again. 
2. Analyze the scan. This takes the raw hex data that we received from the LiDAR for the most recent scan and turns it into distance and angle pairs. 
3. Map the angle and distance pairs to the lidar hit map which is a 2D array of the field. The size of this array depends on three factors: the hight, width, and resolution defined in snowplow_type.h as of writing this. Mapping should also take into account the current location and orientation of the LiDAR so that hits are correctly mapped.
4. As of now the map is printed every 50 iterations of the loop.
