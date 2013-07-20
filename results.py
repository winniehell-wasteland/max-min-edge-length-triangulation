#!/usr/bin/env python2

import json
import pandas
import matplotlib.pyplot as plt
from matplotlib import rc as mpl_rc
import numpy as np


FIG_SIZE = (14, 10)
X_TICK_STEP = 10

# use LaTeX for matplotlib
mpl_rc('text', usetex=True)
mpl_rc('text.latex',
       preamble=r'\usepackage{amsmath}'
                '\usepackage{fourier}'
                '\usepackage{xfrac}'
       )


# error function for a given deviation (e.g. std or mad)
def err(dev):
    c = 2.0

    def func(a):
        return c * dev(a, ddof=1) / np.sqrt(len(a))
    func.__name__ = 'err'

    return func


# remove hierarchical structures from object
def flatten(obj, prefix=''):
    if isinstance(obj, dict):
        flattened_obj = {}

        for key, value in obj.iteritems():
            if isinstance(value, dict):
                flattened_obj.update(flatten(value, prefix + key + '/'))
            else:
                flattened_obj[prefix + key] = value

        return flattened_obj
    elif isinstance(obj, list):
        return list(
            flatten(value)
            for value
            in obj
        )
    else:
        return obj


# load results file into pandas.DataFrame
def load(*columns):
    with open('results.json', 'r') as results_file:
        results = json.load(results_file)

    results = flatten(results)
    results = pandas.concat([pandas.DataFrame(results[i], index=[i]) for i in range(len(results))])

    if len(columns) > 0:
        results = results[[column for column in columns]]
        return results


# median absolute deviation (ddof ignored for now)
def mad(a, ddof):
    return np.median([abs(val - np.median(a)) for val in a])


# transform line segment lengths to unit square
def normalize(data, values):
    for value in values:
        data[value.replace('squared_length', 'length')] = np.sqrt(1.0 * data[value]) / data['file/range']


def output(name, data, columns, aggregations, instances):
    """
    store matplotlib graph as pdf and table as html
    """
    data.columns = pandas.MultiIndex.from_tuples([
        (column, agg.__name__)
        for column
        in columns.itervalues()
        for agg
        in aggregations
    ])

    if instances is not None:
        data[('instances', ' ')] = [instances[i] for i in data.index]

    #plt.show()
    plt.savefig(name + '.pdf', bbox_inches='tight')

    with open(name + '.html', 'w') as html_file:
        html_file.write(data.to_html())


def plot_axes(data, axes, log_scale, x_min=None, y_min=None):
    """
    :type axes: matplotlib.pyplot.Axes
    :type data: pandas.DataFrame
    :type log_scale: bool
    """

    # position legend
    legend = axes.legend(loc='best', prop={'size': 10})
    plt.setp(legend.get_texts(), fontsize='large')

    #x_range = axes.get_xlim()

    if x_min is None:
        x_min = X_TICK_STEP / 2

    x_range = (x_min, max(data.index) + X_TICK_STEP / 2)
    x_ticks = np.arange(X_TICK_STEP * np.ceil(x_range[0] / X_TICK_STEP), x_range[1] + 1, X_TICK_STEP)

    y_range = axes.get_ylim()

    if log_scale:
        axes.set_yscale('log')

    if y_min is None:
        y_min = int(log_scale)

    if log_scale:
        y_range = (10 ** int(np.log10(y_min)), 10 ** int(np.log10(y_range[1]) + 1))
        y_ticks = [10 ** i for i in np.arange(np.log10(y_range[0]), np.log10(y_range[1]) + 1)]
    else:
        y_range = (y_min, int(np.ceil(y_range[1] + 1)))
        y_ticks = np.arange(max(0, y_range[0]), y_range[1] + 1, max(1, (y_range[1] - y_range[0]) / FIG_SIZE[1]))

    axes.set_ylim(y_range)
    axes.set_yticks(y_ticks)

    # mirror y-axes
    ax2 = axes.twinx()

    if log_scale:
        ax2.set_yscale('log')

    ax2.set_xlim(x_range)
    ax2.set_xticks(x_ticks)
    ax2.set_ylim(y_range)
    ax2.set_yticks(y_ticks)


def plot_error(data, columns, aggregations):
    """
    plot columns of data with error bars
    assumes aggregations = [val, err]
    :rtype : matplotlib.pyplot.Axes
    """

    fig = plt.figure(figsize=FIG_SIZE)
    axes = fig.add_subplot(111)

    for key, label in columns.iteritems():
        column = data[key]
        min_err = np.minimum(column[aggregations[1].__name__], column[aggregations[0].__name__] - 1)
        axes.errorbar(
            column.index, column[aggregations[0].__name__],
            yerr=[min_err, column[aggregations[1].__name__]],
            label=label,
            marker='o',
            linestyle=' '
        )

    return axes
