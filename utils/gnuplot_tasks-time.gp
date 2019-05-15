set term pdf font ", 7"
set output "tasks_amount_plot.pdf"

set key spacing 1 font ", 5" width 5
set key left top

set style line  1 lt -1 lw 1 lc rgb "#00FF0000" pt 1 ps 0.1
set style line  2 lt -1 lw 1 lc rgb "#000000FF"  pt 1 ps 0.1
set style line  3 lt -1 lw 1 lc rgb "#0000FF00"  pt 1 ps 0.1
set style line  4 lt -1 lw 1 lc rgb "yellow" pt 1 ps 0.1

rstyle = 1
bstyle = 2
gstyle = 3
ystyle = 4

#set xlabel "Iterations"
set xlabel "Time (s)"
set ylabel "#Tasks"
set y2label "#Edges"

# Use finer granularity for real time
#set xtics 0.5

stats '~/kappa/task_amount.txt' using 4:5

set yrange [0:STATS_max_y]
set ytics nomirror
set y2range [STATS_min_x:STATS_max_x+STATS_max_x*0.001]
set y2tics

# use (1-:n for real time and 2:n for logical time
plot "< awk '{if($6 == \"0\") print}' ~/kappa/task_amount.txt" u 1:5 t "On Activate (compute)" with points ls rstyle axis x1y1, \
     "< awk '{if($6 == \"1\") print}' ~/kappa/task_amount.txt" u 1:5 t "On Add-edge (activate)" with points ls bstyle axis x1y1, \
     "< awk '{if($6 == \"2\") print}' ~/kappa/task_amount.txt" u 1:5 t "Update" with points ls gstyle axis x1y1, \
     "< awk '{if($5 == \"-1\") print}' ~/kappa/task_amount.txt | awk '{print $1, 0}'" notitle with points pt 1 ps 4 axis x1y1, \
     "< cat ~/kappa/task_amount.txt" u 1:4 t "|E|" with points ls ystyle axis x1y2
