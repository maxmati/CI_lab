//
// Created by maxmati on 3/8/18.
//

#ifndef CI_UTILS_H
#define CI_UTILS_H

#include <iostream>
#include <utility>
#include <random>

class RandomGenerator {
private:
    std::random_device rd{};
    unsigned int seed = rd();
    std::mt19937 gen{seed};

    std::normal_distribution<> d{0, 0.5};
public:
    RandomGenerator() {
        std::cout << "Creating random generator with seed: " << seed << std::endl;
    }
    double generate() {
        return d(gen);
    }

    std::mt19937& get() {
        return gen;
    }
};

#endif //CI_UTILS_H
