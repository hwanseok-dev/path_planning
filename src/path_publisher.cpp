#include <iostream>
#include "ros/ros.h"
#include <time.h>
#include <stdlib.h> 
// ROS 기본 헤더파일
#include <geometry_msgs/Twist.h>
#include "std_msgs/String.h"
#include "niklasjang_path_planning/MsgTutorial.h"
//===================================
//Spliter header
using namespace std;
#include <string>
#include <vector>

//=========state check
 #include "gazebo_msgs/ModelStates.h"

class ROOMBA{
private:
	vector< pair<double, double> > curr; //current position
	vector< pair<double, double> > dest; //destination position
	vector< double > yaw;                //current yaw
	ros::Subscriber model_state_sub;
	ros::NodeHandle nh2;
public:
	bool state_sub = false;              //if pddl result is loaded, this variable become true.
	ROOMBA(){
		model_state_sub = nh2.subscribe("/gazebo/model_states", 1000, &ROOMBA::stateCallback, this);
		state_sub = false;
		//Set 8 roomba's initial positions as (0.0, 0.0)
		for(int i=0; i<8; i++){                    
			curr.push_back(make_pair(0.0,0.0));
			dest.push_back(make_pair(0.0,0.0));
			yaw.push_back(0.0);
		}
	}
	void SetCurrX(int index, double x){
		curr[index].first= x;
	}
	void SetCurrY(int index, double x){
		curr[index].second= x;
	}
	void SetDestX(int index, double x){
		dest[index].first= x;
	}
	void SetDestY(int index, double x){
		dest[index].second= x;
	}
	void SetYaw(int index, double x){
		yaw[index] = x;
	}
	double GetCurrX(int index){
		return curr[index].first;
	}
	double GetCurrY(int index){
		return curr[index].second;
	}
	double GetDestX(int index){
		return dest[index].first;
	}
	double GetDestY(int index){
		return dest[index].second;
	}
	double GetYaw(int index){
		return yaw[index];
	}
	void stateCallback(const gazebo_msgs::ModelStates::ConstPtr& msg);
	friend class RONTROLLER;
};

void ROOMBA::stateCallback(const gazebo_msgs::ModelStates::ConstPtr& msg)
{	
	//ROS_INFO("Get current position of models from /gazebo/model_states messeges");

	for(int i=1; i<9; i++){
		//ROS_INFO(msg->name[i].c_str());
		if(msg->name[i] == "create1"){
			SetCurrX(0, msg->pose[i].position.x);
			SetCurrX(0, msg->pose[i].position.y);
		}else if(msg->name[i] == "create2"){
			SetCurrX(1, msg->pose[i].position.x);
			SetCurrX(1, msg->pose[i].position.y);
		}else if(msg->name[i] == "create3"){
			SetCurrX(2, msg->pose[i].position.x);
			SetCurrX(2, msg->pose[i].position.y);
		}else if(msg->name[i] == "create4"){
			SetCurrX(3, msg->pose[i].position.x);
			SetCurrX(3, msg->pose[i].position.y);
		}else if(msg->name[i] == "create5"){
			SetCurrX(4, msg->pose[i].position.x);
			SetCurrX(4, msg->pose[i].position.y);
		}else if(msg->name[i] == "create6"){
			SetCurrX(5, msg->pose[i].position.x);
			SetCurrX(5, msg->pose[i].position.y);
		}else if(msg->name[i] == "create7"){
			SetCurrX(6, msg->pose[i].position.x);
			SetCurrX(6, msg->pose[i].position.y);
		}else if(msg->name[i] == "create8"){
			SetCurrX(7, msg->pose[i].position.x);
			SetCurrX(7, msg->pose[i].position.y);
		}else{
			ROS_INFO("i is %d", i);
			ROS_INFO("Sth wrong!");
		}
	}
	//ROS_INFO("stateCallback done!");
	state_sub = true; //Get current position of 8 roombas
}


class RONTROLLER{

private :
	geometry_msgs::Twist twist;
	ros::NodeHandle nh;            // ROS 시스템과 통신을 위한 노드 핸들 선언
	ros::Publisher path_pub;      // = nh.advertise<niklasjang_path_planning::MsgTutorial>("/path1/cmd_vel", 1000);
	ros::Subscriber pddl_sub;
	vector <string> instructionList;          //instruction string vector??
	vector <pair<string, pair<string, string> > > next; //<roomba_number, x_pos, y_pos>
	string pddl_result;           //Get long pddl result from server
	ROOMBA roomba;
public:
	void Initialize(void);
	void SetMsg(double _x, double _z){
		twist.linear.x = _x;
		twist.angular.z = _z;
	}
	void RollRoll(double spen);
	void msgCallback(const std_msgs::StringConstPtr& pddl_result);
	void stateCallback(const gazebo_msgs::ModelStates::ConstPtr& msg);
	void GoStraight(void){
		SetMsg(0.7, 0.0);
		RollRoll(5);
		Stop();
	}

