#include <moveit/move_group_interface/move_group_interface.h> //to move lenny
#include <moveit/planning_scene_interface/planning_scene_interface.h> //to move lenny

#include <tf/transform_listener.h>



int main(int argc, char** argv)
{
  ros::init(argc, argv, "move_robot_goal"); //dummy node starts
  ros::NodeHandle node_handle;
  ros::AsyncSpinner spinner(1);
  spinner.start();

  
  static const std::string PLANNING_GROUP = "manipulator";

  moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);

  moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

  const robot_state::JointModelGroup* joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
      
  move_group.setPlanningTime(10);
  move_group.setNumPlanningAttempts(8);
  move_group.setGoalPositionTolerance(0.08);
  move_group.setGoalOrientationTolerance(0.02);
  move_group.setMaxVelocityScalingFactor(0.2); 
  
  bool success;

  geometry_msgs::Pose target_pose; 

  tf::TransformListener listener;

  ros::Rate rate(2);

  while (node_handle.ok())
  {
	  tf::StampedTransform transform_aruco;	


	  try{
		listener.lookupTransform("/base_link", "/Herramienta",ros::Time(0), transform_aruco);
	  }
          catch (tf::TransformException ex){
		ROS_ERROR("%s", ex.what());
		ros::Duration(1.0).sleep();
	  }			


	  target_pose.orientation.w = transform_aruco.getRotation().w();
	  target_pose.orientation.x = transform_aruco.getRotation().x();//0.707;
	  target_pose.orientation.y = transform_aruco.getRotation().y();//0.707;
	  target_pose.orientation.z = transform_aruco.getRotation().z();
	  
	  target_pose.position.x = transform_aruco.getOrigin().x();
	  target_pose.position.y = transform_aruco.getOrigin().y();
	  target_pose.position.z = transform_aruco.getOrigin().z()-0.040;
	  
	  move_group.setPoseTarget(target_pose,"ee_link");
	  
	  moveit::planning_interface::MoveGroupInterface::Plan my_plan;

	  success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

	  ROS_INFO("Moving to planned goal pose %s", success ? "" : "FAILED");
	  
	  move_group.move();

	 joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
	
         ros::Duration(2).sleep();
  }
      
  ros::shutdown();
  return 0;
}
