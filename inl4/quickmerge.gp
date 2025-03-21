
set terminal pdfcairo enhanced font 'Arial,12' size 8,6
set output 'sorting_algorithms2_plot.pdf'

set title 'Merge and Quick sort Performance'
set xlabel 'Input Size (Thousands)'
set ylabel 'Time (Milliseconds)'

set grid

plot 'mergeBench.txt' using ($1/1000):($2/1e6) with linespoints title 'Merge Sort' lt rgb "red", \
     'quickBench.txt' using ($1/1000):($2/1e6) with linespoints title 'Quick Sort' lt rgb "blue"
