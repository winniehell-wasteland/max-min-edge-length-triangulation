#!/usr/bin/env python2
import glob

import json
import re
import sys

LOG_PATTERNS = {
    'file':     re.compile(r'(?P<prefix>[^\d\s]+)_(?P<num_points>\d+)'
                           '_(?P<range>\d+)_(?P<iteration>\d+)\.json'
                           ),
#    'igraph':   re.compile(r'intersections=(?P<intersections>\d+)'
#                           ' overlaps=(?P<overlaps>\d+)'
#                           ),
    'input':    re.compile(r'points=(?P<points>\d+)'
                           ' segments=(?P<segments>\d+)'
                           ' convex hull=(?P<convex_hull>\d+)'
                           ),
    'shortest': re.compile(r'shortest (?P<object>[^:]+) segment:'
                           ' (?P<index>\d+):\(.+\)->\(.+\)'
                           ' \(len\^2=(?P<squared_length>.+)\)'
                           ),
    'stats':    re.compile(r'\[STATS\] it=(?P<iteration>\d+)\t'
                           'lb=(?P<lower_bound>\d+)\t'
                           'ub=(?P<upper_bound>\d+)'
                           ),

    'time':     re.compile(r'\[TIME\] (?P<task>.+)=(?P<time>\d+)ms')
}

#'time_stamp'  : re.compile(r'\[(?P<hour>\d{2}):(?P<minute>\d{2}):(?P<second>\d{2}).(?P<millisecond>\d{3})\]')


def make_key(values, key, value=None):
    """
    make key the key of the values dictionary and value the value
    """

    if value is not None:
        return dict(
            (item[key].replace(' ', '_'), item[value])
            for item
            in values
        )
    else:
        return dict(
            (
                item[key].replace(' ', '_'),
                dict(
                    (value, item[value])
                    for value in item.iterkeys()
                    if value != key
                )
            )
            for item
            in values
        )


def numberize(obj):
    """
    convert values of obj to numbers if possible
    """

    if isinstance(obj, int) or isinstance(obj, float):
        return obj
    if isinstance(obj, dict):
        return dict(
            (key, numberize(value))
            for key, value
            in obj.iteritems()
        )
    elif isinstance(obj, list):
        return list(
            numberize(value)
            for value
            in iter(obj)
        )
    elif isinstance(obj, str) or isinstance(obj, unicode):
        try:
            return int(obj)
        except ValueError:
            try:
                return float(obj)
            except ValueError:
                return obj
    else:
        print obj
        raise NotImplementedError

columns = {}
results = []

for key in LOG_PATTERNS.keys():
    columns[key] = set()

#for file_name in os.listdir('.'):
#for file_name in sys.argv[1:]:
for file_name in sys.stdin.readlines():
    file_name = file_name.rstrip()

    if file_name.endswith('.log'):
        print 'Parsing {file} ...'.format(file=file_name)

        with open(file_name, 'r') as log_file:
            result = {}

            for line in log_file:
                for key, regex in LOG_PATTERNS.items():
                    match = regex.search(line)
                    if match:
                        if key not in result.keys():
                            result[key] = []

                        result[key].append(match.groupdict())

            # convert single element arrays to scalars
            for key in ('file', 'input'):
                assert len(result[key]) == 1
                result[key] = result[key][0]

            #parameters = result['file'].pop('parameters').split('_')
            #result['file'].update(
            #    ('param_{}'.format(i), parameters[i])
            #    for i
            #    in range(len(parameters))
            #)

            result['shortest'] = make_key(result['shortest'], 'object')
            result['time'] = make_key(result['time'], 'task', 'time')

            if 'stats' in result.keys():
                result['stats'] = make_key(result['stats'], 'iteration')
            else:
                result['stats'] = {}

            # normalize
            #for key in result['shortest'].iterkeys():
            #    result['shortest'][key]['squared_length'] = (
            #        float(result['shortest'][key]['squared_length']) /
            #        (
            #            float(result['file']['param_0']) / 1000 -
            #            float(result['file']['param_1']) / 1000
            #        ) ** 2
            #    )

            for key in LOG_PATTERNS.iterkeys():
                columns[key].update(result[key].keys())

            results.append(numberize(result))

#import pprint

#pprint.pprint(results)
#pprint.pprint(columns)

with open('results.json', 'w') as data_file:
    json.dump(results, data_file, indent=4)

