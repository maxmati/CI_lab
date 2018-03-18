//
// Created by maxmati on 3/6/18.
//

#include "Neuron.h"

#include "streams.h"

#include <algorithm>

InputType::InputType(NeuronI *neuron, double weight) : neuron(neuron), weight(weight) {}
