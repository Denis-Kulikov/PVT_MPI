#!/usr/bin/gnuplot

set terminal jpeg font arial 8 size 1280,960 font "Helvetica,22"
set output "data1_2.jpg"
set grid x y y2
set xlabel "Processes"
set ylabel "Speed up"
set y2tics
set title ("правило Рунге (nodes=1)")

plot 	'data1_2.txt' using 1:2 title "eps = 1E-6" with linespoints, \
	'data1_2.txt' using 1:1 title "Linear " with linespoints

