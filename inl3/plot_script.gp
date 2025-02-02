set terminal pdfcairo enhanced font "Arial,12" size 8,6
set output "comparison_plot.pdf"

set title "Execution Time Comparison: Sorted vs Unsorted"
set xlabel "Input Size (Thousands)"  # Updated to indicate input is in thousands
set ylabel "Execution Time (µs)"  # Updated to microseconds
set grid
set key top left

plot "fixedUnsortedResult.txt" using ($1/1000):($2/1000) with linespoints title "Unsorted" lt rgb "red" lw 2, \
     "sortedResult.txt" using ($1/1000):($2/1000) with linespoints title "Sorted" lt rgb "blue" lw 2, \
