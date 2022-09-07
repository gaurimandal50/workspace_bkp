#!/bin/bash
#Do not run in sudo
name=`whoami`
if [ $name = "root" ]; then
	echo "Do not run in root mode."
	exit 1
fi

alarm() {
       l=$1
       for((i = 0; i < l; i++))
       do
           cat test1400.bit > /dev/dsp
	   echo "ALARM FOR STUDY"
	   sleep 1
       done
}

while [ 1 ]
do
    Time=`date | cut -d ' ' -f 5`
    echo $Time
    hour=`echo $Time | cut -d ':' -f 1`
    minute=`echo $Time | cut -d ':' -f 2`
    second=`echo $Time | cut -d ':' -f 3`
    echo $minute $second
    #if [ $hour -eq 13 ] && [ $minute -eq 54 ] && [ $second -eq 00 ] ; then
    for value in `cat routine`
    do
       if [ $Time = $value ]; then
          alarm 6 
       fi
    done
    sleep 1
done
