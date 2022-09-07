#!/bin/bash
#Do not run in sudo
name=`whoami`
if [ $name = "root" ]; then
	echo "Do not run in root mode."
	exit 1
fi

while [ 1 ]
do
    val=`cat /proc/devices | grep scull | wc -l`
    if [ $val -eq 1 ]; then
       cat /dev/urandom > /dev/dsp
       exit 0
    fi
    sleep 1
done
