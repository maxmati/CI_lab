//
// Created by maxmati on 3/8/18.
//

#ifndef CI_UTILS_H
#define CI_UTILS_H

#include <utility>
#include <random>

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
  std::mt19937& get() {
      return gen;
  }
};

#endif //CI_UTILS_H
