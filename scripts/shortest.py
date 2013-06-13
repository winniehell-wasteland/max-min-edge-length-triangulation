#!/usr/bin/env python2

import math
import numpy as np

import pandas as pd
import results
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

DEBUG = False
#DEBUG = True
FIG_SIZE = (20, 10)

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
        columns=['file/num_points', 'shortest/non-intersecting/index',
                 'shortest/input/length', 'shortest/non-intersecting/length']
    )
else:
    print 'Loading results...'
    data = results.load(
        'file/num_points',
        'file/range',
        'shortest/input/squared_length',
        'shortest/non-intersecting/index',
        'shortest/non-intersecting/squared_length'
    )

    print 'Normalizing lengths...'
    results.normalize(data, ['shortest/input/squared_length', 'shortest/non-intersecting/squared_length'])

data['shortest/ratio'] = data['shortest/non-intersecting/length'] / data['shortest/input/length']

data = data.groupby('file/num_points')
instances = data.shape.apply(lambda x: x[0])


def output_segment_index(data):
    data = data['shortest/non-intersecting/index']

    data = data.agg([np.mean, results.err])

    #print data

    fig = plt.figure(figsize=FIG_SIZE)

    ax = fig.add_subplot(111)
    ax.set_xlabel('number of points')
    ax.set_ylabel('segment index')

    plt.errorbar(
        data.index, data['mean'],
        yerr=[data['err'], data['err']],
        label='shortest non-crossing segment index',
        marker='o',
        linestyle=' '
    )

    slope, intercept = results.fit(data)
    plt.plot(
        data.index, slope * data.index + intercept,
        label=r'linear fit (\({0!s}n {1:+f}\))'.format(slope, intercept)
    )

    #x_range = ax.get_xlim()
    x_range = (0, max(data.index) + 1)
    ax.set_xlim(x_range)

    y_range = ax.get_ylim()
    y_range = (0, int(math.ceil(y_range[1] + 1)))
    ax.set_ylim(y_range)

    ax.set_xticks(
        np.arange(max(0, x_range[0]), x_range[1] + 1, max(1, (x_range[1] - x_range[0]) / FIG_SIZE[0]))
    )

    ax.set_yticks(
        np.arange(max(0, y_range[0]), y_range[1] + 1, max(1, (y_range[1] - y_range[0]) / FIG_SIZE[1]))
    )

    legend = plt.legend(loc='best', prop={'size': 10})
    plt.setp(legend.get_texts(), fontsize='large')

    data.columns = pd.MultiIndex.from_tuples([
        ('index', 'average'),
        ('index', 'standard error')
    ])
    data.index.name = 'points'
    data[('instances', ' ')] = instances

    results.output('shortest_index', data)


