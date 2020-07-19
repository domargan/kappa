#!/bin/bash

set +e
~/kappa/cmake-build-debug-remote-host/kappa 64 10000
set -e

awk 'NF>=6' task_amount.txt >task_amount_clean.txt

mv task_amount_clean.txt task_amount.txt

gnuplot ~/kappa/utils/gnuplot_tasks-time.gp
