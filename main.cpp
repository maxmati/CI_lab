#include <iostream>
#include <array>
#include <fstream>
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
    TrainData<4,3> td = loadData<4,3>("data/iris-train.csv");
    Network net{2, {30, 20, 20, 15, 3}};

    for (int i = 0; i < 10; ++i) {
        std::cout<<i<<std::endl;
        for(const auto& row: td){
            net.train(std::vector<double>{row.first.begin(), row.first.end()}, std::vector<double>{row.second.begin(), row.second.end()});
        }
    }

    std::cout << net.calculate({5.20, 2.70, 3.90, 1.40}) << std::endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}