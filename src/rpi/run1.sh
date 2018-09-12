#!/bin/bash

cd `dirname $0`

exec 5> debug_output.log
BASH_XTRACEFD="5"
PS4='$LINENO: '
set -x

date

#################################
echo 'DEBUG'
echo `ip addr`
#################################

echo '### dialup ###'
#sudo eject /dev/sr0
#sleep 60
#sleep 70
./dialup.sh 2>&1 dialup.log &
sleep 5
#sleep 10
#sleep 60
./wait_networkconnection.pl

git pull

echo '### raspistill ###'
sudo raspistill -o test.jpg -q 10 -w 320 -h 240

echo '### camera ###'
python camera.py

gpio mode 1 out
gpio write 1 1

#echo '### sleep 300 ###'
sleep 1200
#sleep 5

date

echo 'finished'
sudo shutdown -h now
#sudo reboot


