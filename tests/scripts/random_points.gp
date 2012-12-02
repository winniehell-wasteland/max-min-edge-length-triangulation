set terminal pdf size 30cm, 21cm

set key top left
set logscale y
set xrange [4:]
set xlabel 'Number of input points'

set output "random_intersections.pdf"
set ylabel 'Number of intersecting segments'

plot \
  "random.dat" using 1:(($1/2)**4) with lines title '(n/2)^4', \
  "<(grep -P '^[^\t]+\t0\t50\t' random.dat)" using 1:4:5:6 with yerrorlines title 'point range {0,...,50}', \
  "<(grep -P '^[^\t]+\t0\t500\t' random.dat)" using 1:4:5:6 with yerrorlines title 'point range {0,...,500}', \
  "<(grep -P '^[^\t]+\t0\t5000\t' random.dat)" using 1:4:5:6 with yerrorlines title 'point range {0,...,5000}'

unset logscale y
set output "random_overlaps.pdf"
set ylabel 'Number of overlapping segments'

plot \
  "<(grep -P '^[^\t]+\t0\t50\t' random.dat)" using 1:7:8:9 with yerrorlines title 'point range {0,...,50}', \
  "<(grep -P '^[^\t]+\t0\t500\t' random.dat)" using 1:7:8:9 with yerrorlines title 'point range {0,...,500}', \
  "<(grep -P '^[^\t]+\t0\t5000\t' random.dat)" using 1:7:8:9 with yerrorlines title 'point range {0,...,5000}'

set logscale y
set output "random_times.pdf"
set ylabel 'Execution time (in seconds)'

plot \
  "random.dat" using 1:(($1/2)**4/10**4) with lines title '(n/2)^4 / 10^4', \
  "<(grep -P '^[^\t]+\t0\t50\t' random.dat)" using 1:10:11:12 with yerrorlines title 'point range {0,...,50}', \
  "<(grep -P '^[^\t]+\t0\t500\t' random.dat)" using 1:10:11:12 with yerrorlines title 'point range {0,...,500}', \
  "<(grep -P '^[^\t]+\t0\t5000\t' random.dat)" using 1:10:11:12 with yerrorlines title 'point range {0,...,5000}'
