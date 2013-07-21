#!/bin/bash

FINISHED=`seq -w 0010 10 \`find 0* -name '*.log' | grep -oP '^\d+' | sort -u | tail -n 2 | head -n 1\``

for i in $FINISHED
do
    find $i -name '*.log'
done \
| xargs grep -l '\[TIME\] total=' \
| sort \
| ~/Projects/masterarbeit/scripts/log_parser.py
