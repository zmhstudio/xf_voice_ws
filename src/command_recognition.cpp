/**************************************************************************
作者：Zhan Minghao
功能：命令控制器，命令词识别结果转化为对应的执行动作
**************************************************************************/
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <iostream>
#include <stdio.h>
#include <joint.h>
#include <std_msgs/Int8.h>
#include <geometry_msgs/PoseStamped.h>
#include <stdlib.h>
#include <xf_mic_asr_offline_line/voiceOrder.h>
#include <nlohmann/json.hpp> 

using namespace std;
ros::Publisher vel_pub;    //创建底盘运动话题发布者
ros::Publisher cmd_vel_pub;
ros::Publisher goal_control_pub;
ros::Publisher follow_flag_pub;    //创建寻找声源标志位话题发布者
ros::Publisher cmd_vel_flag_pub;    //创建底盘运动控制器标志位话题发布者
ros::Publisher awake_flag_pub;    //创建唤醒标志位话题发布者
ros::Publisher navigation_auto_pub;    //创建自主导航目标点话题发布者

ros::Publisher robot_arm_command;	//创建robot_arm_command话题发布者

geometry_msgs::Twist cmd_msg;    //底盘运动话题消息数据
geometry_msgs::PoseStamped target;    //导航目标点消息数据

int voice_flag = 0;    //寻找标志位
int voice_open_off = 0;
int goal_control=0;

float I_position_x ;
float I_position_y ;
float I_orientation_z ;
float I_orientation_w ;

float J_position_x ;
float J_position_y ;
float J_orientation_z ;
float J_orientation_w ;

float K_position_x ;
float K_position_y ;
float K_orientation_z ;
float K_orientation_w ;

float line_vel_x ;
float ang_vel_z ;
float turn_line_vel_x ;

nlohmann::json vision_back_msg = nlohmann::json::parse("{\"index\":[0,1,2], \"class\":[\"苹果\",\"橘子\",\"函数\"]}");
 
