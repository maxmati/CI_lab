
set title "MLP network with autoencoder on iris data"
set xlabel "Epoch"
plot "autoencoder_mlp.dat" u 1:3 t "loss" w lines, \
     "autoencoder_mlp.dat" u 1:2 t "accuracy" w lines
