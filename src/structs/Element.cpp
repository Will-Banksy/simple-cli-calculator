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

Element::Element(Type type, string val) {
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

std::string structs::Element::toString() {
	switch(type) {
		case NUMBER:
			return std::to_string(num_value);

		case OPERATOR:
			return std::to_string(op_value);

		case ARGUMENT_SEPARATOR:
			return ",";

		case BRACKET:
			return isOpenBracket() ? "(" : ")";

		case FUNCTION:
			return func_value;

		default:
			return "";
	}
}
