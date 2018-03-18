//
// Created by maxmati on 3/8/18.
//

#ifndef CI_UTILS_H
#define CI_UTILS_H

#include <utility>
#include <random>

template<typename In1, typename In2, typename Out>
void zip(In1 input1It, const In1 &input1End, In2 input2It, Out outputIt) {
    for (; input1It != input1End; ++input1It, ++input2It) {
        outputIt = std::make_pair(*input1It, *input2It);
    }
}

class RandomGenerator {
private:
    std::random_device rd{};
    std::mt19937 gen{rd()};

    std::normal_distribution<> d{0, 0.5};
public:
    double generate() {
        return d(gen);
    }

    int randomIndex(int size) {
        std::uniform_int_distribution<> distribution{0, size - 1};
        return distribution(gen);
    }
};

#endif //CI_UTILS_H
