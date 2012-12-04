#!/usr/bin/env python2

from generators import SquarePointGenerator
from test_class import TestClass

tests = TestClass(
    name='random_square',
    num_point_values=range(4,20,1), #+ range(20,40,5) + range(40,81,10),
    point_generators=[SquarePointGenerator(coordinate_range=(0,max)) for max in [50, 500, 5000]]
)

tests.run()
