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
X_TICK_STEP = 10

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
        columns=['file/num_points', 'shortest/non-crossing/index',
                 'shortest/input/length', 'shortest/non-crossing/length',
                 'shortest/contrained_triangulation']
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

data = data.groupby('file/num_points')
instances = data.shape.apply(lambda x: x[0])


def output_segment_index(data):
    print 'output segment index...'

    data = data['shortest/non-crossing/index', 'shortest/constrained_triangulation/index']

    data = data.agg([np.mean, results.err])

    #print data

    fig = plt.figure(figsize=FIG_SIZE)

    ax = fig.add_subplot(111)
    ax.set_xlabel('number of points')
    ax.set_ylabel('segment index')

    plt.errorbar(
        data.index, data['shortest/non-crossing/index']['mean'],
        yerr=[data['shortest/non-crossing/index']['err'], data['shortest/non-crossing/index']['err']],
        label='shortest non-crossing segment index',
        marker='o',
        linestyle=' '
    )

    plt.errorbar(
        data.index, data['shortest/constrained_triangulation/index']['mean'],
        yerr=[data['shortest/constrained_triangulation/index']['err'], data['shortest/constrained_triangulation/index']['err']],
        label='optimal segment index',
        marker='o',
        linestyle=' '
    )

    def func(n, a, b, c):
        return a * n ** b + c

    def linear_func(n, a, b):
        return a * n + b

    def rt4_func(n, a, b):
        return a * n ** 0.25 + b

    params = curve_fit(rt4_func, data.index.values, data['shortest/non-crossing/index']['mean'])[0]
    plt.plot(
        data.index, rt4_func(data.index, params[0], params[1]),
        label=r'\( ' + str(params[0]) + '\cdot n^{0.25} + ' + str(params[1]) + '\)'
    )

    params = curve_fit(func, data.index.values, data['shortest/constrained_triangulation/index']['mean'])[0]
    plt.plot(
        data.index, func(data.index, params[0], params[1], params[2]),
        label=r'\( ' + str(params[0]) + '\cdot n^{' + str(params[1]) + '} + ' + str(params[2]) + '\)'
    )

    legend = plt.legend(loc='best', prop={'size': 10})
    plt.setp(legend.get_texts(), fontsize='large')

    #x_range = ax.get_xlim()                                                                                                                                         
    x_range = (X_TICK_STEP / 2, max(data.index) + X_TICK_STEP / 2)
    x_ticks = np.arange(max(X_TICK_STEP, x_range[0]), x_range[1] + 1, X_TICK_STEP)

    y_range = ax.get_ylim()
    y_range = (1, 10**int(np.log10(y_range[1])+1))
    y_ticks = np.arange(max(1, y_range[0]), y_range[1] + 1, max(1, (y_range[1] - y_range[0]) / FIG_SIZE[1]))

    ax.set_yscale('log')
    ax.set_ylim(y_range)
    #ax.set_yticks(y_ticks)                                                                                                                                           
    ax2 = ax.twinx()
    ax2.set_yscale('log')
    ax2.set_xlim(x_range)
    ax2.set_xticks(x_ticks)
    ax2.set_ylim(y_range)
    #ax2.set_yticks(y_ticks)

    data.columns = pd.MultiIndex.from_tuples([
        ('shortest non-crossing index', 'average'),
        ('shortest non-crossing index', 'standard error'),
        ('optimal index', 'average'),
        ('optimal index', 'standard error')
    ])
    data.index.name = 'points'
    data[('instances', ' ')] = instances

    results.output('segment_index', data)


def output_segment_length(data):
    print 'output segment length...'

    data = data['shortest/input/length', 'shortest/non-crossing/length']
    #data.columns = ['shortest', 'shortest non-crossing']

    data = data.agg([np.mean, results.err])

    #print data

    fig = plt.figure(figsize=FIG_SIZE)

    ax = fig.add_subplot(111)
    ax.set_xlabel('number of points')
    ax.set_ylabel('segment length')

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

    plt.errorbar(
        data.index, data['shortest/non-crossing/length']['mean'],
        yerr=[data['shortest/non-crossing/length']['err'], data['shortest/non-crossing/length']['err']],
        label='shortest non-crossing',
        marker='o',
        linestyle=' '
    )

    def func(n):
        return np.power(np.sqrt(2) * n, (-2.0 / 3.0))

    plt.plot(
        data.index, func(data.index),
        label=r'\( \dfrac{1}{\sqrt[3]{2 \cdot n^2}} \)'
    )

    legend = plt.legend(loc='best', prop={'size': 10})
    plt.setp(legend.get_texts(), fontsize='large')

    #x_range = ax.get_xlim()                                                                                                                                         
    x_range = (X_TICK_STEP / 2, max(data.index) + X_TICK_STEP / 2)
    x_ticks = np.arange(max(X_TICK_STEP, x_range[0]), x_range[1] + 1, X_TICK_STEP)

    y_range = ax.get_ylim()
    y_range = (y_range[0], 10**int(np.log10(y_range[1])+1))
    y_ticks = np.arange(max(1, y_range[0]), y_range[1] + 1, max(1, (y_range[1] - y_range[0]) / FIG_SIZE[1]))

    ax.set_yscale('log')
    ax.set_ylim(y_range)
    #ax.set_yticks(y_ticks)                                                                                                                                           
    ax2 = ax.twinx()
    ax2.set_yscale('log')
    ax2.set_xlim(x_range)
    ax2.set_xticks(x_ticks)
    ax2.set_ylim(y_range)
    #ax2.set_yticks(y_ticks)  

    data.columns = pd.MultiIndex.from_tuples([
        ('shortest', 'average'),
        ('shortest', 'standard error'),
        ('shortest non-crossing', 'average'),
        ('shortest non-crossing', 'standard error')
    ])
    data.index.name = 'points'
    data[('instances', ' ')] = instances

    results.output('segment_length', data)


def output_segment_ratio(data):
    print 'output segment ratio...'

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

    legend = plt.legend(loc='best', prop={'size': 10})
    plt.setp(legend.get_texts(), fontsize='large')

    #x_range = ax.get_xlim()                                                                                                                                         
    x_range = (X_TICK_STEP / 2, max(data.index) + X_TICK_STEP / 2)
    x_ticks = np.arange(max(X_TICK_STEP, x_range[0]), x_range[1] + 1, X_TICK_STEP)

    y_range = ax.get_ylim()
    y_range = (1, int(math.ceil(y_range[1] + 1)))
    y_ticks = np.arange(max(1, y_range[0]), y_range[1] + 1, max(1, (y_range[1] - y_range[0]) / FIG_SIZE[1]))

    ax.set_yscale('log')
    ax.set_ylim(y_range)
    #ax.set_yticks(y_ticks)

    ax2 = ax.twinx()
    ax2.set_yscale('log')
    ax2.set_xlim(x_range)
    ax2.set_xticks(x_ticks)
    ax2.set_ylim(y_range)
    #ax2.set_yticks(y_ticks)

    data.columns = pd.MultiIndex.from_tuples([
        ('ratio', 'average'),
        ('ratio', 'standard error')
    ])
    data.index.name = 'points'
    data[('instances', ' ')] = instances

    results.output('segment_ratio', data)

output_segment_index(data)
output_segment_length(data)
output_segment_ratio(data)
