//
// Created by maxmati on 3/6/18.
//

#ifndef CI_NETWORK_H
#define CI_NETWORK_H


#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include "Neuron.h"
#include "Input.h"
#include "streams.h"

template<size_t inputsCount>
class Network {
public:
    explicit Network(std::vector<int> layersSizes) {
        bias.setOutput(1);
        layers = stream::fromEnumerable(layersSizes).template map<std::vector<Neuron<>>>([this](int size) {
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

    template<typename T>
    std::vector<double> calculate(const T &inputsData) {
        assert(inputsData.size() == inputsCount);
        for (size_t i = 0; i < inputsCount; ++i) {
            inputs[i].setOutput(inputsData[i]);
        }

        std::vector<double> results{};

        std::transform(std::begin(layers.back()), std::end(layers.back()), std::back_inserter(results),
                       [](Neuron<> &neuron) {
                           return neuron.getOutput();
                       });

        return results;
    }

    template<typename T>
    double testFitness(T data) {
        size_t good = 0;
        for (const auto &row: data) {
            auto result = calculate(std::vector<double>{row.first.begin(), row.first.end()});
            if (std::distance(result.begin(), std::max_element(result.begin(), result.end()))
                == std::distance(row.second.begin(), std::max_element(row.second.begin(), row.second.end()))) {
                ++good;
            }
        }

        return static_cast<double>(good) / data.size();
    }

    template<typename T>
    double testError(T data) {
        double error = 0;
        for (auto &row: data) {
            auto result = calculate(std::vector<double>{row.first.begin(), row.first.end()});
            auto expected = std::vector<double>{row.second.begin(), row.second.end()};

            for (size_t i = 0; i < result.size(); ++i) {
                double a = result[i];
                double b = expected[i];
                error += (a - b) * (a - b);
            }
        }

        return error / data.size();
    }

    template<typename T>
    void train(const T &trainData, const T &testData) {
        double lastError;// = std::numeric_limits<double>::max();
        double currentError = std::numeric_limits<double>::max();
        do {
            lastError = currentError;

            for (int i = 0; i < 50; ++i) {
                trainBatch(trainData);
            }

            currentError = testError(testData);
            std::cout << testFitness(testData) << " " << currentError << std::endl;
        } while (lastError > currentError);
    }

private:
    template<typename T>
    void trainBatch(const T &data, size_t batches = 4) {

        size_t batchSize = data.size() / batches;

        auto it = data.begin();
        auto end = it;
        for (size_t i = 0; i < batches; ++i) {
            std::advance(end, batchSize);

            for (; it != end; ++it) {
                trainExample(std::vector<double>{it->first.begin(), it->first.end()},
                             std::vector<double>{it->second.begin(), it->second.end()});
            }

            for (auto &layer: layers) {
                for (auto &neuron: layer) {
                    neuron.applyTraining(batchSize);
                }
            }
        }
    }

    template<typename T1, typename T2>
    void trainExample(const T1 &inputsData, const T2 &expectedOutputs) {
        std::vector<double> realOutputs = calculate(inputsData);

        std::vector<NeuronI *> neurons{};
        std::transform(std::begin(layers.back()), std::end(layers.back()), std::back_inserter(neurons),
                       [](auto &neuron) {
                           return &neuron;
                       });

        stream::fromEnumerable(realOutputs)
            .zip(stream::fromEnumerable(expectedOutputs))
            .template map<double>([](const auto &it) {
                return it.first - it.second;
            }).zip(stream::fromEnumerable(neurons))
            .
                template map<int>([](auto it) {
                it.second->propagateError(it.first);
                return 0;
            }).flush();

        for (auto &layer: layers) {
            for (auto &neuron: layer) {
                neuron.train();
            }
        }
    }

    RandomGenerator generator;
    Input bias;
    std::vector<std::vector<Neuron<>>> layers;
    std::array<Input, inputsCount> inputs;

};


#endif //CI_NETWORK_H
