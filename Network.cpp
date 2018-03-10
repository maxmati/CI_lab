//
// Created by maxmati on 3/6/18.
//

#include <algorithm>
#include "Network.h"

#include "streams.h"


Network::Network(int inputsCount, std::vector<int> layersSizes) {
    inputs.resize(inputsCount);
    bias.setOutput(1);
    layers = stream::fromEnumerable(layersSizes).map<std::vector<Neuron<>>>([this](int size) {
        auto layer = std::vector<Neuron<>>{};
        std::generate_n(std::back_inserter(layer), size, [this]() { return Neuron<>(generator); });
        return layer;
    }).collect(stream::PushBackColector<std::vector<std::vector<Neuron<>>>>{});

    for (Neuron<> &neuron: layers[0]) {
        neuron.addInput(&bias);
        for (Input &input: inputs) {
            neuron.addInput(&input);
        }
    }

    for (size_t i = 1; i < layers.size(); ++i) {
        for (Neuron<> &output: layers[i]) {
            output.addInput(&bias);
            for (Neuron<> &input: layers[i - 1]) {
                output.addInput(&input);
                input.addOutput(&output);
            }
        }
    }
}

std::vector<double> Network::calculate(const std::vector<double>& inputsData) {
    for (size_t i = 0; i < inputs.size(); ++i) {
        inputs[i].setOutput(inputsData[i]);
    }

    std::vector<double> results{};

    std::transform(std::begin(layers.back()), std::end(layers.back()), std::back_inserter(results),
                   [](Neuron<> &neuron) {
                       return neuron.getOutput();
                   });

    return results;
}

void Network::train(std::vector<double> inputsData, std::vector<double> expectedOutputs) {
    std::vector<double> realOutputs = calculate(inputsData);

    std::vector<NeuronI *> neurons{};
    std::transform(std::begin(layers.back()), std::end(layers.back()), std::back_inserter(neurons),
                   [](auto &neuron) {
                       return &neuron;
                   });

    /*std::vector<double> errors = */stream::fromEnumerable(realOutputs)
        .zip(stream::fromEnumerable(expectedOutputs))
        .map<double>([](const auto &it) {
            return it.first - it.second;
        }).zip(stream::fromEnumerable(neurons))
        .map<int>([](auto it) {
            it.second->propagateError(it.first);
            return 0;
        }).flush();//collect(stream::PushBackColector<std::vector<double>>{});
}
