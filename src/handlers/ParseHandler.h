#ifndef PARSEHANDLER_H
#define PARSEHANDLER_H

#include <string>
#include <vector>
#include "structs/Element.h"
#include "CalculationHandler.h"

using namespace structs;

namespace handlers {
	class ParseHandler {
		public:
			static std::vector<Element> parse(std::string expr, bool cleanNegatives = true);
			static std::vector<Element> cleanNegatives(std::vector<Element>& elems);
			/**
			 * Returns false if there are syntax errors, true otherwise. Requires negatives to be cleaned up
			 */
			static bool check(std::vector<Element>& elems, std::stringstream* err = nullptr);

		private:
			struct BracketInfo { // Default visibility for structs is public - for classes it's private
				int openPos;
				int closePos;
				bool hasFunction;
				int depth;

				BracketInfo(int openPos, bool hasFunction, int depth) {
					this->openPos = openPos;
					this->hasFunction = hasFunction;
					this->depth = depth;
				}
			};

			// Some helper functions
			inline static bool isLetter(char ch);
			inline static bool isOpeningBracket(char ch);
			inline static bool isClosingBracket(char ch);
			inline static bool isOperator(char ch);
			inline static bool isPartOfNum(char ch);
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
		for(int i = 0; i < CalculationHandler::operators.size(); i++) {
			if(ch == CalculationHandler::operators[i]) {
				return true;
			}
		}
		return ch == ',';
	}

	bool ParseHandler::isPartOfNum(char ch) {
		return (isdigit(ch) || ch == '.' || ch == 'e' || ch == 'E'); // Want to include '.' and e or E for scientific notation
	}
}

#endif // PARSEHANDLER_H