#include "Function.h"

using namespace structs;

Function::Function(std::string name, int params_num, FunctionAction action) {
    this->name = name;
    this->params_num = params_num;
    this->eval = action;
}