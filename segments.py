#!/usr/bin/env python2

import numpy as np

import pandas as pd
import results
from scipy.optimize import curve_fit

DEBUG = False
#DEBUG = True


def output_segment_index(data, instances):
    print 'output segment index...'

    columns = {
        'shortest/non-crossing/index': 'shortest non-crossing segment index',
        'shortest/constrained_triangulation/index': 'optimal segment index'
    }
    aggregations = [
        np.median,
        results.err(results.mad)
    ]

    data = data[columns.keys()]
    data = data.agg(aggregations)

    data.index.name = 'number of points'

    #print data

    axes = results.plot_error(data=data, columns=columns, aggregations=aggregations)

    def func(n, a, b, c):
        return a * n ** b + c

    def linear_func(n, a, b):
        return a * n + b

    def rt4_func(n, a, b):
        return a * n ** 0.25 + b

    params = curve_fit(rt4_func, data.index.values, data['shortest/non-crossing/index']['median'])[0]
    axes.plot(
        data.index, rt4_func(data.index, params[0], params[1]),
        label=r'\( ' + str(params[0]) + '\cdot n^{0.25} + ' + str(params[1]) + '\)'
    )

    params = curve_fit(func, data.index.values, data['shortest/constrained_triangulation/index']['median'])[0]
    axes.plot(
        data.index, func(data.index, params[0], params[1], params[2]),
        label=r'\( ' + str(params[0]) + '\cdot n^{' + str(params[1]) + '} + ' + str(params[2]) + '\)'
    )

    axes.set_xlabel(data.index.name)
    axes.set_ylabel('segment index')
    results.plot_axes(data=data, axes=axes, log_scale=True)

    results.output(
        'segment_index',
        data=data,
        columns=columns,
        aggregations=aggregations,
        instances=instances
    )


def output_segment_length(data, instances):
    print 'output segment length...'

    columns = {
        'shortest/input/length': 'shortest',
        'shortest/non-crossing/length': 'shortest non-crossing'
    }
    aggregations = [
        np.median,
        results.err(results.mad)
    ]

    data = data[columns.keys()]
    data = data.agg(aggregations)

    data.index.name = 'number of points'

    #print data

    axes = results.plot_error(data=data, columns=columns, aggregations=aggregations)

    axes.plot(
        data.index, np.sqrt(2.0) / data.index,
        label=r'\( \dfrac{\sqrt{2}}{n} \)'
    )

    def func(n):
        return np.power(np.sqrt(2) * n, (-2.0 / 3.0))

    axes.plot(
        data.index, func(data.index),
        label=r'\( \dfrac{1}{\sqrt[3]{2 \cdot n^2}} \)'
    )

    axes.set_xlabel(data.index.name)
    axes.set_ylabel('segment index')
    results.plot_axes(data=data, axes=axes, log_scale=True)

    results.output(
        'segment_length',
        data=data,
        columns=columns,
        aggregations=aggregations,
        instances=instances
    )


def output_segment_ratio(data, instances):
    print 'output segment ratio...'

    columns = {
        'shortest/ratio': 'shortest non-crossing / shortest'
    }
    aggregations = [
        np.median,
        results.err(results.mad)
    ]

    data = data[columns.keys()]
    data = data.agg(aggregations)

    data.index.name = 'number of points'

    #print data

    axes = results.plot_error(data=data, columns=columns, aggregations=aggregations)

    axes.plot(
        data.index, np.sqrt(data.index) / np.sqrt(6),
        label=r'\( \dfrac{\sqrt{n}}{\sqrt{6}} \)'
    )

    def func(n, a):
        return np.sqrt(n) / np.sqrt(a)

    params = curve_fit(func, data.index.values, data['median'])[0]

    axes.plot(
        data.index, func(data.index, params[0]),
        label=r'\( \dfrac{\sqrt{n}}{\sqrt{' + str(params[0]) + '}} \)'
    )

    axes.set_xlabel(data.index.name)
    axes.set_ylabel('segment length ratio')
    results.plot_axes(data=data, axes=axes, log_scale=True)

    results.output(
        'segment_length',
        data=data,
        columns=columns,
        aggregations=aggregations,
        instances=instances
    )

if __name__ == '__main__':
    if DEBUG:
        data = pd.DataFrame(
            [
                [10, 1, 0.1, 0.2],
                [10, 2, 0.2, 0.4],
                [10, 3, 0.3, 0.6],
                [200, 1, 0.1, 0.2],
                [200, 2, 0.2, 0.4],
                [200, 4, 0.4, 0.8],
                [300, 4, 0.4, 0.8],
                [300, 4, 0.4, 0.8],
                [400, 5, 0.5, 0.1],
                [400, 4, 0.4, 0.8]
            ],
            columns=[
                'file/num_points',
                'shortest/non-crossing/index',
                'shortest/input/length',
                'shortest/non-crossing/length',
                'shortest/contrained_triangulation'
            ]
        )
    else:
        print 'Loading results...'
        data = results.load(
            'file/num_points',
            'file/range',
            'shortest/input/squared_length',
            'shortest/non-crossing/index',
            'shortest/non-crossing/squared_length',
            'shortest/constrained_triangulation/index'
        )

        print 'Normalizing lengths...'
        results.normalize(data, ['shortest/input/squared_length', 'shortest/non-crossing/squared_length'])

    data['shortest/ratio'] = data['shortest/non-crossing/length'] / data['shortest/input/length']

    x_column = 'file/num_points'
    instances = {
        x_val: len(data[data[x_column] == x_val])
        for x_val
        in data[x_column]
    }
    data = data.groupby(x_column)
    output_segment_index(data, instances)
    output_segment_length(data, instances)
    output_segment_ratio(data, instances)
    data = data.obj
