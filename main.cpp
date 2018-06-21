#include <iostream>
#include <array>
#include <fstream>
#include <algorithm>
#include "Network.h"
#include "AutoEncoders.h"

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& dt) {
    for (const T& data: dt) {
        os << data << " ";
    }

    return os;
}

template<size_t in, size_t out>
using TrainRow = std::pair<std::array<double, in>, std::array<double, out>>;

template<size_t in, size_t out>
using TrainData = std::vector<TrainRow<in, out>>;

template<size_t in, size_t out>
TrainData<in, out> loadData(const std::string& trainDataFile) {
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

int main() {
    auto td = loadData<4, 3>("data/iris-train.csv");
    auto testData = loadData<4, 3>("data/iris-test.csv");
//    auto testData2= loadData<13, 3>("data/iris-test.csv");

    AutoEncoders<4> autoEnc{};
    autoEnc.train(td, testData);

    Network<4> net{{8, 5, 3}};

    net.registerProxyInputs(autoEnc.getOutputs(), [&](const std::vector<double>& in){
        autoEnc.setInputs(in);
    });

    std::cout << "Hello, World!" << std::endl;

    net.train(td, testData, [](double lastError, double currentError,
        double lastFitness, double currentFitness) {
        return lastError < currentError && currentFitness > 0.9;
    });
//
    std::cout << net.testFitness(testData) << std::endl;

    std::cout << net.calculate<std::vector<double>>({5.20, 2.70, 3.90, 1.40}) << std::endl;

    return 0;
}