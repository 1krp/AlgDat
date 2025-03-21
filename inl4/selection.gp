set terminal pdfcairo enhanced font 'Arial,12' size 8,6
set output 'selection_sort_plot.pdf'

set title 'Insertion Sort Performance'
set xlabel 'Input Size (Thousands, log scale)'
set ylabel 'Time (Milliseconds, log scale)'

set logscale x 2
set grid

plot 'selectBench2.txt' using ($1/1000):($2/1e6) with linespoints title 'Selection Sort'