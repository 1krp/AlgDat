set terminal pdfcairo enhanced font 'Arial,12' size 8,6
set output 'selection_sort_plot.pdf'

set title 'Selection Sort Performance'
set xlabel 'Input Size (Thousands, log scale)'
set ylabel 'Time (Milliseconds, log scale)'

set logscale x 2
set logscale y 2
set grid

plot 'selectionBench.txt' using ($1/1000):($2/1e6) with linespoints title 'Selection Sort'