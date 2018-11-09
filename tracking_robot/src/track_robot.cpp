#include <ros/ros.h>
#include <std_msgs/String.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PoseStamped.h>
#include <string>
#include <sstream>


void singlePoseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
	ROS_INFO("Publicandonnnnnn ");
}


int main(int argc , char **argv)
{
	double x, y, X, Y, xant, Z; 
	double z, Rx; 
	double Ry, Rz;
	int count;
	bool statusz, statusy, statusx;
	std::stringstream comando_mvl;	
	count = 0;


	ros::init(argc, argv, "tracking");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("aruco_single/pose", 100, singlePoseCallback);
	ros::Publisher track = n.advertise<std_msgs::String>("/ur_driver/URScript",1);///ur_driver/URScript
	
	
	tf::TransformListener listener;

	std_msgs::String msg;
	tf::StampedTransform transform_aruco;	

	ros::Rate rate(0.5);
	
	while(n.ok())
	{

		try{
			listener.lookupTransform("/base", "/target",ros::Time(0), transform_aruco);
		}
		catch (tf::TransformException ex){
			ROS_ERROR("%s", ex.what());
			ros::Duration(1.0).sleep();
		}
	
		x   = transform_aruco.getOrigin().x() + 0.20; //-0.425; -0.46195;	
	        y   = transform_aruco.getOrigin().y();
		z   = transform_aruco.getOrigin().z();
		Rx  = 1.1455;//transform_aruco.getRotation().x();	
		Ry  = -1.1983; //-1.27
		Rz  = -1.2532;//transform_aruco.getRotation().z(); //-1.16;//transform_aruco.getRotation().z();
		
  

		if ((z >= 0.198) && (z <= 0.500)){
 			statusz = 0;	
		}else{
			statusz = 1;
		}


		if ((y >= -0.337) && (y <= 0.280)){
 			statusy = 0;	
		}else{
			statusy = 1;
		}
		
		if ((x >= -0.650 ) && (x <= -0.024 )){
			statusx = 0;
		}else{
			statusx = 1;
		}


		/*ROS_INFO_STREAM("El valor de x : " << x);
		ROS_INFO_STREAM("El valor de y : " << y);
		ROS_INFO_STREAM("El valor de z : " << z);*/



		if (((statusz == 1) || (statusy == 1)) || (statusx == 1)){
			if (statusz == 1){
				ROS_INFO("Se ha exedido el maximo/minimo en z");
			}else if(statusy == 1){
				ROS_INFO("Se ha exedido el maximo/minimo en y");
			}else if(statusx == 1){
				ROS_INFO("Se ha exedido el maximo/minimo en x ");		
			}else{

			}
		}else{
			comando_mvl<<"movel(p["<<x<<","<<y<<","<<z<<","<<Rx<<","<<Ry<<","<< Rz<<"],"<<"a=0.2"<<","<<"v=0.20)\n";
		        msg.data = comando_mvl.str();
	  		//ROS_INFO("Este es el mensage: [%s\n]", msg.data.c_str());		
			track.publish(msg);
			comando_mvl.str("");
		}

		ros::Duration(1).sleep();
		

  
 
	}
       
       
       return 0;	

}
