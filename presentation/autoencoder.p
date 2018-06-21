
set title "Training autoencpder on iris data"
set ylabel "Loss"
set xlabel "Epoch"
#set yrange [0:0.3]
set autoscale
plot "autoencoder.dat" u 1:3 t "loss" w lines, \
