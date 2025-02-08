# Set logarithmic scale for both axes
set logscale xy

# Label the axes
set xlabel "Input Size"
set ylabel "Time (ns)"

# Title of the graph
set title "Merge Sort Time Complexity"

# Plot the data from the file
plot "mergeBench.txt" using 1:2 with linespoints title "Merge Sort"
