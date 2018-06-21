set datafile separator ","
set title "Cifar dataset loss"
set ylabel "Loss"
set xlabel "Epoch"
set yrange [0.3:1.7]
set grid
set samples 20
set key right top
plot "run_cifar_model_3_eval-tag-loss.csv" u 2:3 smooth bezier t "eval" w lines, \
     "run_cifar_model_3-tag-loss.csv" u 2:3 smooth bezier t "test" w lines
