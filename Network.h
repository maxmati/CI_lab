//
// Created by maxmati on 3/6/18.
//

#ifndef CI_NETWORK_H
#define CI_NETWORK_H


#include <vector>
#include "Neuron.h"
#include "Input.h"

class Network {
public:
    Network(int inputs, std::vector<int> layersSizes);
    std::vector<double> calculate(std::vector<double> inputs);

private:
    std::vector<std::vector<Neuron<>>> layers;
    std::vector<Input> inputs;
};


#endif //CI_NETWORK_H
