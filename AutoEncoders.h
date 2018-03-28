//
// Created by maxmati on 3/27/18.
//

#ifndef CI_AUTOENCODERS_H
#define CI_AUTOENCODERS_H


#include "Network.h"

template <size_t inSize>
class AutoEncoders {
public:
    AutoEncoders() : net({10,8,10, inSize}) {}

    template <typename T>
    void train(const T& data){
        net.train(data, data);
    }

private:
    Network<inSize> net;
};


#endif //CI_AUTOENCODERS_H
