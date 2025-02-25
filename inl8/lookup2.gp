set terminal pdfcairo enhanced font "Arial,12" size 8,6
set output "lookupBench.pdf"

set title "Binary Tree Lookup Performance"
set xlabel "Number of Elements (Thousands)"
set ylabel "Lookup Time (ns)"
set grid

set xtics 100 nomirror   # Show x-axis labels only every 10,000 elements
set ytics nomirror
set key top left

# Define a logarithmic trendline
f(x) = a * log(x) + b
fit f(x) "lookupBench.txt" using ($1/1000):2 via a, b

plot "lookupBench.txt" using ($1/1000):2 with linespoints title "Lookup Time" lc rgb "blue" lw 2 pt 7, \
     f(x) title "Logarithmic Trendline" lw 2 lc rgb "red" dashtype 2
