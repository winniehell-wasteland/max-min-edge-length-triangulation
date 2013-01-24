#!env python2

import os
import re

RE_FILE_NAME  = re.compile(r'(?P<prefix>[^\d]+)_(?P<num_points>\d+)_(?P<parameters>.+)\.log')
#RE_TIME_STAMP = re.compile(r'\[(?P<hour>\d{2}):(?P<minute>\d{2}):(?P<second>\d{2}).(?P<millisecond>\d{3})\]')
RE_TIMES      = re.compile(r'Total .+(?P<time>\d+\.\d+) sec.')

times = {}

for file_name in os.listdir('.'):
    if file_name.endswith('.log'):
        num_points = int(RE_FILE_NAME.match(file_name).group('num_points'))

        if num_points not in times.keys():
            times[num_points] = []

        with open(file_name, 'r') as log_file:
            for line in log_file:
                match = RE_TIMES.match(line)
                if match:
                    times[num_points].append(1000*float(match.group('time')))

with open('times.dat', 'w') as data_file:
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
