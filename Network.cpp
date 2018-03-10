//
// Created by maxmati on 3/6/18.
//

#include "Network.h"

#include "streams.h"


Network::Network(int inputsCount, std::vector<int> layersSizes) {
    inputs.resize(inputsCount);
    layers = stream::fromEnumerable(layersSizes).map<std::vector<Neuron<>>>([](int size) {
        auto layer = std::vector<Neuron<>>{};
        layer.resize(size);
        return layer;
    }).collect(stream::PushBackColector<std::vector<std::vector<Neuron<>>>>{});

    for (Neuron<> &neuron: layers[0]) {
        for (Input &input: inputs) {
            neuron.addInput(&input);
        }
    }

    for (int i = 1; i < layers.size(); ++i) {
        for (Neuron<> &input: layers[i - 1]) {
            for (Neuron<> &output: layers[i]) {
                output.addInput(&input);
                input.addOutput(&output);
            }
        }
    }
}

std::vector<double> Network::calculate(std::vector<double> inputsData) {
    for (int i = 0; i < inputs.size(); ++i) {
        inputs[i].setOutput(inputsData[i]);
    }
    return stream::fromEnumerable(layers.back())
        .map<double>([](const Neuron<> &neuron) {
            return neuron.getOutput();
        })
        .collect(stream::PushBackColector<std::vector<double>>{});
}

void Network::train(std::vector<double> inputsData, std::vector<double> expectedOutputs) {
    std::vector<double> realOutputs = calculate(inputsData);

    /*std::vector<double> errors = */stream::fromEnumerable(realOutputs)
        .zip(stream::fromEnumerable(expectedOutputs))
        .map<double>([](const auto &it) {
            return it.second - it.first;
        }).zip(stream::fromEnumerable(layers.back()));
//        .map<int>([](auto it){
//            it.second.propagateError(it.first);
//            return 0;
//        }).flush();//collect(stream::PushBackColector<std::vector<double>>{});
}
