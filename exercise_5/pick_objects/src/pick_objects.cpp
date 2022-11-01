#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
   while(!ac.waitForServer(ros::Duration(5.0))){	
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  //we'll send a goal to the robot to move 1 meter forward
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = 2.0;
  goal.target_pose.pose.orientation.w = 0.001;

  ROS_INFO("Sending target");
  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, Target Succeeded");
  else
    ROS_INFO("Target fails for some reason");


  //wait for the action server to come up
   while(!ac.waitForServer(ros::Duration(5.0))){	
    ROS_INFO("Waiting for robot to reach target");
  }

  move_base_msgs::MoveBaseGoal dropOff;

  //we'll send a goal to the robot to move 1 meter forward
  dropOff.target_pose.header.frame_id = "map";
  dropOff.target_pose.header.stamp = ros::Time::now();

  dropOff.target_pose.pose.position.x = 4.0;
  dropOff.target_pose.pose.orientation.w = 0.001;

  ROS_INFO("Sending target");
  ac.sendGoal(dropOff);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, Target Succeeded");
  else
    ROS_INFO("Target fails for some reason");
  return 0;
}
