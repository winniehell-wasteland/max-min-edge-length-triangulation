#!/usr/bin/env python2

import argparse

parser = argparse.ArgumentParser(description='Run tests with random points.')
parser.add_argument('executable', metavar='EXECUTABLE', type=str, help='max-min-triangulation executable')
parser.add_argument('--output_dir', metavar='OUTPUT_DIRECTORY', type=str, default='.', help='output directory')
args = parser.parse_args()

# constants
BASE_FILE_NAME             = '/random_{num_points:03}_{coordinate_range[0]:04}_{coordinate_range[1]:04}_{iteration:02}'
NUM_POINTS_VALUES          = range(4,20,1) + range(20,40,5) + range(40,81,10)
COORDINATE_RANGE_VALUES    = [(0,50),(0,500),(0,5000)]
GNUPLOT_LINE               = ('{num_points}\t{coordinate_range[0]}\t{coordinate_range[1]}\t'
                              + '{intersections_avg}\t{intersections_min}\t{intersections_max}\t'
                              + '{overlaps_avg}\t{overlaps_min}\t{overlaps_max}\t'
                              + '{time_avg}\t{time_min}\t{time_max}\n')

def generate_points(num_points, coordinate_range, file_name):
    import json
    import random

    points = []

    while len(points) < num_points:
        point = (random.randint(*coordinate_range), random.randint(*coordinate_range))

        if point not in points:
            points.append(point)

    json_file = open(file_name + '.json', 'w')

    json.dump(points, json_file)
    json_file.close()

import re
import subprocess
import sys

from datetime import datetime
def parse_datetime(line):
    return datetime.strptime(line.split(' ')[0].replace(']','000]'), '[%H:%M:%S.%f]')

stat_line_regex = re.compile(r'\[.{12}\] \[\] segments: total=\d+ intersections=(?P<intersections>\d+) overlaps=(?P<overlaps>\d+)\n')

with open(args.output_dir + '/random.dat', 'w') as gnuplot_file:
    gnuplot_file.write('# '+GNUPLOT_LINE)

    for num_points in NUM_POINTS_VALUES:
        for coordinate_range in COORDINATE_RANGE_VALUES:
            intersections = []
            overlaps = []
            times = []

            for iteration in range(10):
                file_name = args.output_dir + BASE_FILE_NAME.format(
                    num_points=num_points,
                    coordinate_range=coordinate_range,
                    iteration=iteration
                )

                print 'Generating file {file}.json...'.format(file=file_name)
                generate_points(num_points=num_points, coordinate_range=coordinate_range, file_name=file_name)

                print 'Running file {file}.json...'.format(file=file_name)
                log_file = open(file_name + '.log', 'w')

                if subprocess.call([args.executable, file_name + '.json'], stdout=log_file, stderr=log_file) != 0:
                    log_file.close()
                    sys.exit('Failed!')

                log_file.close()

                last_line = '[00:00:00.000]'
                for line in open(log_file.name, 'r'):
                    match = stat_line_regex.match(line)
                    if match:
                        intersections.append(int(match.group('intersections')))
                        overlaps.append(int(match.group('overlaps')))

                        start_time = parse_datetime(last_line)
                        end_time = parse_datetime(line)
                        times.append((end_time - start_time).total_seconds())
                        break

                    if line != '\n':
                        last_line = line

            gnuplot_file.write(GNUPLOT_LINE.format(
                num_points=num_points,
                coordinate_range=coordinate_range,
                intersections_avg=sum(intersections)/float(len(intersections)),
                intersections_min=min(intersections),
                intersections_max=max(intersections),
                overlaps_avg=sum(overlaps)/float(len(overlaps)),
                overlaps_min=min(overlaps),
                overlaps_max=max(overlaps),
                time_avg=sum(times)/float(len(times)),
                time_min=min(times),
                time_max=max(times),
            ))

            gnuplot_file.flush()

