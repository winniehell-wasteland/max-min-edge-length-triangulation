import argparse
import datetime
import re
import subprocess
import sys

BASE_FILE_NAME = '{dir}/{name}_{num_points:03}_{parameters}_{iteration:02}'

DATA_LINE = '\t'.join(('{'+column+'}' for column in ('num_points',
                                                     'intersections_avg',
                                                     'intersections_min',
                                                     'intersections_max',
                                                     'overlaps_avg',
                                                     'overlaps_min',
                                                     'overlaps_max',
                                                     'time_avg',
                                                     'time_min',
                                                     'time_max'))) + '\n'

NUM_ITERATIONS = 10

RE_TIME_STAMP = re.compile(r'\[(?P<hour>\d{2}):(?P<minute>\d{2}):(?P<second>\d{2}).(?P<millisecond>\d{3})\]')
RE_STAT_LINE  = re.compile(RE_TIME_STAMP.pattern
                           + r' \[\] segments: total=\d+'\
                           + r' intersections=(?P<intersections>\d+)'
                           + r' overlaps=(?P<overlaps>\d+)\n')

class TestClass(object):
	def __init__(self, name, num_point_values, point_generators):
		arg_parser = argparse.ArgumentParser(description='Run {name} tests.'.format(name=name))
		arg_parser.add_argument('executable', metavar='EXECUTABLE', type=str, help='max-min-triangulation executable')
		arg_parser.add_argument('--output_dir', metavar='OUTPUT_DIRECTORY', type=str, default='.', help='output directory')
		arg_parser.parse_args(namespace=self)

		self.name             = name
		self.num_point_values = num_point_values
		self.point_generators = point_generators

	@classmethod
	def parse_timestamp(cls, line):
		values = RE_TIME_STAMP.match(line).groupdict()

		for key in values:
			values[key] = int(values[key])

		values['microsecond'] = values.pop('millisecond')*1000

		return datetime.datetime(1970, 1, 1, **values)

	def data_file_name(self, point_generator):
		return '{name}_{parameters}.dat'.format(
			name=self.name,
			parameters='_'.join(point_generator.parameter_formats()).format(**point_generator.parameters)
		)

	def run(self):
		for point_generator in self.point_generators:
			with open(self.output_dir + '/' + self.data_file_name(point_generator), 'w') as data_file:

				data_file.write('# '+DATA_LINE)

				for num_points in self.num_point_values:
					results = {
						'intersections' : [],
						'overlaps' : [],
						'time' : []
					}

					for iteration in range(NUM_ITERATIONS):
						self.run_iteration(
							results=results,
							iteration=iteration,
							num_points=num_points,
							point_generator=point_generator
						)

					for key in ('intersections', 'overlaps', 'time'):
						results[key+'_min'] = min(results[key])
						results[key+'_max'] = max(results[key])
						results[key+'_avg'] = sum(results[key])/len(results[key])

					data_file.write(DATA_LINE.format(num_points=num_points, **results))
					data_file.flush()

		self.generate_gnuplot_file()

	def run_iteration(self, results, iteration, num_points, point_generator):
		base_file_name = BASE_FILE_NAME.format(
			dir=self.output_dir,
			name=self.name,
			num_points=num_points,
			parameters='_'.join(point_generator.parameter_formats()).format(**point_generator.parameters),
			iteration=iteration
		)

		print 'Generating input file {file_name}.json...'.format(file_name=base_file_name)
		point_generator.generate(num_points=num_points, file_name=base_file_name + '.json')


		print 'Running {executable} with file {file_name}.json...'.format(
			executable=self.executable,
			file_name=base_file_name
		)

		with open(base_file_name + '.log', 'w') as log_file:
			if subprocess.call([self.executable, base_file_name + '.json'], stdout=log_file, stderr=log_file) != 0:
				log_file.close()
				sys.exit('Failed!')

		with open(base_file_name + '.log', 'r') as log_file:
			for line in log_file:
				last_line = '[00:00:00.000]'

				match = RE_STAT_LINE.match(line)
				if match:
					results['intersections'].append(int(match.group('intersections')))
					results['overlaps'].append(int(match.group('overlaps')))

					start_time = self.parse_timestamp(last_line)
					end_time = self.parse_timestamp(line)
					results['time'].append((end_time - start_time).total_seconds())
					break

				if line != '\n':
					last_line = line

	def generate_gnuplot_file(self):

		with open('{dir}/{name}.gp'.format(dir=self.output_dir, name=self.name), 'w') as gnuplot_file:
			gnuplot_file.write('''
set terminal pdf size 30cm, 21cm

set key top left
set logscale y
set xrange [4:]
set xlabel 'Number of input points'
''')

			gnuplot_file.write('''
set output "{name}_intersections.pdf"
set ylabel "Number of intersecting segments"

f(x) = x*(x-1)*(x-2)*(x-3)/18

plot \\
\tf(x) title "n*(n-1)*(n-2)*(n-3)/18", \\
'''.format(name=self.name))

			gnuplot_file.write(', \\\n'.join((
				'\t"{file_name}" using 1:2:3:4 with yerrorlines title "{title}"'.format(
					file_name=self.data_file_name(point_generator),
					title=str(point_generator)
				) for point_generator in self.point_generators
			)))

			gnuplot_file.write('''

unset logscale y
set output "{name}_overlaps.pdf"
set ylabel "Number of overlapping segments"

plot \\
'''.format(name=self.name))

			gnuplot_file.write(', \\\n'.join((
				'\t"{file_name}" using 1:5:6:7 with yerrorlines title "{title}"'.format(
					file_name=self.data_file_name(point_generator),
					title=str(point_generator)
				) for point_generator in self.point_generators
			)))

			gnuplot_file.write('''

set logscale y
set output "{name}_times.pdf"
set ylabel "Execution time (in seconds)"

plot \\
'''.format(name=self.name))

			gnuplot_file.write(', \\\n'.join((
				'\t"{file_name}" using 1:8:9:10 with yerrorlines title "{title}"'.format(
					file_name=self.data_file_name(point_generator),
					title=str(point_generator)
				) for point_generator in self.point_generators
			)))

			gnuplot_file.write('\n')
