//
// Created by maxmati on 3/6/18.
//

#ifndef CI_NEURONI_H
#define CI_NEURONI_H


class NeuronI {
public:
    virtual ~NeuronI() = default;
    virtual double getOutput() const = 0;
};


#endif //CI_NEURONI_H
