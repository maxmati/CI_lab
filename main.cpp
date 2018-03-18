#include <iostream>
#include <array>
#include <fstream>
#include <algorithm>
#include "Network.h"

template<typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &dt) {
    for (const T &data: dt) {
        os << data << " ";
    }

    return os;
}

template<size_t in, size_t out>
using TrainRow = std::pair<std::array<double, in>, std::array<double, out>>;

template<size_t in, size_t out>
using TrainData = std::vector<TrainRow<in, out>>;

template<size_t in, size_t out>
TrainData<in, out> loadData(const std::string &trainDataFile) {
    TrainData<in, out> result;
    std::ifstream file{trainDataFile};
    TrainRow<in, out> data;

    while (!file.eof() && file.good()) {
        for (size_t i = 0; i < in; ++i) {
            file >> data.first[i];
        }

        for (size_t i = 0; i < out; ++i) {
            file >> data.second[i];
        }

        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        result.push_back(data);
    }
    return result;
}

template<size_t in, size_t out>
/*static*/ void normalizeData(TrainData<in, out> &inputs) {
    for (size_t i = 0; i < in; ++i) {
        const auto inputComparator = [i](const auto &a, const auto &b) {
            return a->first[i] > b->first[i];
        };

        double minInput = std::min(std::begin(inputs), std::end(inputs), inputComparator)->first[i];
        double maxInput = std::max(std::begin(inputs), std::end(inputs), inputComparator)->first[i];

        for (auto &row: inputs) {
            row.first[i] = (row.first[i] - minInput) / (maxInput - minInput);
        }

    }



//    std::vector<double> result(inputs.size());
//    std::transform(std::begin(inputs), std::end(inputs), std::back_inserter(result), [&](const auto &input) {
//        return (input.neuron->getOutput() - minInput) / (maxInput - minInput);
//    });
//
//    return result;
}

int main() {
    TrainData<4, 3> td = loadData<4, 3>("data/iris-train.csv");
    TrainData<4, 3> testData = loadData<4, 3>("data/iris-test.csv");
//    normalizeData(td);
//    normalizeData(testData);
    Network net{4, {10, 8, 5, 3}};

    for (int i = 0; i < 4000; ++i) {
        if(i%100 == 0) {
            std::cout << i << std::endl;
            std::cout << net.testFitnes(testData) << " " << net.testError(testData) << std::endl;
        }
        net.trainBatch(td);
    }

    std::cout << net.testFitnes(testData) << std::endl;

    std::cout << net.calculate({5.20, 2.70, 3.90, 1.40}) << std::endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}