#!/bin/bash

for dir in ~/Projects/masterarbeit/tests/{,aborted/,complete_sat/}square/uniform_distribution/
do
    echo $dir
    cd $dir
    xvfb-run ~/Projects/masterarbeit/scripts/times.py
    sleep 2s
    xvfb-run ~/Projects/masterarbeit/scripts/segments.py
    sleep 2s
done
