// These two preprocessors, and the #endif at the end of the file, is a header guard - https://www.learncpp.com/cpp-tutorial/header-guards/
#ifndef FNHANDLER_HEADER
#define FNHANDLER_HEADER

#include <vector>
#include <string>
#include <map>
#include "../structs/Function.h"

using namespace std;
using namespace structs;

namespace helpers {
    class FunctionHandler {
        public:
            static double HandleFunction(string funName, vector<double> args, stringstream* err);
            static map<string, Function> initFunctions();

        private:
            static map<string, Function> functions;
    };
}

#endif