// These two preprocessors, and the #endif at the end of the file, is a header guard - https://www.learncpp.com/cpp-tutorial/header-guards/
#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>

namespace structs {
	typedef double (*FunctionAction)(std::vector<double>); // It's much nicer to use a typedef for dealing with pointers to functions

    class Function {
        public:
            std::string name;
            int params_num;
            FunctionAction eval; // A function as a variable

            Function(std::string name, int params_num, FunctionAction action); // Taking a pointer to a function as a parameter to this constructor
    };
}

#endif // FUNCTION_H