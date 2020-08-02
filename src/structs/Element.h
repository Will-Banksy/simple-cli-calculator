// These two preprocessors, and the #endif at the end of the file, is a header guard - https://www.learncpp.com/cpp-tutorial/header-guards/
#ifndef ELEMENT_HEADER
#define ELEMENT_HEADER

#include <string>
#include <sstream>

namespace structs {
    enum Type {
        NUMBER,
        OPERATOR,
        FUNCTION,
		CONSTANT,
        BRACKET,
        ARGUMENT_SEPARATOR // ',', a comma
    };

    class Element {
        public:
            // Constructors
            Element(Type type, double val);
            Element(Type type, char val);
            Element(Type type, std::string val);
            Element(Type type, bool val);
            Element(Type type);
			Element(const Element& elem); // Copy constructor

            // Destructor
            ~Element();

            // The type of this element
            Type type;
            // The value of this element - which variable you use depends on the type of this element
            double num_value;
            char op_value;
            std::string func_value;
			std::string const_value;
            bool bracket_isopen;

            bool isOpenBracket();
            bool isCloseBracket();
			bool isOperator(char ch);
			/**
			 * Equivalent to type == NUMBER || type == CONSTANT
			 */
			bool isNumber();

			std::string toString();
    };
}

#endif