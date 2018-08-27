#!/bin/bash

KAPPA_PATH="$HOME/kappa"
KAPPA_EXEC="$KAPPA_PATH/cmake-build-debug/kappa"
KAPPA_LOG="measurements.csv"
KAPPA_PLOT_DATA="scaling.dat"
COMP_PLOT_FILE=$(mktemp gnuplot_computation.pXXX)
COMP_OUT_FILE=computation_scaling_plot
UPDATES_PLOT_FILE=$(mktemp updates_gnuplot.pXXX)
UPDATES_OUT_FILE=updates_scaling_plot


#### Run scaling experiments and log measurements ####

cores=(1 2 4 8 16 32 64)
iters=(1 2 3 4 5)

batch_size=100000

>$KAPPA_PLOT_DATA

for core in ${cores[@]}; do
	KAPPA_LOG_CONCAT="measurements-${core}-core.csv"

	>$KAPPA_LOG_CONCAT

	for iter in ${iters[@]}; do
		>$KAPPA_LOG

		$KAPPA_EXEC ${core} ${batch_size}

		tail -n +2 $KAPPA_LOG >> $KAPPA_LOG_CONCAT
	done

	ingestion_avg=$(awk '{ total += $7 } END { print total/NR }' $KAPPA_LOG_CONCAT)
	computation_avg=$(awk '{ total += $9 } END { print total/NR }' $KAPPA_LOG_CONCAT)

	ingestion_sttdev=$(awk '{total += $7; sumsq += $7*$7} END {print sqrt(sumsq/NR - (total/NR)**2)}' $KAPPA_LOG_CONCAT)
	computation_sttdev=$(awk '{total += $9; sumsq += $9*$9} END {print sqrt(sumsq/NR - (total/NR)**2)}' $KAPPA_LOG_CONCAT)

	echo $core $ingestion_avg $ingestion_sttdev $computation_avg $computation_sttdev >> $KAPPA_PLOT_DATA
done


#### Plot measurements ####

## Plot computation ##
XLABEL="#cores"
YLABEL="CPU time (sec)"

cat << EOF > $COMP_PLOT_FILE
set term pdf monochrome font ", 14"
set output "${COMP_OUT_FILE}.pdf"

set xlabel "${XLABEL}"
set ylabel "${YLABEL}" offset 1.5

set size ratio 0.5
#set size 0.8,0.8
EOF

echo -n 'set xtics (' >> $COMP_PLOT_FILE
for i in ${cores[@]}; do
echo -n " $i," >> $COMP_PLOT_FILE
done
echo ')' >> $COMP_PLOT_FILE

cat << EOF >> $COMP_PLOT_FILE
#set ytics nomirror

set key left top
#set key font ",12"

set xrange [${cores[0]}:${cores[-1]}]
set yrange [0:]

plot \
	"$KAPPA_PLOT_DATA" using 1:4 title 'computation' with linespoint, \
	"$KAPPA_PLOT_DATA" using 1:4:5 title '' with yerrorbars
EOF


## Plot updates ##

XLABEL="#cores"
YLABEL="CPU time (sec)"

cat << EOF > $UPDATES_PLOT_FILE
set term pdf monochrome font ", 14"
set output "${UPDATES_OUT_FILE}.pdf"

set xlabel "${XLABEL}"
set ylabel "${YLABEL}" offset 1.5

set size ratio 0.5
#set size 0.8,0.8
EOF

echo -n 'set xtics (' >> $UPDATES_PLOT_FILE
for i in ${cores[@]}; do
echo -n " $i," >> $UPDATES_PLOT_FILE
done
echo ')' >> $UPDATES_PLOT_FILE

cat << EOF >> $UPDATES_PLOT_FILE
#set ytics nomirror

set key left top
#set key font ",12"

set xrange [${cores[0]}:${cores[-1]}]
set yrange [0:]

plot \
	"$KAPPA_PLOT_DATA" using 1:2 title 'ingestion' with linespoint, \
	"$KAPPA_PLOT_DATA" using 1:2:3 title '' with yerrorbars
EOF


# call gnuplot
gnuplot $COMP_PLOT_FILE
gnuplot $UPDATES_PLOT_FILE
# rm $PLOT_FILE

# pdfcrop on the figure
pdfcrop --margins "0 0 0 0" --clip ${COMP_OUT_FILE}.pdf ${COMP_OUT_FILE}.pdf &> /dev/null
pdfcrop --margins "0 0 0 0" --clip ${UPDATES_OUT_FILE}.pdf ${UPDATES_OUT_FILE}.pdf &> /dev/null
