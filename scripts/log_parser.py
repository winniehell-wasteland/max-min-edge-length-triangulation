#!/usr/bin/env python2

import os
import re
import sys

REGEXES = {
    'file_name'   : re.compile(r'(?P<prefix>[^\d]+)_(?P<num_points>\d+)_(?P<parameters>.+)_\d+\.log'),
    'igraph_data' : re.compile(r'intersections=(?P<intersections>\d+) overlaps=(?P<overlaps>\d+)'),
    'input_data'  : re.compile(r'points=(?P<points>\d+) segments=(?P<segments>\d+) convex hull=(?P<convex_hull>\d+)'),
    'shortest'    : re.compile(r'shortest (?P<object>[^:]+): (?P<index>\d+):\(\d+,\d+\)->\(\d+,\d+\) \(len^2=(?P<squared_length>\d+)\)'),
    'stats'       : re.compile(r'\[STATS\] it=(?P<iteration>\d+)\tlb=(?P<lower_bound>\d+)\tub=(?P<upper_bound>\d+)'),
    'time'        : re.compile(r'\[TIME\] (?P<task>.+)=(?P<time>\d+)ms')
    #'time_stamp'  : re.compile(r'\[(?P<hour>\d{2}):(?P<minute>\d{2}):(?P<second>\d{2}).(?P<millisecond>\d{3})\]')
}

def make_key(values, key, value=None):
    """
    make key the key of the values dictionary and value the value
    """

    if value is not None:
        return dict(
            (
                (item[key], item[value])
                for item
                in values
            )
        )
    else:
        return dict(
            (
                (item[key],
                    dict(
                        (value, item[value])
                        for value in item.keys()
                        if value != key
                    )
                )
                for item
                in values
            )
        )

results = []

TIME_TASKS = set()

for file_name in os.listdir('.'):
    if file_name.endswith('.log'):
        with open(file_name, 'r') as log_file:
            result = {}

            for line in log_file:
                for key, regex in REGEXES.items():
                    match = regex.search(line)
                    if match:
                        if key not in result.keys():
                            result[key] = []

                        result[key].append(match.groupdict())

            result['time'] = make_key(result['time'], 'task', 'time')
            TIME_TASKS.update(result['time'].keys())

print TIME_TASKS
sys.exit(0)

with open('results.dat', 'w') as data_file:
    data_file.write('# num_points\tavg\tmin\tmax\n')

    for num_points in times.keys():
        if len(times[num_points]) < 1:
            continue

        time_avg = sum(times[num_points])/len(times[num_points])
        time_max = max(times[num_points])
        time_min = min(times[num_points])

        data_file.write('{num_points}\t{avg}\t{min}\t{max}\n'.format(
            num_points=num_points,
            avg=time_avg,
            min=time_min,
            max=time_max
        ))
