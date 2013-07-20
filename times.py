#!/usr/bin/env python2

import numpy as np

import results
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

print 'Loading results...'
data = results.load(
    'file/num_points',
    'file/iteration',
    'file/range',
    'time/total',
    'time/SAT_solving',
    'time/generating_segments',
    'shortest/non-crossing/index'
)


def output_time_comparison(data, instances):
    print 'Generating time comparison...'

    columns = {
        'time/total': 'total running time',
        'time/SAT_solving': 'time for SAT solving',
        'time/generating_segments': 'time for segment generation'
    }
    aggregations = [
        np.median,
        results.err(results.mad)
    ]

    data = data[columns.keys()]
    data = data.agg(aggregations)

    data.index.name = 'number of points'

    #print data

    axes = results.plot_error(
        data=data,
        columns=columns,
        aggregations=aggregations
    )

    axes.set_xlabel(data.index.name)
    axes.set_ylabel('time in milliseconds')
    results.plot_axes(data=data, axes=axes, log_scale=True, y_min=(10 ** -1))

    results.output(
        'time_comparison',
        data=data,
        columns=columns,
        aggregations=aggregations,
        instances=instances
    )


def output_time_total(data, instances):
    print 'Generating total time...'

    columns = {
        'time/total': 'total running time'
    }
    aggregations = [
        np.median,
        results.err(results.mad)
    ]

    data = data[columns.keys()]
    data = data.agg(aggregations)

    data.index.name = 'number of points'

    #print data

    axes = results.plot_error(
        data=data,
        columns=columns,
        aggregations=aggregations
    )

    def func(n, a, b, c):
        return a * np.power(n, b) + c

    params = curve_fit(func, data.index.values, data['time/total']['median'])[0]

    plt.plot(
        data.index, func(data.index, params[0], params[1], params[2]),
        label=r'\({0:.2f} \cdot n^{{1:.2f}} {2:+.2f}\)'
        .format(*(float(param) for param in params))
    )

    axes.set_xlabel(data.index.name)
    axes.set_ylabel('time in milliseconds')
    results.plot_axes(data=data, axes=axes, log_scale=True)

    results.output(
        'time_total',
        data=data,
        columns=columns,
        aggregations=aggregations,
        instances=instances
    )


def output_time_noncrossing(data, instances, threshold):
    print 'Generating time noncrossing...'

    columns = {
        'time/total': 'total running time'
    }
    aggregations = [
        np.median,
        results.err(results.mad)
    ]

    data = data[columns.keys()]
    data = data.agg(aggregations)
    data = data[data.index <= np.min(
        [i for i in data.index if instances[i] < threshold]
    )]

    data.index.name = 'shortest non-crossing segment index'

    #print data

    axes = results.plot_error(
        data=data,
        columns=columns,
        aggregations=aggregations
    )

    axes.set_xlabel(data.index.name)
    axes.set_ylabel('time in milliseconds')
    results.plot_axes(data=data, axes=axes, log_scale=True, x_min=-1)

    x_range = axes.get_xlim()
    axes.set_xticks(np.arange(0, x_range[1], 1))

    results.output(
        'time_noncrossing',
        data=data,
        columns=columns,
        aggregations=aggregations,
        instances=instances
    )


def output_time_hist(data, threshold):
    print 'Output time histogram...'

    def count(a):
        return len(a)

    columns = {
        'file/iteration': 'instances'
    }
    aggregations = [
        count
    ]

    data = data[data['time/total'] < threshold]
    data = data.groupby('file/num_points')

    data = data[columns.keys()]
    data = data.agg(aggregations)

    #print data

    data.index.name = 'number of points'

    fig = plt.figure(figsize=results.FIG_SIZE)
    axes = fig.add_subplot(111)

    for key, label in columns.iteritems():
        column = data[key]
        axes.bar(
            data.index,
            column['count'],
            label=label,
            width=10,
            align='center'
        )

    axes.set_xlabel(data.index.name)
    axes.set_ylabel('instances < ' + str(threshold) + ' milliseconds')
    results.plot_axes(data=data, axes=axes, log_scale=False, y_min=0)

    results.output(
        'time_hist',
        data=data,
        columns=columns,
        aggregations=aggregations,
        instances=None
    )


def output_time_allhist(data):
    print 'Output all time histograms...'

    for points in np.unique(data['file/num_points']):
        hist_data = data[data['file/num_points'] == points]

        fig = plt.figure(figsize=results.FIG_SIZE)
        axes = fig.add_subplot(111)

        axes.set_xlabel('time in milliseconds')
        axes.set_ylabel('number of instances')

        axes.hist(hist_data['time/total'], bins=50)

        axes.savefig('time_hist_' + str(points) + '.pdf', bbox_inches='tight')

if __name__ == '__main__':
    x_column = 'file/num_points'
    instances = {
        x_val: len(data[data[x_column] == x_val])
        for x_val
        in data[x_column]
    }
    data = data.groupby(x_column)
    output_time_comparison(data, instances)
    output_time_total(data, instances)
    data = data.obj

    x_column = 'shortest/non-crossing/index'
    instances = {
        x_val: len(data[data[x_column] == x_val])
        for x_val
        in data[x_column]
    }
    data = data.groupby(x_column)
    output_time_noncrossing(data, instances, threshold=50)
    data = data.obj

    output_time_hist(data, 30 * 60 * 1000)
    #output_time_allhist(data)
