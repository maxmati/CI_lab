//
// Created by maxmati on 3/6/18.
//

#ifndef CI_NETWORK_H
#define CI_NETWORK_H


#include <vector>
#include "Neuron.h"
#include "Input.h"
#include "streams.h"

class Network {
public:
    Network(int inputs, std::vector<int> layersSizes);

    std::vector<double> calculate(const std::vector<double> &inputs);

    void train(std::vector<double> inputsData, std::vector<double> expectedOutputs);

    template<typename T>
    double testFitnes(T data) {
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
    void trainBatch(T data) {

//        size_t batchSize = data.size()/4;
//
//        for (size_t i = 0; i < batchSize; ++i) {
//            std::swap(data[i],data[generator.randomIndex(data.size)]);
//        }

        for (const auto &row: data) {
            train(std::vector<double>{row.first.begin(), row.first.end()},
                  std::vector<double>{row.second.begin(), row.second.end()});
        }

        for (auto &layer: layers) {
            for (auto &neuron: layer) {
                neuron.applyTraining(data.size());
            }
        }
    }

private:
    RandomGenerator generator;
    Input bias;
    std::vector<std::vector<Neuron<>>> layers;
    std::vector<Input> inputs;

};


#endif //CI_NETWORK_H
