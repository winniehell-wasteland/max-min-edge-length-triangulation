import json
import math
import random

class PointGenerator(object):
    def check_point(self, points, point):
        return point not in points

    def generate(self, num_points, file_name):
        points = set()

        while len(points) < num_points:
            points.add(self.generate_point())

        with open(file_name, 'w') as json_file:
            json.dump(list(points), json_file)

    @property
    def parameters(self):
        return self._parameters


class RectanglePointGenerator(PointGenerator):
    def __init__(self, x_range, y_range):
        self._parameters = {
            'x_range' : x_range,
            'y_range' : y_range
        }

    def __str__(self):
        return 'random points out of rectangle {{{x_range[0]},...,{x_range[1]}}}x{{{y_range[0]},...,{y_range[1]}}}'\
        .format(**self.parameters)

    @classmethod
    def parameter_formats(cls):
        return '{x_range[0]:04}','{x_range[1]:04}','{y_range[0]:04}','{y_range[1]:04}'

    def generate_point(self):
        return random.randint(*self._parameters['x_range']), random.randint(*self._parameters['y_range'])

class SquarePointGenerator(RectanglePointGenerator):
    def __init__(self, coordinate_range):
        super(SquarePointGenerator, self).__init__(x_range=coordinate_range, y_range=coordinate_range)

    def __str__(self):
        return 'random points out of square {{{x_range[0]},...,{x_range[1]}}}^2'\
        .format(**self.parameters)

    @classmethod
    def parameter_formats(cls):
        return '{x_range[0]:04}','{x_range[1]:04}'

class CirclePointGenerator(RectanglePointGenerator):
    def __init__(self, radius, center=None):
        if not center:
            center = (radius, radius)

        super(CirclePointGenerator, self).__init__(
            x_range=(center[0] - radius, center[0] + radius),
            y_range=(center[1] - radius, center[1] + radius)
        )

        self._parameters = {
            'center' : center,
            'radius' : radius
        }

    def __str__(self):
        return 'random points out of circle centered at ({center[0]},,{center[1]}) with radius {radius}'\
        .format(**self.parameters)

    @classmethod
    def parameter_formats(cls):
        return '{center[0]:04}','{center[1]:04}','{radius[0]:04}'

    def generate_point(self):
        while True:
            point = super(CirclePointGenerator, self).generate_point()

            if math.hypot(point[0] - self._parameters['center'][0],
                          point[1] - self._parameters['center'][1]) <= self._parameters['radius']:
                return point
