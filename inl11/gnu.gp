set terminal pdfcairo size 1200,800
set output 'malmo_travel_plot.pdf'

set title 'Travel Data from Malmö'
set xlabel 'Distance'
set ylabel 'Some Value'

set grid
set key outside
set style data points

plot 'bench5.txt' using 1:2 with points title 'Travel Data'
