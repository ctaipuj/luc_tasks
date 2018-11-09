/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Sachin Chitta, Dave Coleman, Mike Lautman */

//Modified by Wilson Hernández---->this is a very dummy implementation to move the robot to four positions using the joints for ROBOT UR3 

/*****************************************************
	THIS NODES REQUIRES ROSSERVICE robot_enable BE CALLED
	IS NOT RECOMMENDED CALLING THE SERVICE INSIDE A NODE. ROBOT COULD MOVE UNEXPECTEDLY!
	CAUTION WHEN CALLING robot_enable SERVICE MAKE SURE THE EMERGENCY STOP BUTTON IS REACHABLE BY OPERATOR
****************************************************************/  

#include <moveit/move_group_interface/move_group_interface.h> //to move lenny
#include <moveit/planning_scene_interface/planning_scene_interface.h> //to move lenny

int main(int argc, char** argv)
{
  ros::init(argc, argv, "move_robot"); //dummy node starts
  ros::NodeHandle node_handle;
  ros::AsyncSpinner spinner(1);
  spinner.start();

  /* MoveIt! operates on sets of joints called "planning groups" and stores them in an object called
   the `JointModelGroup`. Throughout MoveIt! the terms "planning group" and "joint model group"
   are used interchangably.*/
  static const std::string PLANNING_GROUP = "manipulator"; //the group you want to move

  /*The :move_group_interface:`MoveGroup` class can be easily
  setup using just the name of the planning group you would like to control and plan for.*/
  moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);

  // We will use the :planning_scene_interface:`PlanningSceneInterface`
  // class to add and remove collision objects in our "virtual world" scene
  moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

  // Raw pointers are frequently used to refer to the planning group for improved performance.
  const robot_state::JointModelGroup* joint_model_group =
      move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP); //Update the current joint state
      
  move_group.setPlanningTime(20.0); //set allowed planning time
  move_group.setNumPlanningAttempts(8); //planning attempts
  //////////////////////
  move_group.setMaxVelocityScalingFactor(0.1); //TODO change speed IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  /////////////////////
  
  std::vector<double> move_group_joint_goal(6); //this vector stores the different joint positions according to the planning group order is important here
  
  bool success; //flag

  ROS_INFO("Reference frame: %s", move_group.getPlanningFrame().c_str());

  // We can also print the name of the end-effector link for this group.
  //ROS_INFO("End effector link: %s", move_group.getEndEffectorLink().c_str());
  
  ros::Rate rate(10.0);
  while (node_handle.ok()){ //while ok; this node will execute a routine 
   
  move_group_joint_goal[0]= 0; //radians. You can fill the vector your preferred method.
  move_group_joint_goal[1]= 0;
  move_group_joint_goal[2]= 0;
  move_group_joint_goal[3]= 0;
  move_group_joint_goal[4]= 0;
  move_group_joint_goal[5]= 0.5;
 
  
  move_group.setJointValueTarget(move_group_joint_goal); //update target

  // Now, we call the planner to compute the plan and visualize it.
  // Note that we are just planning, not asking move_group
  // to actually move the robot.
  moveit::planning_interface::MoveGroupInterface::Plan my_plan; //create a plan

  success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS); //plan trajectory

  ROS_INFO("Trajectory planning 1 %s", success ? "SUCCEED" : "FAILED");
  if(success)
  move_group.move(); //MOVES THE ROBOT 
  
 }
    
  ros::shutdown(); //OFF
  return 0;
}
