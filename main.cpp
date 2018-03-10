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
    Network net{3, {2}};
    std::cout << net.calculate({1.5, 0.6}) << std::endl;
    net.train({1,0},{1,1});
    std::cout << "Hello, World!" << std::endl;
    return 0;
}