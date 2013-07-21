#!/bin/bash

# random display index to avoid conflicts
export DISPLAY=:1609

date >> xvfb.log
Xvfb $DISPLAY >> xvfb.log 2>&1 &
XVFB_PID=$!

# wait for display
sleep 2s

while read file
do
  if [ ! -e ${file/json/log} ]
  then
    echo $file
    timeout -k 1m 1d ./mmlt $file > ${file/json/log} 2>&1 
  fi
done

kill $XVFB_PID >> xvfb.log 2>&1
sleep 2m
kill -9 $XVFB_PID >> xvfb.log 2>&1
