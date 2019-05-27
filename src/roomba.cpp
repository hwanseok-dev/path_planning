#include "Roomba.h"

/**
* Settings for subscribe model state
*/
Roomba::Roomba(){
	running_index = -1;
	roomba_state_subscriber = state_nh.subscribe("/gazebo/model_states", 1000, &ROOMBA::simStateCallback, this);
	roomba_state_subscriber = state_nh.subscribe("/tf", 1000, &ROOMBA::roombaStateCallback, this);
	//Set 8 roomba's initial positions as (0.0, 0.0)
	for(int i=0; i<HOW_MANY_ROOMBAS; i++){                    
		curr.push_back(make_pair(0.0,0.0));
		dest.push_back(make_pair(0.0,0.0));
		yaw.push_back(0.0);
	}
}


int Roomba::checkOrientation(int haveToMoveIndex){
	//Check current orientation
	/*
	return value
			1      //Looking Up : return 1;
		   /|
		  / |
		3 --+-- 4
			|
			2	   //else : return -1;
	*/
	double yaw = roomba.GetYaw(haveToMoveIndex);
	if (-0.5 <= yaw && yaw <= 0.5 ){
		//Looking Down
		return 2;
	}else if (1.0 <= yaw && yaw <= 2.0 ){
		//Looking Right
		return 4;
	}else if (-2.0 <= yaw && yaw  <= -1.0 ){
		//Looking Left
		return 3;
	}else if (-2.5 >= yaw || yaw >= 2.5 ){
		//Looking Up
		return 1;
	}else{
		ROS_INFO("Really Bad orientation. %f", yaw);
		return -1;
	}
}


/**
* Get current position of models from /gazebo/model_states messeges in REAL WORLD.
* From x,y,z,w calculate yaw.
*/
void Roomba::realStateCallback(const tf2_msgs::TFMessage::ConstPtr& msg){
	//Left wheel : msg[0].transforms.transform.rotation.x
	//Right wheel : msg[1].transforms.transform.rotation.x
	geometry_msgs::Quaternion rot = msg->transforms[0].transform.rotation;
	tf2::Quaternion q(rot.x, rot.y, rot.z, rot.w);
	tf2::Matrix3x3 m(q);
	double roll, pitch, yaw;
  	m.getRPY(roll, pitch, yaw);
  	ROS_INFO("Current yaw is %f\n", yaw);
}

/**
* Without Gazebo simulator, this method will not be run.
* Get current position of models from /gazebo/model_states messeges in GAZEBO SIMULATOR.
* From x,y,z,w calculate yaw.
*/
void Roomba::simStateCallback(const gazebo_msgs::ModelStates::ConstPtr& msg)
{	
	for(int i=1; i<9; i++){
		//ROS_INFO(msg->name[i].c_str());
		if(msg->name[i] == "create1"){
			SetCurrX(0, msg->pose[i].position.x);
			SetCurrX(0, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			SetYaw(0, yaw);
			//ROS_INFO("yaw : %f", yaw);
		}else if(msg->name[i] == "create2"){
			SetCurrX(1, msg->pose[i].position.x);
			SetCurrX(1, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			SetYaw(1, yaw);
		}else if(msg->name[i] == "create3"){
			SetCurrX(2, msg->pose[i].position.x);
			SetCurrX(2, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			SetYaw(2, yaw);
		}else if(msg->name[i] == "create4"){
			SetCurrX(3, msg->pose[i].position.x);
			SetCurrX(3, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			SetYaw(3, yaw);
		}else if(msg->name[i] == "create5"){
			SetCurrX(4, msg->pose[i].position.x);
			SetCurrX(4, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			SetYaw(4, yaw);
		}else if(msg->name[i] == "create6"){
			SetCurrX(5, msg->pose[i].position.x);
			SetCurrX(5, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			SetYaw(5, yaw);
		}else if(msg->name[i] == "create7"){
			SetCurrX(6, msg->pose[i].position.x);
			SetCurrX(6, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			SetYaw(6, yaw);
		}else if(msg->name[i] == "create8"){
			SetCurrX(7, msg->pose[i].position.x);
			SetCurrX(7, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			SetYaw(7, yaw);
		}else{
			ROS_INFO("i is %d", i);
			ROS_INFO("Sth wrong!");
		}
	}
	//ROS_INFO("stateCallback done!");
}


void Roomba::setRunningIndex(int index){
	running_index = index;
}
void Roomba::setCurrX(int index, double x){
	curr[index].first= x;
}
void Roomba::setCurrY(int index, double x){
	curr[index].second= x;
}
void Roomba::setDestX(int index, double x){
	dest[index].first= x;
}
void Roomba::setDestY(int index, double x){
	dest[index].second= x;
}
void Roomba::setYaw(int index, double x){
	yaw[index] = x;
}
double Roomba::getCurrX(int index){
	return curr[index].first;
}
double Roomba::getCurrY(int index){
	return curr[index].second;
}
double Roomba::getDestX(int index){
	return dest[index].first;
}
double Roomba::getDestY(int index){
	return dest[index].second;
}
double Roomba::getYaw(int index){
	return yaw[index];
}
Roomba::~Roomba(){
	//empty
}