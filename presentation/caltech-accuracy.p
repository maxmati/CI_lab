set datafile separator ","
set title "Caltech dataset accuracy"
set ylabel "Accuracy"
set xlabel "Epoch"
set autoscale
set grid
set samples 20
set key right bottom
plot "run_caltech_model_3_eval-tag-accuracy.csv" u 2:3 smooth bezier t "cifar-3" w lines, \
     "run_caltech_model_5_eval-tag-accuracy.csv" u 2:3 smooth bezier t "cifar-5" w lines, \
     "run_caltech_model_7_eval-tag-accuracy.csv" u 2:3 smooth bezier t "cifar-7" w lines, \
     "run_caltech_model_9_eval-tag-accuracy.csv" u 2:3 smooth bezier t "cifar-9" w lines
