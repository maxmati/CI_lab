set datafile separator ","
set title "Mnist dataset accuracy"
set ylabel "Accuracy"
set xlabel "Epoch"
set autoscale
set grid
set samples 20
set key right bottom
plot "run_mnist_eval-tag-accuracy.csv" u 2:3 smooth bezier t "mnist-3" w lines
