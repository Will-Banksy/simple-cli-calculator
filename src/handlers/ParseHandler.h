#ifndef PARSEHANDLER_H
#define PARSEHANDLER_H

#include <string>
#include <vector>
#include "src/structs/Element.h"

using namespace structs;

/**
 * @todo write docs
 */
class ParseHandler {
	public:
		vector<Element>* parse(std::string expr);

	private:
		// Some helper functions
		inline bool isLetter(char ch);
		inline bool isOpeningBracket(char ch);
		inline bool isClosingBracket(char ch);
		inline bool isOperator(char ch);
		inline bool isPartOfNum(char ch);
};

bool ParseHandler::isLetter(char ch) {
	return isalpha(ch);
}

bool ParseHandler::isOpeningBracket(char ch) {
	return ch == '(';
}

bool ParseHandler::isClosingBracket(char ch) {
	return ch == ')';
}

bool ParseHandler::isOperator(char ch) {
}

bool ParseHandler::isPartOfNum(char ch) {
	return (isdigit(ch) || ch == '.' || ch == 'e' || ch == 'E'); // Want to include '.' and e or E for scientific notation
}


#endif // PARSEHANDLER_H
