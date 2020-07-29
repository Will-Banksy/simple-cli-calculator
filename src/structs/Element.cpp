#include "Element.h"

using namespace structs;

Element::Element(Type type, double val) {
    this->type = type;
    num_value = val;

    op_value = 0;
    func_value = "";
    bracket_isopen = false;
}

Element::Element(Type type, char val) {
    this->type = type;
    op_value = val;

    num_value = 0;
    func_value = "";
    bracket_isopen = false;
}

Element::Element(Type type, std::string val) {
    this->type = type;
    func_value = val;

    num_value = 0;
    op_value = 0;
    bracket_isopen = false;
}

Element::Element(Type type, bool val) {
    this->type = type;
    bracket_isopen = val;

    num_value = 0;
    op_value = 0;
    func_value = "";
}

Element::Element(Type type) {
	this->type = type;
    num_value = 0;
    op_value = 0;
    func_value = "";
    bracket_isopen = false;
}

Element::~Element() {
    // Don't think I need to do anything here, as apparently you only need to manually deallocate memory if you initialise pointers with the 'new' operator
}

bool Element::isOpenBracket() {
    return type == BRACKET && bracket_isopen;
}

bool Element::isCloseBracket() {
    return type == BRACKET && !bracket_isopen;
}

bool structs::Element::isOperator(char ch) {
	return type == OPERATOR && op_value == ch;
}

std::string structs::Element::toString() {
	std::stringstream stream;
	switch(type) {
		case NUMBER:
			stream << "|num[" << num_value << "]func[" << func_value << "]type[" << type << "] |";
			break;

		case OPERATOR:
			stream << op_value;
			break;

		case ARGUMENT_SEPARATOR:
			stream << ",";
			break;

		case BRACKET:
			stream << (isOpenBracket() ? "(" : ")");
			break;

		case FUNCTION:
			stream << func_value;
			break;

		default:
			return "";
	}
	stream.flush();
	return stream.str();
}

std::string Element::toString(char ch) {
	std::stringstream str;
	str << ch;
	return str.str();
}

std::string structs::Element::toString(double num) {
	std::stringstream str;
	str << num;
	return str.str();
}