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
    print 1/0
else:
    print 'Loading results...'
    data = results.load(
        'file/num_points',
        'file/range',
        'time/total',
        'time/SAT_solving',
        'time/generating_segments'
    )

# filter out non-positive times
#for key in ['time/total', 'time/SAT_solving', 'time/generating_segments']:
#    data[key] = data[key][data[key]>0]

data = data.groupby('file/num_points')
instances = data.shape.apply(lambda x: x[0])


def output_time_comparison(data):
    print 'Generating time comparison...'

    columns = {
        'time/total': 'total running time',
        'time/SAT_solving': 'time for SAT solving',
        'time/generating_segments': 'time for segment generation'
        }

    data = data[columns.keys()]
    data = data.agg([np.mean, results.err])

    #print data

    fig = plt.figure(figsize=FIG_SIZE)

    ax = fig.add_subplot(111)
    ax.set_xlabel('number of points')
    ax.set_ylabel('time in milliseconds')

    for key, label in columns.iteritems():
        column = data[key]
        min_err = np.minimum(column['err'], column['mean']-1)
        plt.errorbar(
            column.index, column['mean'],
            yerr=[min_err,column['err']],
            label=label,
            marker='o',
            linestyle=' '
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
        ('total', 'average'),
        ('total', 'standard error'),
        ('SAT solving', 'average'),
        ('SAT solving', 'standard error'),
        ('segment generation', 'average'),
        ('segment generation', 'standard error')
    ])
    data.index.name = 'points'
    data[('instances', ' ')] = instances

    results.output('time_comparison', data)

def output_time_total(data):
    print 'Generating total time...'

    data = data['time/total']

    data = data.agg([np.mean, results.err])

    #print data

    fig = plt.figure(figsize=FIG_SIZE)

    ax = fig.add_subplot(111)
    ax.set_xlabel('number of points')
    ax.set_ylabel('time in milliseconds')

    min_err= np.minimum(data['err'], data['mean']-1)
    plt.errorbar(
        data.index, data['mean'],
        yerr=[min_err, data['err']],
        label='total running time',
        marker='o',
        linestyle=' '
    )

    def func(n, a, b, c):
        return a * np.power(n, b) + c

    params = curve_fit(func, data.index.values, data['mean'])[0]

    plt.plot(
        data.index, func(data.index, params[0], params[1], params[2]),
        label=r'\( ' + str(params[0]) + ' \cdot n^{' + str(params[1]) + '} + ' + str(params[2]) + '\)'
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
        ('time', 'average'),
        ('time', 'standard error')
    ])
    data.index.name = 'points'
    data[('instances', ' ')] = instances

    results.output('time_total', data)

output_time_comparison(data)
output_time_total(data)
