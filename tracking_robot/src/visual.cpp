#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>

void singlePoseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
	ROS_INFO("Publicandonnnnnn ");
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "listener");
	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("aruco_single/pose", 100, singlePoseCallback);
	
	ros::spin();




	return 0;

}
