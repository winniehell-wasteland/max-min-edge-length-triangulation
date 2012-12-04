#!/usr/bin/env python2

from generators import CirclePointGenerator
from test_class import TestClass

tests = TestClass(
	name='random_circle',
	num_point_values=range(4,20,1) + range(20,40,5) + range(40,81,10),
	point_generators=[CirclePointGenerator(radius=radius) for radius in [50, 5000]]
)

tests.run()
