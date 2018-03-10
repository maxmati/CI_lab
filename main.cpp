#include <iostream>
#include "Network.h"

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& dt)
{
    for(const T& data: dt){
        os << data << " ";
    }

    return os;
}

int main() {
    Network net{2, {2, 1}};
    std::cout << net.calculate({1, 0}) << std::endl;
    for (int i = 0; i < 30000; ++i) {
        net.train({1,1},{0});
        net.train({1,0},{1});
        net.train({0,1},{1});
        net.train({0,0},{0});

    }
    std::cout << "1 1 " << net.calculate({1, 1}) << std::endl;
    std::cout << "1 0 " << net.calculate({1, 0}) << std::endl;
    std::cout << "0 1 " << net.calculate({0, 1}) << std::endl;
    std::cout << "0 0 " << net.calculate({0, 0}) << std::endl;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}