#!/usr/bin/gnuplot

set terminal jpeg font arial 8 size 1280,960 font "Helvetica,22"
set output "data1.jpg"
set grid x y y2
set xlabel "Processes"
set ylabel "Speedup"
set y2tics
set title ("правило Рунге (nodes=2)")

plot 	'data1.txt' using 1:2 title "eps = 1E-6" with linespoints pt 10 ps 2, \
	'data2.txt' using 1:1 title "Linear speedup" with linespoints pt 7 ps 2

