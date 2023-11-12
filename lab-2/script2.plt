#!/usr/bin/gnuplot

set terminal jpeg font arial 8 size 1280,960 font "Helvetica,22"
set output "data2.jpg"
set grid x y y2
set xlabel "Processes"
set ylabel "Speedup"
set y2tics
set title ("Метод Монте-Карло (nodes=2)")

plot 	'data2.txt' using 1:2 title "n = 10^7" with linespoints, \
	'data2.txt' using 1:3 title "n = 10^8" with linespoints, \
	'data2.txt' using 1:1 title "Linear speedup" with linespoints

