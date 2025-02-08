
set terminal pdfcairo enhanced font 'Arial,12' size 8,6
set output 'sorting_algorithms_plot.pdf'

set title 'Sorting Algorithm Performance'
set xlabel 'Input Size (Thousands)'
set ylabel 'Time (Milliseconds)'

set grid

plot 'selectBench2.txt' using ($1/1000):($2/1e6) with linespoints title 'Selection Sort' lt rgb "red", \
     'insertBench.txt' using ($1/1000):($2/1e6) with linespoints title 'Insertion Sort' lt rgb "blue"
