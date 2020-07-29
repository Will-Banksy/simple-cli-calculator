#include "Expression.h"
#include <iostream>

Expression::Expression(std::vector<Element> elems, int start, int end) {
	if(start == 0 & end == -1) {
		this->elems = &elems;
	} else {
		this->elems = new std::vector<Element>();
		int j = 0;
		for(int i = start; i < end; i++) {
			this->elems->at(j) = elems.at(i);
			j++;
		}
	}

	std::cout << "Type: " << this->elems->at(0).type << std::endl;

	// Check if there is an invalid element in the expression. Invalid elements are: Commas, Brackets
	/*for(Element elem : *this->elems) {
		if(elem.type == BRACKET || elem.type == ARGUMENT_SEPARATOR) {
			// Do fatal error thing here
			this->elems = nullptr;
		}
	}*/
}

bool Expression::isValid(std::stringstream* err) {
	std::string elemsStr = elemsToString();
	// Check if there is an invalid element in the expression. Invalid elements are: Commas, Brackets, Functions
	for(Element elem : *this->elems) {
		if(elem.type == BRACKET || elem.type == ARGUMENT_SEPARATOR || elem.type == FUNCTION) {
			// Do fatal error thing here
			*err << "ERROR: " << elemsStr << " is not an expression\n";
			return false;
		}
	}

	if(elems->at(0).type == OPERATOR) {
		*err << "ERROR: Operator " << elems->at(0).op_value << " cannot be at the start of an expression\n";
		return false;
	}

	if(elems->at(elems->size() - 1).type == OPERATOR && elems->at(elems->size() - 1).op_value != '!') {
		*err << "ERROR: Operator " << elems->at(elems->size() - 1).op_value << " cannot be at the end of an expression\n";
		return false;
	}

	for(int i = 0; i < elems->size(); i++) {
		if(i > 0) {
			Element* curr = &elems->at(i);
			Element* prev = &elems->at(i - 1);

			if(prev->type == NUMBER) {
				if(curr->type == NUMBER) {
					*err << "ERROR: Numbers " << prev->num_value << " and " << curr->num_value << " cannot be together";
				}
			} else if(prev->type == OPERATOR) {
				if(curr->type == OPERATOR && prev->op_value != '!') {
					*err << "ERROR: Operators " << prev->op_value << " and " << curr->op_value << " cannot be together";
				}
			}
		}
	}

	return true;
}


std::string Expression::elemsToString() {
	std::stringstream stream;
	stream << "> ";
	for(int i = 0; i < elems->size(); i++) {
		stream << elems->at(i).toString() << "<-type=" << elems->at(i).type << " ¬ "; // FIXME: For some reason, in this method, the first element is found to always have a type of zero? Even if it has it's proper type in the constructor??
	}
	stream.flush();
	return stream.str();
}

std::vector<Element>* Expression::parse() {
	for(int i = 0; i < elems->size(); i++) {
		if(i > 0) {
			Element* curr = &elems->at(i);
			Element* prev = &elems->at(i - 1);
			Element* pprev = i > 1 ? &elems->at(i - 2) : nullptr;

			if(prev->type == OPERATOR && prev->op_value == '-') {
				if(curr->type == NUMBER) {
					bool currNeg = pprev ? (pprev->type == OPERATOR && pprev->op_value != '!') : true; // 2! - 4 | 2 * -6 | 2 - 3
					if(currNeg) {
						curr->num_value *= -1;
						elems->erase(elems->begin() + i - 1);
						i--;
						continue;
					}
				}
			}
		}
	}
	return elems;
}
