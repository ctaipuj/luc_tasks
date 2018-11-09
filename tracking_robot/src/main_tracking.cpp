#include "../include/robot_tracking.h"

int main(int argc, char*argv[]){

	ros::init(argc,argv, "tracking_robot");
	ros::NodeHandle n;

	Trackingrobot move_robot;
		
	move_robot.Home();

        
	while(n.ok()){
		
		move_robot.PoseTarget();

	}

	return 0;

}
