set terminal pdf size 30cm, 21cm

set key top left
set logscale y
set xlabel 'Number of input points'

set output "times.pdf"
set ylabel "Time in seconds"

plot \
	"times.dat" using 1:2:3:4 with yerrorlines title "CPLEX running time"