/**************************************************************************
函数功能：离线命令词识别结果sub回调函数
入口参数：命令词字符串  voice_control.cpp等
返回  值：无
**************************************************************************/
void voice_words_callback(const std_msgs::String& msg)
{
	/***指令集***/
	string str1 = msg.data.c_str();    //取传入数据
	string str2 = "左手向上十厘米";
	string str3 = "左手向下十厘米"; 
	string str4 = "左手向前五厘米";
	string str5 = "向后五厘米";
	string str6 = "向左十厘米";
	string str7 = "左手向右十厘米";
	string str8 = "比个一";
	string str9 = "比个三";
	string str10 = "右手比个四";
	string str11 = "右手比个耶";
	string str12 = "比个六";
	string str13 = "右手比个欧可";
	string str14 = "双手向上";
	string str15 = "双手向下";
	string str16 = "双手向前";
	string str17 = "双手向后";
	string str18 = "双手向左";
	string str19 = "双手向右";
	string str20 = "倒水";
	string str21 = "左手挥手";
	string str22 = "右手挥手";
	string str23 = "双手挥手";
	string str24 = "左手碰拳";
	string str25 = "左手回到初始位置";
	string str26 = "右手回到初始位置";
	string str27 = "双手回到初始位置";
	string str28 = "抓箱子";
	string str29 = "左手和我猜个拳吧";
	string str30 = "和我猜个拳吧";
	string str31 = "双手松开";
	string str32 = "承让承让";
	string str33 = "抓苹果";
	string str34 = "抓橘子";

	// Json key&data, initialized with None and -1
	string LA_order = "None";
	string RA_order = "None";
	int item;
	string task = "None";
	string content;

/***********************************
指令：L_arm_up_10
动作：左手向上十厘米
***********************************/
	if(str1 == str2)
	{
		cout<<"feed back command:  L_arm_up_10"<<endl;
		cout<<"好的：左手向上十厘米"<<endl;
		content = str2;
		LA_order = "{\"order\":\"up\",\"data\":0.1}";
	}
/***********************************
指令：L_arm_down_10
动作：左手向下十厘米
***********************************/
	if(str1 == str3)
	{
		cout<<"feed back command:  L_arm_down_10"<<endl;
		cout<<"好的：左手向下十厘米"<<endl;
		content = str3;
		LA_order = "{\"order\":\"down\",\"data\":0.1}";
	}
/***********************************
指令：L_arm_forward_0.05
动作：左手向前五厘米
***********************************/
	if(str1 == str4)
	{
		cout<<"feed back command:  L_arm_forward_0.05"<<endl;
		cout<<"好的：左手向前五厘米"<<endl;
		content = str4;
		LA_order = "{\"order\":\"forward\",\"data\":0.05}";
	}
/***********************************
指令：R_arm_backward_0.05
动作：右手向后五厘米
***********************************/
	if(str1 == str5)
	{
		cout<<"feed back command:  R_arm_backward_0.05"<<endl;
		cout<<"好的：右手向后五厘米"<<endl;
		content = str5;
		RA_order = "{\"order\":\"backward\",\"data\":0.05}";
	}
/***********************************
指令：R_arm_left_0.1
动作：右手向左十厘米
***********************************/
	if(str1 == str6)
	{
		cout<<"feed back command:  R_arm_left_0.1"<<endl;
		cout<<"好的：右手向左十厘米"<<endl;
		content = str6;
		RA_order = "{\"order\":\"left\",\"data\":0.1}";
	}
/***********************************
指令：L_arm_right_0.1
动作：左手向右十厘米
***********************************/
	if(str1 == str7)
	{
		cout<<"feed back command:  L_arm_right_0.1"<<endl;
		cout<<"好的：左手向右十厘米"<<endl;
		content = str7;
		LA_order = "{\"order\":\"right\",\"data\":0.1}";
	}
/***********************************
指令：R_hand_one
动作：右手比个一
***********************************/
	if(str1 == str8)
	{
		cout<<"feed back command:  R_hand_one"<<endl;
		cout<<"好的：右手比个一"<<endl;
		content = str8;
		task = "R_handOne";
	}
/***********************************
指令：R_hand_three
动作：右手比个三
***********************************/
	if(str1 == str9)
	{
		cout<<"feed back command:  R_hand_three"<<endl;
		cout<<"好的：右手比个三"<<endl;
		content = str9;
		task = "R_handThree";
	}
/***********************************
指令：R_hand_four
动作：右手比个四
***********************************/
	if(str1 == str10)
	{
		cout<<"feed back command:  R_hand_four"<<endl;
		cout<<"好的：右手比个四"<<endl;
		content = str10;
		task = "R_handFour";
	}
/***********************************
指令：R_hand_yeah
动作：右手比个耶
***********************************/
	if(str1 == str11)
	{
		cout<<"feed back command:  R_hand_yeah"<<endl;
		cout<<"好的：右手比个耶"<<endl;
		content = str11;
		task = "R_yeah";
	}
/***********************************
指令：R_hand_six
动作：右手比个六
***********************************/
	if(str1 == str12)
	{
		cout<<"feed back command:  R_hand_six"<<endl;
		cout<<"好的：右手比个六"<<endl;
		content = str12;
		task = "R_handSix";
	}
/***********************************
指令：R_hand_ok
动作：右手比个OK
***********************************/
	if(str1 == str13)
	{
		cout<<"feed back command:  R_hand_ok"<<endl;
		cout<<"好的:右手比个OK"<<endl;
		content = "右手比个OK";
		task = "R_ok";
	}
/***********************************
指令：LR_arm_up
动作：双手向上运动
***********************************/
	if(str1 == str14)
	{
		cout<<"feed back command:  LR_arm_up"<<endl;
		cout<<"好的：双手向上"<<endl;
		content = str14;
		LA_order = "{\"order\":\"up\",\"data\":-1}";
		RA_order = "{\"order\":\"up\",\"data\":-1}";
	}
/***********************************
指令：LR_arm_down
动作：双手向下运动
***********************************/
	if(str1 == str15)
	{
		cout<<"feed back command:  LR_arm_down"<<endl;
		cout<<"好的：双手向下"<<endl;
		content = str15;
		LA_order = "{\"order\":\"down\",\"data\":-1}";
		RA_order = "{\"order\":\"down\",\"data\":-1}";
	}
/***********************************
指令：LR_arm_forward
动作：双手向前运动
***********************************/
	if(str1 == str16)
	{
		cout<<"feed back command:  LR_arm_forward"<<endl;
		cout<<"好的：双手向前"<<endl;
		content = str16;
		LA_order = "{\"order\":\"forward\",\"data\":-1}";
		RA_order = "{\"order\":\"forward\",\"data\":-1}";
	}
/***********************************
指令：LR_arm_backward
动作：双手向后运动
***********************************/
	if(str1 == str17)
	{
		cout<<"feed back command:  LR_arm_backward"<<endl;
		cout<<"好的：双手向后"<<endl;
		content = str17;
		LA_order = "{\"order\":\"backward\",\"data\":-1}";
		RA_order = "{\"order\":\"backward\",\"data\":-1}";
	}
/***********************************
指令：LR_arm_left
动作：双手向左运动
***********************************/
	if(str1 == str18)
	{
		cout<<"feed back command:  LR_arm_left"<<endl;
		cout<<"好的：双手向左"<<endl;
		content = str18;
		LA_order = "{\"order\":\"left\",\"data\":-1}";
		RA_order = "{\"order\":\"left\",\"data\":-1}";
	}
/***********************************
指令：LR_arm_right
动作：双手向右运动
***********************************/
	if(str1 == str19)
	{
		cout<<"feed back command:  LR_arm_right"<<endl;
		cout<<"好的：双手向右"<<endl;
		content = str19;
		LA_order = "{\"order\":\"right\",\"data\":-1}";
		RA_order = "{\"order\":\"right\",\"data\":-1}";
	}
/***********************************
指令：pourWater
动作：倒水
***********************************/
	if(str1 == str20)
	{
		cout<<"feed back command:  pourWater"<<endl;
		cout<<"好的：倒水"<<endl;
		content = str20;
		task = "pourWater";
	}
/***********************************
指令：L_wave
动作：左手挥手
***********************************/
	if(str1 == str21)
	{
		cout<<"feed back command:  L_wave"<<endl;
		cout<<"好的：左手挥手"<<endl;
		content = str21;
		task = "L_wave";
	}
/***********************************
指令：R_wave
动作：右手挥手
***********************************/
	if(str1 == str22)
	{
		cout<<"feed back command:  R_wave"<<endl;
		cout<<"好的：右手挥手"<<endl;
		content = str22;
		task = "R_wave";
	}
/***********************************
指令：LR_wave
动作：双手挥手
***********************************/
	if(str1 == str23)
	{
		cout<<"feed back command:  LR_wave"<<endl;
		cout<<"好的：双手挥手"<<endl;
		content = str23;
		task = "LR_wave";
	}
/***********************************
指令：L_fist
动作：左手碰拳
***********************************/
	if(str1 == str24)
	{
		cout<<"feed back command:  L_fist"<<endl;
		cout<<"好的：左手碰拳"<<endl;
		content = str24;
		task = "L_fist";
	}
/***********************************
指令：L_base
动作：左手回到初始位置
***********************************/
	if(str1 == str25)
	{
		cout<<"feed back command:  L_base"<<endl;
		cout<<"好的：左手回到初始位置"<<endl;
		content = str25;
		task = "L_base";
	}
/***********************************
指令：R_base
动作：右手回到初始位置
***********************************/
	if(str1 == str26)
	{
		cout<<"feed back command:  R_base"<<endl;
		cout<<"好的：右手回到初始位置"<<endl;
		content = str26;
		task = "R_base";
	}
/***********************************
指令：LR_base
动作：双手回到初始位置
***********************************/
	if(str1 == str27)
	{
		cout<<"feed back command:  LR_base"<<endl;
		cout<<"好的：双手回到初始位置"<<endl;
		content = str27;
		task = "LR_base";
	}
/***********************************
指令：LR_moveBox
动作：抓箱子
***********************************/
	if(str1 == str28)
	{
		cout<<"feed back command:  LR_moveBox"<<endl;
		cout<<"好的：抓箱子"<<endl;
		content = str28;
		task = "moveBox";
	}
/***********************************
指令：L_Rock_Scissors_Paper
动作：左手猜拳
***********************************/
	if(str1 == str29)
	{
		cout<<"feed back command:  L_Rock_Scissors_Paper"<<endl;
		cout<<"好的：左手猜拳"<<endl;
		content = str29;
		switch(int temp = rand()%3)
		{
			case 0:
			task = "L_scissors";
			break;
			case 1:
			task = "L_stone";
			break;
			case 2:
			task = "L_cloth";
			break;
		};
	}
/***********************************
指令：R_Rock_Scissors_Paper
动作：右手猜拳
***********************************/
	if(str1 == str30)
	{
		cout<<"feed back command:  R_Rock_Scissors_Paper"<<endl;
		cout<<"好的：右手猜拳"<<endl;
		content = str30;
		switch(int temp = rand()%3)
		{
			case 1:
			task = "R_scissors";
			break;
			case 2:
			task = "R_stone";
			break;
			case 0:
			task = "R_cloth";
			break;
		};
	}
/***********************************
指令：LR_hand_release
动作：双手松开
***********************************/
	if(str1 == str31)
	{
		cout<<"feed back command:  LR_hand_release"<<endl;
		cout<<"好的：双手松开"<<endl;
		content = str31;
		task = "LR_release";
	}
/***********************************
指令：LR_concession
动作：拱手相让
***********************************/
	if(str1 == str32)
	{
		cout<<"feed back command:  LR_concession"<<endl;
		cout<<"好的：拱手相让"<<endl;
		content = str32;
		task = "LR_concession";
	}
/***********************************
指令：R_grab_apple 
动作：抓苹果
***********************************/
	if(str1 == str33)
	{
		cout<<"feed back command:  R_grab_apple"<<endl;
		cout<<"好的：抓苹果"<<endl;
		content = str33;
		string temp_str = "苹果";
		int item = -1;
		for(int i=0;i<vision_back_msg["index"].size();i++)
		{
			if(vision_back_msg["class"][i] == temp_str){item = i;}
		}
		RA_order = "{\"order\":\"grab\",\"data\":" + std::to_string(item) + "}";
	}
/***********************************
指令：R_grab_orange 
动作：抓橘子
***********************************/
	if(str1 == str34)
	{
		cout<<"feed back command:  R_grab_orange"<<endl;
		cout<<"好的：抓橘子"<<endl;
		content = str34;
		string temp_str = "橘子";
		int item = -1;
		for(int i=0;i<vision_back_msg["index"].size();i++)
		{
			if(vision_back_msg["class"][i] == temp_str){item = i;}
		}
		RA_order = "{\"order\":\"grab\",\"data\":" + std::to_string(item) + "}";
	}

// xf_mic_asr_offline_line::voiceOrder VO;
	std_msgs::String voiceOrder;
	voiceOrder.data = "{\"leftArm\":\"" + LA_order
					+ "\",\"rightArm\":\"" + RA_order
					+ "\",\"task\":\"" + task
					+ "\",\"content\":\"" + content 
					+ "\"}";
/******                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ************************************/
// PUBLISHER
    cout<<"publishing....."<<endl;
	robot_arm_command.publish(voiceOrder);
	cout<<voiceOrder<<endl;
	cout<<"succeed......"<<endl;
}




