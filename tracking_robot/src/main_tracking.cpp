#include "../include/robot_tracking.h"

int main(int argc, char*argv[]){




	ros::init(argc,argv, "tracking_robot");
	ros::NodeHandle n;
      

	Trackingrobot move_robot;

	//ros::Rate loop_rate(1);
	
	
	while(n.ok()){

		ros::spinOnce();


		if (move_robot.auxx_status == 1 & move_robot.init_ok == false){
		
			move_robot.Home();
			ROS_INFO("---------------start task----------------");
			move_robot.init_ok = 1;

		}
	
		if (move_robot.auxx_status == 2 & move_robot.init_ok == true){
		
			move_robot.status_tracking = 0;
			while(move_robot.auxx_status != 3){

				move_robot.PoseTarget();
				ROS_INFO("---------------start task tracking----------------");
				ros::spinOnce();
				
			}
	
			ROS_INFO("---------------stop task tracking----------------");
			move_robot.status_tracking = 1;
		}
	
		
		if (move_robot.auxx_status == 4 & move_robot.status_tracking == true ){
			
			ROS_INFO("---------------return home ----------------");
			move_robot.Back_home();
			move_robot.init_ok = 0;
			move_robot.status_tracking = 0; 	
		
		}

		
		ros::spinOnce();

	}

 		//loop_rate.sleep();
		return 0;
}

	


