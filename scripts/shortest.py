#!/usr/bin/env python2

#import matplotlib as mpl
from matplotlib import rc as mpl_rc
import numpy as np

import mmlt_utils

mpl_rc('text', usetex=True)
mpl_rc('text.latex',
       preamble=r'\usepackage{amsmath}' \
                '\usepackage{fourier}'
                '\usepackage{xfrac}'
)

data = mmlt_utils.load_results()

data = data[[
    'file/num_points',
    'file/range',
    'shortest/input/squared_length',
    'shortest/non-intersecting/index',
    'shortest/non-intersecting/squared_length'
]]

# normalize
for length in ['shortest/input/squared_length', 'shortest/non-intersecting/squared_length']:
    data[length.replace('squared_length', 'length')] = np.sqrt(1.0 * data[length]) / data['file/range']

data['ratio'] = data['shortest/non-intersecting/length'] / data['shortest/input/length']

data = data.groupby('file/num_points')

mmlt_utils.output(
    data=data,
    name='shortest segment index',
    columns={
        'shortest/non-intersecting/index': 'shortest non-crossing'
    },
    aggregations=[
        {
            'name': 'avg',
            'func': np.mean,
            'comparisons': {
                '\( 0.1n \)': lambda n: 0.1 * n
            }
        },
        {
            'name': 'min',
            'func': np.min,
            'comparisons': {
            }
        },
        {
            'name': 'max',
            'func': np.max,
            'comparisons': {
                '\( \dfrac{3}{4}n \)': lambda n: 3 * n / 4.0,
                '\( n \)': lambda n: n,
                '\( 2n \)': lambda n: 2 * n
            }
        }
    ]
)

mmlt_utils.output(
    data=data,
    name='shortest segment length',
    columns={
        'shortest/input/length': 'shortest',
        'shortest/non-intersecting/length': 'shortest non-crossing'
    },
    aggregations=[
        {
            'name': 'avg',
            'func': np.mean,
            'comparisons': {
                r'\( \dfrac{1}{\sqrt{5n}} \)': lambda n: 1.0 / np.sqrt(5.0*n),
                r'\( \dfrac{\sqrt{2}}{n} \)': lambda n: np.sqrt(2.0) / (1.0 * n)
            }
        },
        {
            'name': 'min',
            'func': np.min,
            'comparisons': {
                r'\( \dfrac{1}{10^2\sqrt{n}} \)': lambda n: 1.0 / (100.0 * np.sqrt(1.0 * n))
            }
        },
        {
            'name': 'max',
            'func': np.max,
            'comparisons': {
                r'\( \dfrac{2}{\sqrt{n}} \)': lambda n: 2.0 / np.sqrt(1.0 * n),
                r'\( \dfrac{\sqrt{2}}{\sqrt{n}} \)': lambda n: np.sqrt(2.0) / np.sqrt(1.0 * n),
                r'\( \dfrac{4}{n} \)': lambda n: 4.0 / (1.0 * n)
            }
        }
    ],
    y_scale='log'
)

mmlt_utils.output(
    data=data,
    name='shortest segment length ratio',
    columns={
        'ratio': 'shortest non-crossing/shortest'
    },
    aggregations=[
        {
            'name': 'avg',
            'func': np.mean,
            'comparisons': {
                r'\( \dfrac{\sqrt{n}}{\sqrt{6}} \)': lambda n: np.sqrt(1.0 * n) / np.sqrt(6.0)
            }
        },
        {
            'name': 'min',
            'func': np.min,
            'comparisons': {
            }
        },
        {
            'name': 'max',
            'func': np.max,
            'comparisons': {
                r'\( 5\sqrt{n} \)': lambda n: 5.0 * np.sqrt(1.0 * n)
            }
        }
    ],
    y_scale='log'
)