/**************************************************************************
函数功能：寻找语音开启成功标志位sub回调函数
入口参数：voice_flag_msg  voice_control.cpp
返回  值：无
**************************************************************************/
void voice_flag_Callback(std_msgs::Int8 msg)
{
	voice_flag = msg.data;
	OTHER = (char*) "/feedback_voice/voice_control.wav";
	WHOLE = join((head + audio_path),OTHER);
	if(voice_flag == 1 && !voice_open_off)
	{
		system(WHOLE);
		voice_open_off = 1;
		cout<<"语音打开成功"<<endl;
	}
}

/**************************************************************************
函数功能：视觉引导回传消息话题回调函数
入口参数：vision_back_msg  voice_control.cpp
返回  值：无
**************************************************************************/
void visionRecognize_Callback(std_msgs::String msg)
{
	// cout<<"视觉反馈ys： "<<vision_back_msg_str<<endl;
	std::string vision_back_msg_str = msg.data;
	vision_back_msg = nlohmann::json::parse(vision_back_msg_str);
	cout<<"视觉反馈： "<<vision_back_msg<<endl;
}


void test_json()
{
	std::string vision_back_msg_str = "{\"key\":[1,2,3]}";
	// Json::Value vision_data;
	// Json::Reader reader;
	nlohmann::json vision_back_msg = nlohmann::json::parse(vision_back_msg_str);
	// Json::Value vision_back_msg;
	//Value vision_back_msg = reader.parse(vision_back_msg_str);
	// reader.parse(vision_back_msg_str , vision_back_msg);
	cout<<"视觉反馈： "<<vision_back_msg<<"---"<<vision_back_msg["key"][1]<<endl;
	//Json::Value classArray = vision_back_msg["class"];
}


