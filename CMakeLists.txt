cmake_minimum_required(VERSION 3.8)
project(CI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fPIC -Wall -Werror -pedantic")

add_executable(CI main.cpp Neuron.cpp Neuron.h NeuronI.cpp NeuronI.h Input.cpp Input.h Network.cpp Network.h streams.h utils.h)