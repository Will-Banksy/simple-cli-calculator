// These two preprocessors, and the #endif at the end of the file, is a header guard - https://www.learncpp.com/cpp-tutorial/header-guards/
#ifndef ELEMENT_HEADER
#define ELEMENT_HEADER

#include <string>
#include <vector>

using namespace std;

namespace structs {
    class Function {
        public:
            string name;
            int params_num;
            double (*eval)(vector<double>); // A function as a variable

            Function(string name, int params_num, double (*eval)(vector<double>)); // Taking a pointer to a function as a parameter to this constructor
    };
}

#endif