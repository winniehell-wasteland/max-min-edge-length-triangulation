#!/usr/bin/env python2

# for num_points in {0003,0005,0010,0020,0050}; do for coord_range in {0050,0100,0200}; do for i in {01..09}; do ./random_points.py $num_points $coord_range > random_${num_points}_${coord_range}_${i}.json; done; done; done

import argparse

parser = argparse.ArgumentParser(description='Generate random points.')
parser.add_argument('num_points', metavar='NUMBER_OF_POINTS', type=int, help='number of points to generate')
parser.add_argument('coordinate_max', metavar='COORDINATE_MAX', type=int, help='upper bound for coordinates')

args = parser.parse_args()

import json
import random

points = []

for i in range(0, args.num_points):
    points.append((random.randint(0, args.coordinate_max+1), random.randint(0, args.coordinate_max+1)))

print json.dumps(points)
