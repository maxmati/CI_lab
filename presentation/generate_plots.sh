#!/bin/bash

gnuplot -e "set terminal png;set output 'mnist-accuracy.png';load 'mnist-accuracy.p';"
gnuplot -e "set terminal png;set output 'mnist-loss.png';load 'mnist-loss.p';"
gnuplot -e "set terminal png;set output 'cifar-accuracy.png';load 'cifar-accuracy.p';"
gnuplot -e "set terminal png;set output 'cifar-loss.png';load 'cifar-loss.p';"
gnuplot -e "set terminal png;set output 'caltech-accuracy.png';load 'caltech-accuracy.p';"
gnuplot -e "set terminal png;set output 'caltech-loss.png';load 'caltech-loss.p';"
