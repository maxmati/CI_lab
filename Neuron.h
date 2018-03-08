//
// Created by maxmati on 3/6/18.
//

#ifndef CI_NEURON_H
#define CI_NEURON_H


#include <vector>
#include <cmath>
#include <numeric>
#include "NeuronI.h"

class TanHActivation {
public:
    static double activation(double x){
        return tanh(x);
    }
};

template<typename ActivationFunction = TanHActivation>
class Neuron : public NeuronI {
public:
    typedef std::pair<NeuronI*, double> InputType;
    virtual ~Neuron() = default;
    double getOutput() const override{
        double sum = std::accumulate(std::begin(inputs), std::end(inputs), 0.0, [](double current, const InputType &it) {
            return current + it.first->getOutput() * it.second;
        });

        return ActivationFunction::activation(sum);
    }
    void addInput(NeuronI* neuron){
        inputs.emplace_back(neuron, 0.2);
    }
    void addOutput(NeuronI* neuron){
        outputs.push_back(neuron);
    }

private:
    std::vector<InputType> inputs;
    std::vector<NeuronI*> outputs;

};


#endif //CI_NEURON_H
