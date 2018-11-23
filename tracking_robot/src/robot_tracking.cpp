#include "../include/robot_tracking.h"



Trackingrobot::Trackingrobot(){

	track = n.advertise<std_msgs::String>("/ur_driver/URScript",1);
	sub = n.subscribe("sstatus", 1, &Trackingrobot::status_callback, this);
	ros::Duration(1).sleep();
	
	//---------------------------------------------------------------


}



void Trackingrobot::info(){

	ROS_INFO("ROBOT TRACKING UR3\n");

}

void Trackingrobot::Home(){

 
        double pos_home[4][columnas]= {{ 0.0,-1.57,0.0,-1.57, 0.0,0.0}, 
				       { 0.0286,-1.5566,1.4894,-1.4570,-1.5964,6.2779},
				       {-2.03,-1.94,2.11,-1.74,-1.61,2.66},
                 		       {-2.03,-1.86,2.32,-2.04,-1.61,2.66}}; //-2.05,-1.81,2.32,-2.05,-1.58,5.82
        
	g.init();	
	ros::Duration(5).sleep();
	g.open();
	ros::Duration(2).sleep();

	for(int i = 0; i < 4; i++){

		base     = pos_home[i][0];
		shoulder = pos_home[i][1]; 
		elbow    = pos_home[i][2];
		wrist1   = pos_home[i][3];
		wrist2   = pos_home[i][4];
		wrist3   = pos_home[i][5];

      		comando<<"movej(["<<base<<","<<shoulder<<","<<elbow<<","<<wrist1<<","<<wrist2<<","
	       	<<wrist3<<"],"<<"a=1.4"<<","<<"v=1.05)\n";
		
		publisher();
		ros::Duration(4).sleep();
		
	}

	TakeCamera();
	ros::Duration(2).sleep();
	HomeTraking();

}

void Trackingrobot::TakeCamera(){
	
	
	
	g.close();//cerrar gripper
	ros::Duration(1).sleep();
	Attach_camera();

	
}

void Trackingrobot::HomeTraking(){

	double pos_home_tracking[filas][columnas]={{-2.03,-1.94,2.11,-1.74,-1.61,2.66}, //-1.99,-1.85,2.17,-1.87,-1.63,5.82
											   {0.0,-1.57,0.0,-1.57, 0.0,0.0},
											   {-0.074, -1.597, 1.555, -3.124, -1.513, 3.1414}};
			                           
	for(int i = 0; i < 3; i++){

		base     = pos_home_tracking[i][0];
		shoulder = pos_home_tracking[i][1]; 
		elbow    = pos_home_tracking[i][2];
		wrist1   = pos_home_tracking[i][3];
		wrist2   = pos_home_tracking[i][4];
		wrist3   = pos_home_tracking[i][5];
		
	 	comando<<"movej(["<<base<<","<<shoulder<<","<<elbow<<","<<wrist1<<","<<wrist2<<","
		       <<wrist3<<"],"<<"a=1.4"<<","<<"v=1.05)\n";

		publisher();
		ros::Duration(4).sleep();

	}
		
}

void Trackingrobot::PoseTarget(){

	try{
		listener.lookupTransform("/base", "/target",ros::Time(0), transform_aruco);
	}
	catch (tf::TransformException ex){
		ROS_ERROR("%s", ex.what());
		ros::Duration(1.0).sleep();
	}
	
	x   = transform_aruco.getOrigin().x() + 0.20; //-0.425;	
	y   = transform_aruco.getOrigin().y();
	z   = transform_aruco.getOrigin().z();
	Rx  = 1.1455;//transform_aruco.getRotation().x();	
	Ry  = -1.1983; //-1.27
	Rz  = -1.2532;//transform_aruco.getRotation().z(); //-1.16;//transform_aruco.getRotation().z();

	
	
	if (!Restric(x, y , z)){
		MoveRobot();
	}else{
		ROS_WARN("Se a exedido los limites del Robot UR3");
	}

}

bool Trackingrobot::Restric( double a,  double b ,  double c){

	status_global = 0;
	
	if ((c >= 0.198) && (c <= 0.500)){
		statusz = 0;	
	}else{
		statusz = 1;
	}

	if ((b >= -0.337) && (b <= 0.280)){
		statusy = 0;	
	}else{
		statusy = 1;
	}

	if ((a >= -0.650 ) && (a <= -0.024 )){
		statusx = 0;
	}else{
		statusx = 1;
	}	

	if (((statusz == 1) || (statusy == 1)) || (statusx == 1)){
		if (statusz == 1){
			ROS_WARN("Se ha exedido el maximo/minimo en z");
			return status_global = 1;		
		}else if(statusy == 1){
			ROS_WARN("Se ha exedido el maximo/minimo en y");
			return status_global = 1;
		}else if(statusx == 1){
			ROS_WARN("Se ha exedido el maximo/minimo en x ");
			return status_global = 1;		
		}else{

		}
	}else{
		return status_global = 0;	
	}	

}

void Trackingrobot::MoveRobot(){

	
	comando<<"movel(p["<<x<<","<<y<<","<<z<<","<<Rx<<","<<Ry<<","<< Rz<<"],"<<"a=0.2"<<","<<"v=0.20)\n";		

	publisher();
	ros::Duration(1.0).sleep();	

}

void Trackingrobot::Attach_camera(){

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
	pose.position.y = -0.235;
	pose.position.z = 0.050;

	collision_object.primitives.push_back(primitive);
	collision_object.primitive_poses.push_back(pose);
	collision_object.operation = collision_object.ADD;

	std::vector<moveit_msgs::CollisionObject> collision_objects;
	collision_objects.push_back(collision_object);

	planning_scene_interface.applyCollisionObject(collision_object);

	ros::Duration(0.5).sleep();

	moveit::planning_interface::MoveGroupInterface move_group("manipulator");
	move_group.attachObject(collision_object.id);

	

}

void Trackingrobot::Back_home(){
	
	double pos_home_tracking[5][columnas]={{-2.03,-1.94,2.11,-1.74,-1.61,2.66},
					       {-2.03,-1.86,2.32,-2.04,-1.61,2.66},
					       {-2.03,-1.94,2.11,-1.74,-1.61,2.66},
					       {-0.02,-1.57,0.02,-1.57,-0.02,0.03},
					       { 0.0,-3.14,2.65,-4.22,0.0,4.71}};
			                           
	for(int i = 0; i < 5; i++){

		base     = pos_home_tracking[i][0];
		shoulder = pos_home_tracking[i][1]; 
		elbow    = pos_home_tracking[i][2];
		wrist1   = pos_home_tracking[i][3];
		wrist2   = pos_home_tracking[i][4];
		wrist3   = pos_home_tracking[i][5];
		
	 	comando<<"movej(["<<base<<","<<shoulder<<","<<elbow<<","<<wrist1<<","<<wrist2<<","
		       <<wrist3<<"],"<<"a=1.4"<<","<<"v=1.05)\n";

		publisher();
		ros::Duration(4).sleep();
		
		if (i == 1){
				g.open();
		}

	}
}

void Trackingrobot::status_callback(const tracking_robot::status_msg& msg)
{
	
	auxx_status = msg.A;

}

void Trackingrobot::publisher(){
	 msg.data = comando.str();
	 track.publish(msg);
	 comando.str("");
	 
}
