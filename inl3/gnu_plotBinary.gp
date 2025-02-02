set terminal pdfcairo enhanced font "Arial,12" size 8,6
set output "binary_search_plot.pdf"

set title "Binary Search Execution Time"
set xlabel "Input Size (Thousands)"
set ylabel "Execution Time (ns)"
set grid
set key top left
set logscale x 10  # Logarithmic scale for x-axis (base 10)

# Plot the data with x-values divided by 1000 for readability
plot "binaryBench.txt" using ($1/1000):2 with linespoints title "Binary Search" lt rgb "blue" lw 2
