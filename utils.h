//
// Created by maxmati on 3/8/18.
//

#ifndef CI_UTILS_H
#define CI_UTILS_H

#include <utility>

template<typename In1, typename In2, typename Out>
void zip(In1 input1It, const In1 &input1End, In2 input2It, Out outputIt) {
    for (; input1It != input1End; ++input1It, ++input2It) {
        outputIt = std::make_pair(*input1It, *input2It);
    }
};

#endif //CI_UTILS_H
