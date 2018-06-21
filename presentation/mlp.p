
set title "MLP network on iris data"
set ylabel "Loss"
set xlabel "Epoch"
plot "mlp.dat" u 1:3 t "loss" w lines, \
     "mlp.dat" u 1:2 t "accuracy" w lines
