#include "Function.h"

using namespace structs;

Function::Function(string name, int params_num, double (*eval)(vector<double>)) {
    this->name = name;
    this->params_num = params_num;
    this->eval = eval;
}