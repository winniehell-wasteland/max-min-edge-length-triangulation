#!/usr/bin/env python2

import json

DATA_ASPECT = 'shortest'

# read in results
with open('results.json', 'r') as results_file:
    results = json.load(results_file,)

columns = {
    'non-intersecting': ['index', 'squared_length']
}
data = {}

# filter results
for result in results:
    num_points = result['file']['num_points']

    if num_points not in data.iterkeys():
        data[num_points] = dict(
            (group + '_' + column, [])
            for group
            in columns.iterkeys()
            for column
            in columns[group]
        )

    for group in columns.iterkeys():
        for column in columns[group]:
            data[num_points][group + '_' + column].append(result[DATA_ASPECT][group][column])

# aggregate
for num_points in data.iterkeys():
    for key in data[num_points].keys():
        data[num_points][key + '_min'] = min(data[num_points][key])
        data[num_points][key + '_max'] = max(data[num_points][key])
        data[num_points][key + '_avg'] = sum(data[num_points][key]) / len(data[num_points][key])

        data[num_points].pop(key)

# write data
with open(DATA_ASPECT + '.dat', 'w') as data_file:
    assert len(data) > 0
    keys = sorted(data[data.keys()[0]].keys())

    data_file.write(
        '# ' +
        '\t'.join(['num_points'] + keys) +
        '\n'
    )

    for num_points in sorted(data.iterkeys()):
        data_file.write(
            '\t'.join(
                [str(num_points)] +
                [str(data[num_points][key])
                 for key
                 in keys
                 ]
            ) +
            '\n'
        )

# write gnuplot file
with open(DATA_ASPECT + '.gp', 'w') as gnuplot_file:
    gnuplot_file.write(
        '''
set terminal pdf size 30cm, 21cm

set key top left
set xlabel 'Number of input points'
set xrange [{num_points_min}:{num_points_max}]

set output "{data_aspect}_index.pdf"
#set logscale y
set ylabel 'segment index'
plot \
    '{data_aspect}.dat' using 1:3 with lines smooth bezier title 'non-intersecting (max.)', \
    '{data_aspect}.dat' using 1:(sqrt(16*$1)-5) with lines title 'sqrt(16*n)-5', \
    '{data_aspect}.dat' using 1:2 with lines smooth bezier title 'non-intersecting (avg.)', \
    '{data_aspect}.dat' using 1:($1>10?sqrt(1.5*$1)-4:0) with lines title 'sqrt(1.5*n)-4', \
    '{data_aspect}.dat' using 1:4 with lines smooth bezier title 'non-intersecting (min.)'

set output "{data_aspect}_length.pdf"
set logscale y
set ylabel 'normalized squared segment length'
plot \
    '{data_aspect}.dat' using 1:6 with lines smooth bezier title 'non-intersecting (max.)', \
    '{data_aspect}.dat' using 1:(3e6/($1**1.5)) with lines title '3e6/(n^1.5)', \
    '{data_aspect}.dat' using 1:5 with lines smooth bezier title 'non-intersecting (avg.)', \
    '{data_aspect}.dat' using 1:(4e5/($1**1.5)) with lines title '4e5/(n^1.5)', \
    '{data_aspect}.dat' using 1:7 with lines smooth bezier title 'non-intersecting (min.)', \
    '{data_aspect}.dat' using 1:(5e3/($1**1.5)) with lines title '5e3/(n^1.5)'

'''
        .format(
            data_aspect=DATA_ASPECT,
            num_points_max=max(data.iterkeys()),
            num_points_min=min(data.iterkeys())
        )
    )
