#include "ParseHandler.h"
#include <iostream>
#include "src/helpers/Helper.h"

vector<Element>* ParseHandler::parse(std::string expr) {
	vector<Element>* elems = new vector<Element>();
	bool isInNum = false;
	bool isInStr = false;
	stringstream stream; // A string stream. Like how I'd use a Java StringBuilder
	for(int i = 0; i < expr.size(); i++) {
		char ch = expr[i];
		if(isOpeningBracket(ch)) {
			elems->push_back(Element(BRACKET, true)); // If the character is a bracket, add it to the end of the vector
		} else if(isClosingBracket(ch)) {
			elems->push_back(Element(BRACKET, false));
		} else if(isOperator(ch)) {
			if(ch == ',') {
				elems->push_back(Element(ARGUMENT_SEPARATOR));
			} else {
				elems->push_back(Element(OPERATOR, ch));
			}
		} else if((isLetter(ch) || (isInStr && !isOpeningBracket(ch) && !isClosingBracket(ch) && !isOperator(ch))) && !isInNum) {
			if(!isInStr) {
				isInStr = true;
				stream.str(""); // stringstream.clear doesn't clear the stream contents, this does
			}
			// Insert character into stream
			stream << ch;

			// If we have reached the end of the letter sequence, do the appropriate things
			if(i == expr.size() - 1) {
				goto endOfStr; // We want to do the same stuff as in the next else if, so just goto that label. This is my version of a 'safe' OR, at least in this context
			} else if(!(/*isLetter(expr[i + 1]) || */(!isOpeningBracket(expr[i + 1]) && !isClosingBracket(expr[i + 1]) && !isOperator(expr[i + 1])))) {
				endOfStr:
				isInStr = false;
				elems->push_back(Element(FUNCTION, stream.str()));
			}
		} else if(isPartOfNum(ch) && !isInStr) { // If the current character is part of a number
			if(!isInNum) {
				isInNum = true;
				stream.str(""); // stringstream.clear doesn't clear the stream contents, this does
			}
			// Insert character into stream
			stream << ch;

			// If we have reached the end of the number, do the appropriate things
			if(i == expr.size() - 1) {
				goto endOfNum; // We want to do the same stuff as in the next else if, so just goto that label. This is my version of a 'safe' OR, at least in this context
			} else if(!isPartOfNum(expr[i + 1])) {
				endOfNum:
				isInNum = false;
				double num = stod(stream.str()); // Parse the string from the stringstream as a double
				elems->push_back(Element(NUMBER, num));
			}
		}
	}

	// Just a debugging thing
	std::cout << "getElements: " << helpers::Helper::elemsToStr(*elems);

	return elems;
}
