#include "ros/ros.h"
#include  "tracking_robot/status_msg.h"
#include <sstream>


int main (int argc, char **argv)
{

  ros::init(argc, argv, "status");
  ros::NodeHandle n;

  ros::Publisher status_pub = n.advertise<tracking_robot::status_msg>("sstatus",1000);

  ros::Rate loop_rate(10);

  int data_1;

  while (ros::ok())
  {
  	tracking_robot::status_msg msg;

     	
	ROS_INFO("ENTER -----------------> 1-------> TO TAKE CAMERA FOR TASK TRACKING");
	ROS_INFO("ENTER -----------------> 2-------> TO START TASK TRACKING");
	ROS_INFO("ENTER -----------------> 3-------> TO STOP  TRACKING");
	ROS_INFO("ENTER -----------------> 4-------> TO STOP  RETURN HOME TRACKING");
	std::cin>>data_1;
	msg.A= data_1;

	status_pub.publish(msg);
	
        ros::spinOnce();
        loop_rate.sleep();

  }



  return 0;


}





