set term pdf monochrome font ", 14"
set output "tasks_amount_plot.pdf"

set xlabel "time"
set ylabel "#tasks"

set yrange [0:2600000]

plot "task_amount.txt" w l
