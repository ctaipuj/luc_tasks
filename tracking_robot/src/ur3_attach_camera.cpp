#include <ros/ros.h>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>

int main(int argc, char** argv)
{

	ros::init(argc,argv, "attach_camera");
	ros::NodeHandle node_handle;
	ros::AsyncSpinner spinner(1);
	spinner.start();
	
	//---------------------Move group-----------
	

	//---------------------Attach camera-----------
	moveit_msgs::CollisionObject collision_object;
	moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
	moveit::planning_interface::MoveGroupInterface move_group("manipulator");
	collision_object.header.frame_id = "base_link";

	collision_object.id = "camera";
	
	shape_msgs::SolidPrimitive primitive;
	primitive.type = primitive.BOX;
	primitive.dimensions.resize(3);
	primitive.dimensions[0] = 0.05;
  	primitive.dimensions[1] = 0.044;
  	primitive.dimensions[2] = 0.02;

	geometry_msgs::Pose pose;
	pose.orientation.w = 1.0;
	pose.position.x = 0.0;
	pose.position.y = -0.250;
	pose.position.z = 0.0;

	collision_object.primitives.push_back(primitive);
	collision_object.primitive_poses.push_back(pose);
	collision_object.operation = collision_object.ADD;

	std::vector<moveit_msgs::CollisionObject> collision_objects;
	collision_objects.push_back(collision_object);

	planning_scene_interface.applyCollisionObject(collision_object);
	move_group.attachObject(collision_object.id);	
	ros::shutdown();
	return 0;

}
