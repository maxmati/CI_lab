//
// Created by maxmati on 3/6/18.
//

#include "Input.h"

double Input::getOutput() const{
    return output;
}

void Input::setOutput(double output) {
    this->output = output;
}

void Input::propagateError(double delta) {

}
