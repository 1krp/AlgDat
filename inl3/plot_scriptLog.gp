set terminal pdfcairo enhanced font "Arial,12" size 8,6
set output "comparisonLog_plot.pdf"

set title "Execution Time Comparison: Sorted vs Unsorted"
set xlabel "Input Size (Thousands)"
set ylabel "Execution Time (µs)"
set grid
set key top left
set logscale x 10  # Logarithmic scale for x-axis (base 10)
set xrange [0.8:1100]

plot "fixedUnsortedResult.txt" using ($1/1000):($2/1000) with linespoints title "Unsorted" lt rgb "red" lw 2, \
     "sortedResult.txt" using ($1/1000):($2/1000) with linespoints title "Sorted" lt rgb "blue" lw 2
