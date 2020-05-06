// These two preprocessors, and the #endif at the end of the file, is a header guard - https://www.learncpp.com/cpp-tutorial/header-guards/
#ifndef ELEMENT_HEADER
#define ELEMENT_HEADER

#include <string>

using namespace std;

namespace structs {
    enum Type {
        NUMBER,
        OPERATOR,
        FUNCTION,
        BRACKET,
        ARGUMENT_SEPARATOR // ',', a comma
    };

    class Element {
        public:
            // Constructors
            Element(Type type, double val);
            Element(Type type, char val);
            Element(Type type, string val);
            Element(Type type, bool val);
            Element(Type type);

            // Destructor
            ~Element();

            // The type of this element
            Type type;
            // The value of this element - which variable you use depends on the type of this element
            double num_value;
            char op_value;
            string func_value;
            bool bracket_isopen;

            bool isOpenBracket();
            bool isCloseBracket();
    };
}

#endif