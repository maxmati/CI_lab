//
// Created by maxmati on 3/27/18.
//

#ifndef CI_AUTOENCODERS_H
#define CI_AUTOENCODERS_H

#include "Network.h"

template<size_t inSize>
class AutoEncoders {
public:
    AutoEncoders() : net({3, 2, 3, inSize}), outputs(net.createProxyFromLayer(1)) {}

    template<typename T>
    void train(const T& train, const T& test) {
        TrainData<inSize, inSize> trainInputs;
        std::transform(train.begin(), train.end(), std::back_inserter(trainInputs), [](const auto& row) {
            return TrainRow<inSize, inSize>{row.first, row.first};
        });
        TrainData<inSize, inSize> testInputs;
        std::transform(test.begin(), test.end(), std::back_inserter(testInputs), [](const auto& row) {
            return TrainRow<inSize, inSize>{row.first, row.first};
        });

        net.train(trainInputs, testInputs, [](double lastError, double currentError,
                                              double lastFitness, double currentFitness) {
            return lastError < currentError && lastError < 0.1;
        });
    }

    std::vector<ProxyNeuron>& getOutputs() {
        return outputs;
    }

    void setInputs(const std::vector<double>& inputs) {
        assert(inputs.size() == inSize);
        net.setInputs(inputs);
    }
private:
    Network<inSize> net;
    std::vector<ProxyNeuron> outputs;
};

#endif //CI_AUTOENCODERS_H