/*
void kill_pro(char pro_name[])
{
	char get_pid[30] = "pidof ";
	strcat(get_pid,pro_name);
	FILE *fp = popen(get_pid,"r");
	
	char pid[10] = {0};
	fgets(pid,10,fp);
	pclose(fp);
	
	char cmd[20] = "kill -9 ";
	strcat(cmd,pid);
	system(cmd);
	
}
*/

/**************************************************************************
函数功能：主函数
入口参数：无
返回  值：无
**************************************************************************/
int main(int argc, char** argv)
{
	// system("")
	test_json();
	ros::init(argc, argv, "cmd_rec");     //初始化ROS节点

	ros::NodeHandle n;    //创建句柄
	robot_arm_command = n.advertise<std_msgs::String>("/wukong_demo/voiceOrder", 1000);
	// robot_arm_command = n.advertise<std_msgs::String>("/realman_demo/voiceOrder", 1000);
	
	string if_akm;
	
	/***创建寻找声源标志位话题发布者***/
	follow_flag_pub = n.advertise<std_msgs::Int8>("follow_flag",1);

	/***创建I、J、K点到达标志位话题发布者***/
	goal_control_pub = n.advertise<std_msgs::Int8>("goal_control_flag",1);

	/***创建底盘运动控制器标志位话题发布者***/
	cmd_vel_flag_pub = n.advertise<std_msgs::Int8>("cmd_vel_flag",1);

	/***创建底盘运动话题发布者***/
	vel_pub = n.advertise<geometry_msgs::Twist>("ori_vel",1);

	cmd_vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);

	/***创建唤醒标志位话题发布者***/
	awake_flag_pub = n.advertise<std_msgs::Int8>("awake_flag", 1);

  /***创建自主导航目标点话题发布者***/
	navigation_auto_pub = n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",1);

	/***创建离线命令词识别结果话题订阅者***/
	ros::Subscriber voice_words_sub = n.subscribe("voice_words",1,voice_words_callback);

	/***创建寻找语音开启标志位话题订阅者***/
	ros::Subscriber voice_flag_sub = n.subscribe("voice_flag", 1, voice_flag_Callback);

	/***创建视觉引导回传消息话题订阅者***/
	ros::Subscriber visionRecognizeMsg_sub = n.subscribe("/wukong_demo/visionRecognizeBackMsg", 1, visionRecognize_Callback);

	n.param("/command_recognition/audio_path", audio_path, std::string("~/catkin_ws1111/src/xf_mic_asr_offline/feedback_voice"));


	n.param<float>("/I_position_x", I_position_x, 1);
	n.param<float>("/I_position_y", I_position_y, 0);
	n.param<float>("/I_orientation_z", I_orientation_z, 0);
	n.param<float>("/I_orientation_w", I_orientation_w, 1);
	n.param<float>("/J_position_x", J_position_x, 2);
	n.param<float>("/J_position_y", J_position_y, 0);
	n.param<float>("/J_orientation_z", J_orientation_z, 0);
	n.param<float>("/J_orientation_w", J_orientation_w, 1);
	n.param<float>("/K_position_x", K_position_x, 3);
	n.param<float>("/K_position_y", K_position_y, 0);
	n.param<float>("/K_orientation_z", K_orientation_z, 0);
	n.param<float>("/K_orientation_w", K_orientation_w, 1);
	n.param<float>("/line_vel_x", line_vel_x, 0.2);
	n.param<float>("/ang_vel_z", ang_vel_z, 0.2);
	n.param("/if_akm_yes_or_no", if_akm, std::string("no"));

	if(if_akm == "yes")
		turn_line_vel_x = 0.2;
	else 
		turn_line_vel_x = 0;

	/***自主导航目标点数据初始化***/
	target.header.seq = 0;
	//target.header.stamp;
	target.header.frame_id = "map";
	target.pose.position.x = 0;
	target.pose.position.y = 0;
	target.pose.position.z = 0;
	target.pose.orientation.x = 0;
	target.pose.orientation.y = 0;
	target.pose.orientation.z = 0;
	target.pose.orientation.w = 1;


  /***用户界面***/
	sleep(7);
	cout<<"您可以语音控制啦！唤醒词“你好小电”"<<endl;
	cout<<"(左手|右手|双手|拿|倒)"<<endl;
	cout<<"向上|向下|向前|向后|向左|向右|挥手|回到初始位置"<<endl;

	// 回调函数循环
	ros::spin();
}


