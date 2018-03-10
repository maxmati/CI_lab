//
// Created by maxmati on 3/6/18.
//

#ifndef CI_INPUT_H
#define CI_INPUT_H


#include "NeuronI.h"

class Input : public NeuronI {
public:
    double getOutput() override;
    void setOutput(double output);

    void propagateError(double delta) override;

    double getLastOutput() override;

private:
    double output;
};


#endif //CI_INPUT_H
