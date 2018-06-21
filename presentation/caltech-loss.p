set datafile separator ","
set title "Caltech dataset loss"
set ylabel "Loss"
set xlabel "Epoch"
set yrange [0:4.7]
set xrange [0:20000]
set grid
set samples 100
set key right top
plot "run_caltech_model_3_eval-tag-loss.csv" u 2:3 smooth bezier t "eval" w lines, \
     "run_caltech_model_3-tag-loss.csv" u 2:3 smooth bezier t "test" w lines
