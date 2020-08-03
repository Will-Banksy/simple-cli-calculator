#include "Expression.h"
#include "handlers/ParseHandler.h"

Expression::Expression(std::vector<Element> elems, int start, int end) {
	if(start == 0 & end == -1) {
		end = elems.size();
	}

	this->elems = std::vector<Element>();
	this->elems.resize(elems.size(), Element(NUMBER)); // Provide the constructor for adding new elements
	int j = 0;
	for(int i = start; i < end; i++) {
		this->elems.at(j) = Element(elems.at(i));
		j++;
	}

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
	for(Element elem : this->elems) {
		if(elem.type == BRACKET || elem.type == ARGUMENT_SEPARATOR || elem.type == FUNCTION) {
			*err << "ERROR: " << elemsStr << " is not an expression" << std::endl;
			return false;
		}
	}

	return handlers::ParseHandler::check(elems, err);
}


std::string Expression::elemsToString() {
	std::stringstream stream;
	stream << "> ";
	for(int i = 0; i < elems.size(); i++) {
		stream << elems.at(i).toString();
	}
	stream.flush();
	return stream.str();
}

std::vector<Element>* Expression::parse() {
	for(int i = 0; i < elems.size(); i++) {
		if(i > 0) {
			Element* curr = &elems.at(i);
			Element* prev = &elems.at(i - 1);
			Element* pprev = i > 1 ? &elems.at(i - 2) : nullptr;

			if(prev->type == OPERATOR && prev->op_value == '-') {
				if(curr->type == NUMBER) {
					bool currNeg = pprev ? (pprev->type == OPERATOR && pprev->op_value != '!') : true; // 2! - 4 | 2 * -6 | 2 - 3
					if(currNeg) {
						curr->num_value *= -1;
						elems.erase(elems.begin() + i - 1);
						i--;
						continue;
					}
				}
			}
		}
	}
	return &elems;
}