def output_segment_length(data):
    data = data['shortest/input/length', 'shortest/non-intersecting/length']
    #data.columns = ['shortest', 'shortest non-crossing']

    data = data.agg([np.mean, results.err])

    #print data

    fig = plt.figure(figsize=FIG_SIZE)

    ax = fig.add_subplot(111)
    ax.set_xlabel('number of points')
    ax.set_ylabel('segment length')

    ax.set_yscale('log')

    plt.errorbar(
        data.index, data['shortest/input/length']['mean'],
        yerr=[data['shortest/input/length']['err'], data['shortest/input/length']['err']],
        label='shortest',
        marker='o',
        linestyle=' '
    )

    plt.plot(
        data.index, np.sqrt(2.0) / data.index,
        label=r'\( \dfrac{\sqrt{2}}{n} \)'
    )

    def func(n, a):
        return np.sqrt(a) / n

    params = curve_fit(func, data.index.values, data['shortest/input/length']['mean'])[0]

    plt.plot(
        data.index, func(data.index, params[0]),
        label=r'\( \dfrac{\sqrt{' + str(params[0]) + '}}{n} \)'
    )

    plt.errorbar(
        data.index, data['shortest/non-intersecting/length']['mean'],
        yerr=[data['shortest/non-intersecting/length']['err'], data['shortest/non-intersecting/length']['err']],
        label='shortest non-crossing',
        marker='o',
        linestyle=' '
    )

    plt.plot(
        data.index, 1.0 / np.sqrt(5.0 * data.index),
        label=r'\( \dfrac{1}{\sqrt{5n}} \)'
    )

    def func(n, a):
        return 1.0 / np.sqrt(a * n)

    params = curve_fit(func, data.index.values, data['shortest/non-intersecting/length']['mean'])[0]

    plt.plot(
        data.index, func(data.index, params[0]),
        label=r'\( \dfrac{1}{\sqrt{' + str(params[0]) + 'n}} \)'
    )

    #x_range = ax.get_xlim()
    x_range = (0, max(data.index) + 1)
    ax.set_xlim(x_range)

    ax.set_xticks(
        np.arange(max(0, x_range[0]), x_range[1] + 1, max(1, (x_range[1] - x_range[0]) / FIG_SIZE[0]))
    )

    #y_range = ax.get_ylim()
    #y_range = (0.001, 1)
    #y_range = (0, int(math.ceil(y_range[1] + 1)))
    #ax.set_ylim(y_range)

    #ax.set_yticks(
    #    np.arange(y_range[0], y_range[1], (y_range[1] - y_range[0]) / FIG_SIZE[1])
    #)

    legend = plt.legend(loc='best', prop={'size': 10})
    plt.setp(legend.get_texts(), fontsize='large')

    data.columns = pd.MultiIndex.from_tuples([
        ('shortest', 'average'),
        ('shortest', 'standard error'),
        ('shortest non-crossing', 'average'),
        ('shortest non-crossing', 'standard error')
    ])
    data.index.name = 'points'
    data[('instances', ' ')] = instances

    results.output('shortest_length', data)


def output_segment_ratio(data):
    data = data['shortest/ratio']

    data = data.agg([np.mean, results.err])

    #print data

    fig = plt.figure(figsize=FIG_SIZE)

    ax = fig.add_subplot(111)
    ax.set_xlabel('number of points')
    ax.set_ylabel('segment length ratio')

    ax.set_yscale('log')

    plt.errorbar(
        data.index, data['mean'],
        yerr=[data['err'], data['err']],
        label='shortest non-crossing / shortest',
        marker='o',
        linestyle=' '
    )

    plt.plot(
        data.index, np.sqrt(data.index) / np.sqrt(6),
        label=r'\( \dfrac{\sqrt{n}}{\sqrt{6}} \)'
    )

    def func(n, a):
        return np.sqrt(n) / np.sqrt(a)

    params = curve_fit(func, data.index.values, data['mean'])[0]

    plt.plot(
        data.index, func(data.index, params[0]),
        label=r'\( \dfrac{\sqrt{n}}{\sqrt{' + str(params[0]) + '}} \)'
    )

    #x_range = ax.get_xlim()
    x_range = (0, max(data.index) + 1)
    ax.set_xlim(x_range)

    ax.set_xticks(
        np.arange(max(0, x_range[0]), x_range[1] + 1, max(1, (x_range[1] - x_range[0]) / FIG_SIZE[0]))
    )

    #y_range = ax.get_ylim()
    #y_range = (0.001, 1)
    #y_range = (0, int(math.ceil(y_range[1] + 1)))
    #ax.set_ylim(y_range)

    #ax.set_yticks(
    #    np.arange(y_range[0], y_range[1], (y_range[1] - y_range[0]) / FIG_SIZE[1])
    #)

    legend = plt.legend(loc='best', prop={'size': 10})
    plt.setp(legend.get_texts(), fontsize='large')

    data.columns = pd.MultiIndex.from_tuples([
        ('ratio', 'average'),
        ('ratio', 'standard error')
    ])
    data.index.name = 'points'
    data[('instances', ' ')] = instances

    results.output('shortest_ratio', data)

output_segment_index(data)
output_segment_length(data)
output_segment_ratio(data)
