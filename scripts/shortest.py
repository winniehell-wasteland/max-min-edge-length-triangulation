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
    data[length] /= 100.0 * data['file/range']

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
                r'\( \dfrac{1}{3\cdot \sqrt{n}} \)': lambda n: 1.0 / (3 * np.sqrt(n)),
                r'\( \dfrac{1}{n \cdot \log(n)} \)': lambda n: 1.0 / (n * np.log(n))
            }
        },
        {
            'name': 'min',
            'func': np.min,
            'comparisons': {
                r'\( \dfrac{1}{10^5\sqrt{n}} \)': lambda n: 1.0 / (10000 * np.sqrt(n))
            }
        },
        {
            'name': 'max',
            'func': np.max,
            'comparisons': {
                r'\( \dfrac{1}{\sqrt{n}} \)': lambda n: 1.0 / np.sqrt(n),
                r'\( \dfrac{2}{n \cdot \log(n)} \)': lambda n: 2.0 / (n * np.log(n))
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
                r'\( \dfrac{n\cdot\log(n)}{\sqrt{n}} \)': lambda n: n * np.log(n) / (1.0 * np.sqrt(n))
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
                r'\( \dfrac{30n\cdot\log(n)}{\sqrt{n}} \)': lambda n: 30 * n * np.log(n) / (1.0 * np.sqrt(n))
            }
        }
    ],
    y_scale='log'
)
