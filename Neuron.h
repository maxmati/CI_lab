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
    static double activation(double x) {
        return tanh(x);
    }

    static double derivative(double x) {
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

struct InputType {
    NeuronI *neuron;
    double weight;
    double dWeight = 0;

    InputType(NeuronI *neuron, double weight);
};

template<typename ActivationFunction = SigmActivation>
class Neuron : public NeuronI {
private:
    double eta = 0.2;
public:
    ~Neuron() override = default;

    explicit Neuron(RandomGenerator &generator) : generator(generator) {};

    Neuron(const Neuron &) = delete;

    Neuron(Neuron &&) noexcept = default;

    double getOutput() override {
        double sum = std::accumulate(std::begin(inputs), std::end(inputs), 0.0,
                                     [](double current, const InputType &it) {
                                         return current + it.neuron->getOutput() * it.weight;
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
        this->delta += delta;
    }

    void train(size_t batch_size) {
        for (auto &input: inputs) {
            input.neuron->propagateError(delta * input.weight * ActivationFunction::derivative(lastOutput));
            input.dWeight +=
                (-1. * eta * delta * ActivationFunction::derivative(lastOutput) * input.neuron->getLastOutput()) /
                batch_size;
        }
        delta = 0;
    }

    void applyTraining(size_t batch_size) {
        for (auto &input: inputs) {
            input.weight += input.dWeight / batch_size;
            input.dWeight = 0;
        }
        eta *= 0.99999;
    }

    double getLastOutput() override {
        return lastOutput;
    }

private:
    RandomGenerator &generator;
    std::vector<InputType> inputs;
    std::vector<NeuronI *> outputs;
    double lastOutput = 0;
    double delta = 0;
};


#endif //CI_NEURON_H
