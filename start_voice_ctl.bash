#!/bin/bash

echo 0 | sudo tee /sys/bus/usb/dev
echo 1 | sudo tee /sys/bus/usb/devices/1-4.4/authorized
gnome-terminal -x bash -c "roslaunch xf_mic_asr_offline_line base.launch; exec bash;"
sleep 3
gnome-terminal -x bash -c "roslaunch xf_mic_asr_offline_line mic_init.launch; exec bash;"
