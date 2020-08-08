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
	if(this->type == FUNCTION) {
		func_value = val;
		const_value = "";
	} else if(this->type == CONSTANT) {
		const_value = val;
		func_value = "";
	}

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
	const_value = "";
	bracket_isopen = false;
}

Element::Element(const Element& elem) {
	type = elem.type;
	num_value = elem.num_value;
	op_value = elem.op_value;
	func_value = elem.func_value;
	const_value = elem.const_value;
	bracket_isopen = elem.bracket_isopen;
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

bool Element::isOperator(char ch) {
	return type == OPERATOR && op_value == ch;
}

bool Element::isNumber() {
	return type == NUMBER || type == CONSTANT;
}

std::string Element::toString() {
	std::stringstream stream;
	switch(type) {
		case NUMBER:
			stream << num_value;
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

		case CONSTANT:
			stream << const_value;
			break;

		default:
			return "";
	}
	stream.flush();
	return stream.str();
}