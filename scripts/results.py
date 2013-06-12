#!/usr/bin/env python2

import json
import pandas
import matplotlib.pyplot as plt
import numpy as np


from matplotlib import rc as mpl_rc

mpl_rc('text', usetex=True)
mpl_rc('text.latex',
    preamble=r'\usepackage{amsmath}'
             '\usepackage{fourier}'
             '\usepackage{xfrac}'
)


def err(x):
    return 2.0 * np.std(x, ddof=1) / np.sqrt(len(x))


def fit(data):
    A = np.vstack([data.index, np.ones(len(data.index))]).T
    return np.linalg.lstsq(A, data['mean'])[0]


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


def load(*args):
    with open('results.json', 'r') as results_file:
        results = json.load(results_file)

    results = flatten(results)
    results = pandas.concat([pandas.DataFrame(results[i], index=[i]) for i in range(len(results))])

    if len(args) > 0:
        results = results[[arg for arg in args]]
        return results


def normalize(data, values):
    for value in values:
        data[value.replace('squared_length', 'length')] = np.sqrt(1.0 * data[value]) / data['file/range']


def output_(data, name, columns, aggregations, y_scale=None, legend_loc='best'):
    print 'Generating ' + name + '...'

    # plot graph
    for aggregation in aggregations:
        graph_data = data[columns.keys()]
        graph_data = graph_data.agg(aggregation['func'])

        graph_data.columns = list(
            key + ' (' + aggregation['name'] + ')'
            for key
            in columns.values()
        )

        for func in aggregation['comparisons'].iteritems():
            graph_data[func[0]] = func[1](graph_data.index)

        fig_size = (20, 10)
        fig = graph_data.plot(figsize=fig_size)

        x_range = fig.get_xlim()
        y_range = fig.get_ylim()

        if (y_scale == 'log') and (x_range[0] <= 0):
            x_range = (1, x_range[1])
            fig.set_xlim(x_range)

        if (0 < y_range[0]) and (y_range[0] < 1):
            y_range = (0, y_range[1])
            fig.set_ylim(y_range)

        fig.set_xlabel('number of points')
        fig.set_ylabel(name)

        if y_scale is None:
            fig.set_xticks(
                np.arange(max(0, x_range[0]), x_range[1] + 1, max(1, (x_range[1] - x_range[0]) / fig_size[0])))
            fig.set_yticks(
                np.arange(max(0, y_range[0]), y_range[1] + 1, max(1, (y_range[1] - y_range[0]) / fig_size[1])))
        else:
            fig.set_yscale(y_scale)

        legend = plt.legend(loc=legend_loc, prop={'size': 10})
        plt.setp(legend.get_texts(), fontsize='large')

        #plt.show()
        plt.savefig(name.replace(' ', '_') + '_' + aggregation['name'] + '.pdf', bbox_inches='tight')

    # write HTML file
    html_data = data[columns.keys()]
    html_data = html_data.agg([aggregation['func'] for aggregation in aggregations])

    html_data.columns = list(
        key + ' (' + aggregation['name'] + ')'
        for key
        in columns.values()
        for aggregation
        in aggregations
    )

    html_data['instances'] = data.size()

    with open(name.replace(' ', '_') + '.html', 'w') as html_file:
        html_file.write(html_data.to_html())


def output(name, data):
    #plt.show()
    plt.savefig(name + '.pdf', bbox_inches='tight')

    with open(name + '.html', 'w') as html_file:
        html_file.write(data.to_html())
