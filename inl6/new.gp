set terminal pdfcairo enhanced font "Arial,12" size 6,4
set output "queue_comparison.pdf"

set title "Queue Performance Comparison"
set xlabel "Queue Size (thousands)"
set ylabel "Time (us)"
set logscale y
set grid
set key top left

# Convert ns to us (divide by 1000)
plot "firstQ.txt" using ($1/1000):($2/1000) with linespoints title "No last pointer" lw 2 lc rgb "red", \
     "ImprovedQ.txt" using ($1/1000):($2/1000) with linespoints title "With last pointer" lw 2 lc rgb "blue"