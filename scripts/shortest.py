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
    data[length] /= 1.0 * data['file/range']

data['ratio'] = data['shortest/non-intersecting/squared_length'] / data['shortest/input/squared_length']

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
                '6/100*n': lambda n: 6 * n / 100.0,
                '10/100*n': lambda n: 10 * n / 100.0,
                '13/100*n': lambda n: 13 * n / 100.0
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
                '7/20*n': lambda n: 7 * n / 20.0,
                '14/20*n': lambda n: 14 * n / 20.0,
                '28/20*n': lambda n: 28 * n / 20.0
            }
        }
    ]
)

mmlt_utils.output(
    data=data,
    name='shortest segment length',
    columns={
        'shortest/input/squared_length': 'shortest',
        'shortest/non-intersecting/squared_length': 'shortest non-crossing'
    },
    aggregations=[
        {
            'name': 'avg',
            'func': np.mean,
            'comparisons': {
                r'\( \dfrac{10^2}{5\cdot \sqrt{n}} \)': lambda n: 100.0 / (5 * np.sqrt(n)),
                r'\( \dfrac{2*10^2}{n \cdot \sqrt{n}} \)': lambda n: 200.0 / (n * np.sqrt(n)),
                r'\( \dfrac{20}{n} \)': lambda n: 20.0 / (1 * n)
            }
        },
        {
            'name': 'min',
            'func': np.min,
            'comparisons': {
                r'\( \dfrac{1}{10^2\sqrt{n}} \)': lambda n: 1.0 / (100 * np.sqrt(n))
            }
        },
        {
            'name': 'max',
            'func': np.max,
            'comparisons': {
                r'\( \dfrac{10^2}{\sqrt{n}} \)': lambda n: 100.0 / np.sqrt(n),
                r'\( \dfrac{7*10^2}{n \cdot \sqrt{n}} \)': lambda n: 700.0 / (n * np.sqrt(n)),
                r'\( \dfrac{70}{n} \)': lambda n: 70.0 / (1 * n)
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
                r'\( 0.2n \)': lambda n: 0.2 * n,
                r'\( 0.5n \)': lambda n: 0.5 * n,
                r'\( 1.1n \)': lambda n: 1.1 * n
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
                r'\( 2.0n \)': lambda n: 2.0 * n,
                r'\( 7.0n \)': lambda n: 7.0 * n,
                r'\( 70.0n \)': lambda n: 70.0 * n
            }
        }
    ],
    y_scale='log'
)
