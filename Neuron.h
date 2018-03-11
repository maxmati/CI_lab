//
// Created by maxmati on 3/6/18.
//

#ifndef CI_NEURON_H
#define CI_NEURON_H


#include <vector>
#include <cmath>
#include <numeric>
#include "NeuronI.h"
#include "utils.h"

class TanHActivation {
public:
    static constexpr double activation(double x) {
        return tanh(x);
    }

    static constexpr double derivative(double x) {
        const auto d = activation(x);
        return 1 - d * d;
    }
};

class SigmActivation {
public:
    static double activation(double x) {
        return 1. / (1. + exp(-1. * x));
    }

    static double derivative(double x) {
        return x * (1 - x);
    }
};

template<typename ActivationFunction = SigmActivation>
class Neuron : public NeuronI {
private:
    const double eta = 0.2;
public:
    typedef std::pair<NeuronI *, double> InputType;

    ~Neuron() override = default;

    explicit Neuron(RandomGenerator &generator) : generator(generator) {};

    Neuron(const Neuron &) = delete;

    Neuron(Neuron &&) noexcept = default;

    double getOutput() override {
        double sum = std::accumulate(std::begin(inputs), std::end(inputs), 0.0,
                                     [](double current, const InputType &it) {
                                         return current + it.first->getOutput() * it.second;
                                     });

        lastOutput = ActivationFunction::activation(sum);
        return lastOutput;
    }

    void addInput(NeuronI *neuron) {
        inputs.emplace_back(neuron, generator.generate());
    }

    void addOutput(NeuronI *neuron) {
        outputs.push_back(neuron);
    }

    void propagateError(double delta) override {
        for (auto &input: inputs) {
            input.first->propagateError(delta * input.second * ActivationFunction::derivative(lastOutput));
            input.second +=
                -1. * eta * delta * ActivationFunction::derivative(lastOutput) * input.first->getLastOutput();
        }
    }

    double getLastOutput() override {
        return lastOutput;
    }

private:
    RandomGenerator &generator;
    std::vector<InputType> inputs;
    std::vector<NeuronI *> outputs;
    double lastOutput = 0;
};


#endif //CI_NEURON_H
