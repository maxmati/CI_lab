//
// Created by maxmati on 3/6/18.
//

#include "Neuron.h"

#include "streams.h"

#include <algorithm>

/*static*/ std::vector<double> normalizeInputs(const std::vector<Neuron<>::InputType> &inputs) {
    const auto inputComparator = [](const auto &a, const auto &b) {
        return a->first->getOutput() > b->first->getOutput();
    };
    double minInput = std::min(std::begin(inputs), std::end(inputs), inputComparator)->first->getOutput();
    double maxInput = std::max(std::begin(inputs), std::end(inputs), inputComparator)->first->getOutput();

    std::vector<double> result(inputs.size());
    std::transform(std::begin(inputs), std::end(inputs), std::back_inserter(result), [&](const auto &input) {
        return (input.first->getOutput() - minInput) / (maxInput - minInput);
    });

    return result;
}
