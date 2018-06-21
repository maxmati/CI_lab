set datafile separator ","
set title "Mnist dataset loss"
set ylabel "Loss"
set xlabel "Epoch"
set yrange [0:0.4]
set grid
set samples 20
set key right top
plot "run_mnist_eval-tag-loss.csv" u 2:3 smooth bezier t "eval" w lines, \
     "run_mnist-tag-loss.csv" u 2:3 smooth bezier t "test" w lines
