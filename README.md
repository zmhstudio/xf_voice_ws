# this is a simple user guide for voice_ws module
# Last modification: 20231214
# author: Zhan Minghao
# 可以部署在ROS1 + Arm64/aarch64的板卡上，推荐操作系统为ubuntu2004，ROS版本noetic
# 核心部署ROS代码包:voice_ws/src/xf_mic_asr_offline_line

# ----------------- 部署方式 --------------------- #
basic setup method:
1. create a ros workspace 
$ mkdir voice_ws/src/
$ cp -r xf_mic_asr_offline_line voice_ws/src/

2. setup cJson:
$ cd json/
$ rm -r build/
$ mkdir build 
$ cd build 
$ cmake ..
$ make
$ sudo make install

3. setup nlohmann_Json:
$ cd cJson/
$ rm -r build/
$ mkdir build 
$ cd build 
$ cmake ..
$ make
$ sudo make install

4. setup libmsc* to /usr/lib:
$ cd voice_ws/src/xf_mic_asr_offline_line/lib/
$ sudo cp lib* /usr/lib/
$ sudo /sbin/ldconfig

5. setup M260C microphone:
插入环形麦克风阵列
$ ll /dev   # 可以看到ttyACM0设备
$ sudo su   # 进入root权限
$ sudo chmod 0777 /dev/ttyACM0
$ sudo chmod 0777 ch9102_udev.sh
$ ./ch9102_udev.sh
$ exit      # 退出root权限
重新插拔环形麦克风阵列
$ ll /dev   # 可以看到 wheeltec_mic -> ttyAMC0，完成设备名称映射

6. build:
$ cd voice_ws/
$ rm -r build
$ rm -r devel  # 删除先前的编译文件
# for M260(recommanded)
$ catkin_make -DCATKIN_WHITELIST_PACKAGES=xf_mic_asr_offline_line
# for M260C(deprecated)		
$ catkin_make -DCATKIN_WHITELIST_PACKAGES=xf_mic_asr_offline_circle 
$ echo "source [absolute_path]/voice_ws/devel/setup.bash" ~/.bashrc	


# ----------------- 使用方式 --------------------- #
7. use:
$ source devel/setup.bash
# 重启麦克风阵列
$ ./restart_mic.bash
# receive awaken and recognition result, give out voice_orders in Json format
$ roslaunch xf_mic_asr_offline_line base.launch	
# main recognition program entrance, callback awaken and recognition result			
$ roslaunch xf_mic_asr_offline_line mic_init.launch	

7.1 一键启动:
$ ./start_voice_ctl.bash


# ----------------- 自定义方式 --------------------- #
8. modification:
8.1. change the awaken word:
use a windows computer and use the AIUI serial assistant

8.2. change APPID and recognition source:
apply for your own voice recognition resource at XunFei_Opensource_Platform # URL: https://www.xfyun.cn/
# swap your common.jet and the origin common.jet
$ sudo cp common.jet voice_ws/src/xf_mic_asr_offline_line/config/msc/res/asr/
# swap your appid and the origin appid
appid interface: voice_ws/src/xf_mic_asr_offline_line/config/appid_params.yaml
# !! the above two resources have to be in pair in case it doesn't work !!
# !! 请务必保证appid与common.jet匹配，否则将无法启动 !!

9. 修改命令词:
9.1 修改命令词语法文件:
command word interface: voice_ws/src/xf_mic_asr_offline_line/config/call.bnf
# 语法逻辑详见讯飞云平台: URL: https://www.xfyun.cn/
9.2 修改命令词对应的输出:
output interface: voice_ws/src/xf_mic_asr_offline_line/src/command_recognition.cpp
# 修改后需要再次执行步骤 5 ，进行重新编译
