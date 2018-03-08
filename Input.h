//
// Created by maxmati on 3/6/18.
//

#ifndef CI_INPUT_H
#define CI_INPUT_H


#include "NeuronI.h"

class Input : public NeuronI {
public:
    double getOutput() const override;
    void setOutput(double output);

private:
    double output;
};


#endif //CI_INPUT_H
