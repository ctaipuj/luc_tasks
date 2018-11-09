#ifndef TRACKING_UR3
#define TRACKING_UR3

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PoseStamped.h>
#include <string>
#include <sstream>


#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>

#include <robotiq_85_control/gripper_ur_control.h>


class Trackingrobot{

	private:
	   
	   ros::NodeHandle n;
	   ros::Publisher track;
	   ros::Rate rate(); 	   
           
           //--------------------------------

	   tf::TransformListener listener;
	   tf::StampedTransform transform_aruco;			
           
	   std_msgs::String msg;			

	   //--------------------------------
	   moveit_msgs::CollisionObject collision_object;
	   moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
	   
	   //-------------------------------- 
		
	   double x, y, X, Y, xant, Z, z, Rx, Ry, Rz;
	   double base, shoulder, elbow, wrist1, wrist2, wrist3;
	   int count;
	   bool statusz, statusy, statusx, status_global, status;
	   std::stringstream comando;
	   const int filas = 3;
           const int columnas = 6;
	   const int filas_aux = 1;	
         	
   
		

	public:
	   
	   Trackingrobot();
	   GripperUR g; //Objeto griper	
	   void  info();	
	   void Home();	//Posicion de home para tomar la camara
	   void TakeCamera(); // Inicializar el gripper cerrar gripper
	   void HomeTraking(); //Home del tracking
	   void PoseTarget(); //Posicion del objetivo
	   bool Restric(double a, double b, double c); //restriccion del tracking
	   void MoveRobot();	//mueve el robot a la posicion deseada
	   void publisher();
	   void Attach_camera();



};

#endif
