set term pdf font ", 14"
set output "tasks_amount_plot.pdf"

set style line  4 lt -1 lw 1 lc rgb "blue" pt 1 ps 0.1
set style line  5 lt -1 lw 1 lc rgb "blue" pt 1 ps 1 dashtype 4
set style line  6 lt -1 lw 1 lc rgb "red"  pt 1 ps 0.1

bstyle = 4
rstyle = 6

set xlabel "iterations"
set ylabel "#tasks"

#set yrange [0:10000000]

plot "< awk '{if($5 == \"0\") print}' ~/kappa/task_amount.txt.numbered" u 1:4 t "Schedule" with points ls rstyle, \
     "< awk '{if($5 == \"1\") print}' ~/kappa/task_amount.txt.numbered" u 1:4 t "Compute" with points ls bstyle


