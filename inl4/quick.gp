set terminal pdfcairo enhanced font "Arial,12" size 6,4
set output "quickTest2.pdf"

set xlabel "Array Size (Thousands)"
set ylabel "Time (Microseconds)"
set title "Quick Sort Performance"

set xtics format "%.0fK"
set ytics format "%.1f μs"

set grid

plot "quickTest2.txt" using ($1/1000):($2/1000) with linespoints title "Quick Sort Time" lw 2 pt 7 lc rgb "blue"
