//
// Created by maxmati on 4/7/18.
//

#pragma once

#include <functional>
#include <utility>
#include "NeuronI.h"

class ProxyNeuron : public NeuronI {
public:
    explicit ProxyNeuron(std::function<double()> outputFunction,
                         std::function<void(double)> errorPropagationFunction = {})
        : outputFunction(std::move(outputFunction)),
          errorPropagationFunction(std::move(errorPropagationFunction)) {}

    double getOutput() override {
        return outputFunction();
    }
    void propagateError(double delta) override {
        if(errorPropagationFunction)
            errorPropagationFunction(delta);
    }
    double getLastOutput() override {
        return lastOutput;
    }

private:
    double lastOutput = 0;
    std::function<double()> outputFunction;
    std::function<void(double)> errorPropagationFunction;

};