	void TurnLeft(void){
		SetMsg(0.0, -2.2);
		RollRoll(1.5);
		Stop();
	}

	void TurnRight(void){
		SetMsg(0.0, +2.2);
		RollRoll(1.5);
		Stop();
	}

	void Stop(void){
		SetMsg(0.0, 0.0);
		RollRoll(0.2);
	}
	void SplitByDelimiter(vector<string> &arr, string& str, const string& delim);
	void SplitNextInstruction(vector<pair<string, pair<string, string> > > &arr, string& str, const string& delim);
	void SplitPDDLResult(void);
	vector <pair<string, pair<string, string> > >  GetNext(void){
		return next;
	}
	void control(void);
	void simulation(void);
	friend class ROOMBA;
};

void RONTROLLER::SplitByDelimiter(vector<string> &arr, string& str, const string& delim) {
	//ROS_INFO("SPLIT INST");
	size_t pos = 0;		
	string token;
	while ((pos = str.find(delim)) != string::npos) { //delim ���ڸ� ã������
		token = str.substr(0, pos);  //ó������ pos ���̸�ŭ�� ���ڸ� token����
		//ROS_INFO(token.c_str());
		str.erase(0, pos + delim.length());
		arr.push_back(token);
	}
	//ROS_INFO(token.c_str());
	arr.push_back(str);
}

void RONTROLLER::SplitNextInstruction(vector <pair<string, pair<string, string> > > &arr, string& str, const string& delim) {
	//ROS_INFO("SPLIT NEXT");
	size_t pos = 0;
	string token;
	int index = 0;
	string obj;
	string x_pos;
	string y_pos;
	while ((pos = str.find(delim)) != string::npos) { 
		token = str.substr(0, pos);  
		//ROS_INFO(token.c_str());
		//ROS_INFO("index is %d", index);
		if (index == 0) { obj = token; }
		if (index == 1) { x_pos = token;}
		if (index == 2) {
			y_pos = token;
			//ROS_INFO(obj.c_str(), x_pos.c_str(), y_pos.c_str());
			next.push_back(make_pair(obj, make_pair(x_pos, y_pos)));
		}
		str.erase(0, pos + delim.length());
		index++;
	}
	
}

void RONTROLLER::msgCallback(const std_msgs::StringConstPtr& _pddl_result)
{	
	//ROS_INFO("MSGCALLBACK");
	pddl_result = _pddl_result->data;
	ROS_INFO("Subscribe data : %s", pddl_result.c_str());
	SplitPDDLResult();
}



void RONTROLLER::Initialize(void){
	//REMOVED : RONTROLLER rontroller;
	path_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	// 루프 주기를 설정한다. "10" 이라는 것은 10Hz를 말하는 것으로 0.1초 간격으로 반복된다
	pddl_sub = nh.subscribe("/result", 1000, &RONTROLLER::msgCallback, this);
}

void RONTROLLER::RollRoll(double spen){
	ros::Time beginTime = ros::Time::now();
	double begin = beginTime.toSec();
	ros::Duration secondsIWantToSendMessagesFor = ros::Duration(spen); 
	double end = secondsIWantToSendMessagesFor.toSec() + begin;
	double current = 0.0;
	while(current = ros::Time::now().toSec() < end )
	{
		current = ros::Time::now().toSec();
		//ROS_INFO("x, z : (%f, %f) \n", twist.linear.x, twist.angular.z);
		//ROS_INFO("current : %f \n", current);
		//ROS_INFO("end : %f \n", end);
		path_pub.publish(twist);

		// Time between messages, so you don't blast out an thousands of 
		// messages in your 3 secondperiod
		ros::Duration(0.1).sleep();
	}
}

void RONTROLLER::SplitPDDLResult(void){
	//ROS_INFO("SPILT");
	int i = 0;
	//string pddl_result = "[ t7,x2,y2,move-left t6,x3,y2,move-down t3,x3,y1,move-right t7,x2,y1,move-up ]";
	string delimiter = " ";
	
	SplitByDelimiter(instructionList, pddl_result, delimiter);
	
	instructionList.pop_back(); 
	delimiter = ",";
	for (int i = 1; i < instructionList.size(); i++) { 
		//ROS_INFO("string is %s", ins[i].c_str());
		//cout << ins[i] << "\n";
		SplitNextInstruction(next, instructionList[i] , delimiter);
	}
	//ROS_INFO("next size is %d", next.size());
	//cout << next.size() << "\n";
	//for (int i = 0; i < next.size(); i++) {
	//	ROS_INFO("result 1:",  next[i].first.c_str(),  next[i].second.first.c_str(), next[i].second.second.c_str());
	//	//cout << "(" << next[i].first <<"," << next[i].second.first << "," << next[i].second.second << ")\n";
	//}
	//cout << next[0].second.first << "\n";
}

