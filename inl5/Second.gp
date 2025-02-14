set terminal pdfcairo enhanced font "Arial,12" size 8,6
set output 'second.pdf'

set title 'A-fixed, B-increasing'
set xlabel "size (Thousands)"
set ylabel "(Time in µs)"

set format x "%.0sK"  # Formats x-axis labels in thousands
set format y "%.0sµs"  # Uses microseconds (µs) for Y values

set grid
plot 'append100k.txt' using ($1/1000):($2/1000) with linespoints title 'Append Increasing A' lw 2 pt 7, \
     'appendFixed100k.txt' using ($1/1000):($2/1000) with linespoints title 'Append Fixed A' lw 2 pt 7
