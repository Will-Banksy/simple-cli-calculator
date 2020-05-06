#include "FunctionHandler.h"
#include <math.h>
#include <sstream>

using namespace helpers;
using namespace structs;

map<string, Function> FunctionHandler::functions = initFunctions();

map<string, Function> FunctionHandler::initFunctions() {
    map<string, Function> map;

    // map["sin"] = Function("sin", 1, [](vector<double> args) {
    //     return sin(args[0]);
    // });
    // map["cos"] = Function("cos", 1, [](vector<double> args) {
    //     return cos(args[0]);
    // });
    // map["tan"] = Function("tan", 1, [](vector<double> args) {
    //     return tan(args[0]);
    // });

    // map.insert(pair<string, Function>("sin", Function("sin", 1, [](vector<double> args) { // Define function with lambda
    //     return sin(args[0]);
    // })));
    // map.insert(pair<string, Function>("cos", Function("cos", 1, [](vector<double> args) {
    //     return cos(args[0]);
    // })));
    // map.insert(pair<string, Function>("tan", Function("tan", 1, [](vector<double> args) {
    //     return tan(args[0]);
    // })));
    return map;
}

double FunctionHandler::HandleFunction(string name, vector<double> args, stringstream *err) {
    int count = functions.count(name);
    if(count <= 0) { // If there is no such function name defined in map functions, then throw an error and return 0
        if(!err) { err = new stringstream(); }
        *err << "ERROR: Function " << name << " does not exist";
        return 0;
    } else {
        if(args.size() != functions[name].params_num) { // If the number of arguments given is not equal to the number expected, throw an error
            if(!err) { err = new stringstream(); }
            *err << "ERROR: Function " << name << " expects " << functions[name].params_num << " arguments, you supplied " << args.size();
            return 0;
        } else {
            return functions[name].eval(args);
        }
    }
}