#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <vector>
#include "nav_msgs/Odometry.h"
#include<math.h>

//define robot initial pose
double pose[2] = {0,0};

void odometryCallBack(const nav_msgs::Odometry::ConstPtr& msg){
	//ROS_INFO("x: %f, y: %f",msg->pose.pose.position.x,msg->pose.pose.position.y);	
	pose[0] = msg->pose.pose.position.x + 4;
	pose[1] = msg->pose.pose.position.y;
}



double calculateDistance (double target[2] ){
	double res =  sqrt(pow((target[0] - pose[0]), 2) + pow((target[1] - pose[1]), 2));
	//ROS_INFO("distance %f",res);	
	return res;  
}



int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
// Define subscriber
  ros::Subscriber odom_sub = n.subscribe("odom",1000,odometryCallBack);
 
// Define goal and dropoff positions
/*
std::vector<vector<double>> targets;
targets[0] = {-0.5,-0.5,1.0}; // pick up position
targets[1] = {0.5,0.5,1.0}; //  drop off position
*/
double pickUp[2] = {2.0,0.0};
double dropOff[2] = {4.0,0.0};
int state = 0;
  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;
	ROS_INFO("Approachin to pick up area!");
  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    //marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header


    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.1;
    marker.scale.y = 0.1;
    marker.scale.z = 0.1;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();



  ros::spinOnce();



if(state == 0){
	 marker.action = visualization_msgs::Marker::ADD;
      	  marker.pose.position.x = pickUp[0];
          marker.pose.position.y = pickUp[1];
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
          marker_pub.publish(marker);
          double distance = calculateDistance(pickUp);
          ROS_INFO("d: %f",distance);          
if (distance < 0.5){
            sleep(5);
	    ROS_INFO("Pick up position reached!");
            state++;	
          }

        }

else if(state == 1){
          ROS_INFO("Marker is being carried!");
          marker.action = visualization_msgs::Marker::DELETE;
          double distance = calculateDistance(dropOff);
          marker_pub.publish(marker);
          ROS_INFO("d: %f",distance);
          if (distance < 0.7){
	       ROS_INFO("Drop off position reached!");
               state++;	
          }
       }

        else if (state == 2) 
          {
           marker.action = visualization_msgs::Marker::ADD;
           marker.pose.position.x = dropOff[0];
           marker.pose.position.y = dropOff[1];
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
           marker_pub.publish(marker);
            ROS_INFO("Marker is dropped off at target!");

          }

     
	
	
			
    //marker_pub.publish(marker);

    r.sleep();
  }
}