//Determin what roomba will subscribe topic
void RONTROLLER::control(void){
	//ROS_INFO("CONTROL");
	path_pub = nh.advertise<geometry_msgs::Twist>("/robot2/cmd_vel", 1000);

	/*
	for (int i = 0; i < next.size(); i++) {
		//ROS_INFO(next[i].first.c_str());
		//ROS_INFO(next[i].second.first.c_str());
		//ROS_INFO(next[i].second.second.c_str());
		if (next[i].first == "t1"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot1/cmd_vel", 1000);
		}else if (next[i].first == "t2"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot2/cmd_vel", 1000);
		}else if (next[i].first == "t3"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot3/cmd_vel", 1000);
		}else if (next[i].first == "t4"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot4/cmd_vel", 1000);
		}else if (next[i].first == "t5"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot5/cmd_vel", 1000);
		}else if (next[i].first == "t6"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot6/cmd_vel", 1000);
		}else if (next[i].first == "t7"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot7/cmd_vel", 1000);
		}else if (next[i].first == "t8"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot8/cmd_vel", 1000);
		}else{
			ROS_INFO("Sth is wrong!");
		} 

		//Currn model state
		for(int i=1; i<9 ; i++){
			ROS_INFO("i th: %d", i);
			ROS_INFO("curr x is %f",roomba.GetCurrX(i));
		}
		//Make instruction

		//Test
	}
	ROS_INFO("control done");*/
}



void RONTROLLER::simulation(void){
	path_pub = nh.advertise<geometry_msgs::Twist>("/robot2/cmd_vel", 1000);
	ros::Duration(2.0).sleep();
	GoStraight();
	
	path_pub = nh.advertise<geometry_msgs::Twist>("/robot3/cmd_vel", 1000);
	TurnRight();
	ros::Duration(2.0).sleep();
	GoStraight();

	path_pub = nh.advertise<geometry_msgs::Twist>("/robot5/cmd_vel", 1000);
	TurnRight();
	ros::Duration(2.0).sleep();
	TurnRight();
	ros::Duration(2.0).sleep();
	GoStraight();

	path_pub = nh.advertise<geometry_msgs::Twist>("/robot8/cmd_vel", 1000);
	TurnRight();
	ros::Duration(2.0).sleep();
	TurnRight();
	ros::Duration(2.0).sleep();
	GoStraight();

	path_pub = nh.advertise<geometry_msgs::Twist>("/robot7/cmd_vel", 1000);
	TurnLeft();
	ros::Duration(2.0).sleep();
	GoStraight();
}


int main(int argc, char **argv)// 노드 메인 함수
{	
	ros::init(argc, argv, "path_publisher"); // 노드명 초기화
	
	RONTROLLER rontroller;
	rontroller.Initialize(); //Subscribe pddl_result from server and then split to next instruction vector
	ros::Rate loop_rate(10); // subscriber는 메시지가 오는 즉시 callback을 요청하고 바로 그 다음 메시지를 기다립니다. 
	
	while (ros::ok())
	{	
		ROS_INFO("Waiting for PPDL result");
		if(rontroller.GetNext().size() != 0){
			rontroller.simulation();
			return 0;
  		}
	  	ros::spinOnce();  
	  	loop_rate.sleep();
	}
	ros::spin(); // ros::spin() 함수를 이용하여 반복 구독을 수행하고 callback을 지속적으로 요청합니다. 
}


// TODO : Identificate robot id 
// TODO : Load 6 robots in a world
// TODO : Make reset robot's position with command
// TODO : Pushlish cmd_vel topic for a while
// TODO : Determine current robot's position
// TODO : Make turn function
// TODO : Make go straight function
// TODO : According to plan call function




	//https://www.opentutorials.org/module/2894/16661 GOD !! GOD!!
	// build tf quaternion
	  tf2::Quaternion q( _odom_msg->pose.pose.orientation.x, 
	  	_odom_msg->pose.pose.orientation.y, 
	  	_odom_msg->pose.pose.orientation.z, 
	  	_odom_msg->pose.pose.orientation.w );
	  // build tf matrix from the quaternion
	  tf2::Matrix3x3 m(q);
	  // get angles
	  double roll, pitch, yaw;
	  m.getRPY(roll, pitch, yaw);

