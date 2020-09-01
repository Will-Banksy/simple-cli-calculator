#include "ParseHandler.h"
#include <iostream>
#include "helpers/Helper.h"
#include <stack>

using namespace handlers;

std::vector<Element> ParseHandler::parse(std::string expr, bool cleanNegatives) {
	std::vector<Element> elems = std::vector<Element>();
	bool isInNum = false;
	bool isInStr = false;
	std::stringstream stream; // A string stream. Like how I'd use a Java StringBuilder
	for(int i = 0; i < expr.size(); i++) {
		char ch = expr[i];
		if(isOpeningBracket(ch)) {
// 			bool isNegative = i > 0 ? expr[i - 1] == '-' : false;
// 			isNegative = isNegative && (i > 1 ? isOperator(expr[i - 2]) : true); // If the character 2 back is an operator, or there isn't a character 2 back
// 			if(isNegative) { // If this open bracket is negative, then we need to delete the actual negative
//
// 			}
			elems.push_back(Element(BRACKET, true)); // If the character is a bracket, add it to the end of the vector
		} else if(isClosingBracket(ch)) {
			elems.push_back(Element(BRACKET, false));
		} else if(isOperator(ch)) {
			if(ch == ',') { // The comma counts as an honourary operator
				elems.push_back(Element(ARGUMENT_SEPARATOR));
			} else {
				elems.push_back(Element(OPERATOR, ch));
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
			} else if((isOpeningBracket(expr[i + 1]) || isClosingBracket(expr[i + 1]) || isOperator(expr[i + 1]))) {
				endOfStr:
				bool isConstant = i == expr.size() - 1 ? true : !isOpeningBracket(expr[i + 1]);
				isInStr = false;
				if(isConstant) {
					elems.push_back(Element(CONSTANT, stream.str()));
				} else {
					elems.push_back(Element(FUNCTION, stream.str()));
				}
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
				elems.push_back(Element(NUMBER, num));
			}
		}
	}

	if(cleanNegatives) {
		ParseHandler::cleanNegatives(elems);
	}

	// Just a debugging thing
	std::cout << "getElements: " << helpers::Helper::elemsToStr(elems);

	return elems;
}

std::vector<Element> ParseHandler::cleanNegatives(std::vector<Element>& elems, int start, int end) {
	if(end == -1) {
		end = elems.size();
	}
	for(int i = start; i < end; i++) {
		if(i > 0) {
			Element* curr = &elems.at(i);
			Element* prev = &elems.at(i - 1);
			Element* pprev = i > 1 ? &elems.at(i - 2) : nullptr;

			if(prev->type == OPERATOR && prev->op_value == '-') {
				if(curr->type == NUMBER) {
					bool currNeg = pprev ? ((pprev->type == OPERATOR && pprev->op_value != '!') || pprev->isOpenBracket()) : true; // 2! - 4 | 2 * -6 | 2 - 3
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
	return elems;
}

bool ParseHandler::check(std::vector<Element>& elems, std::stringstream* err) {
	// If the error stream is null, create a new one
	if(err == nullptr) {
		err = new std::stringstream();
	}

	if(elems.size() == 1) {
		Element& curr = elems.at(0);

		if(!curr.isNumber()) {
			*err << "ERROR: " << curr.toString() << " is not an expression" << std::endl;
			return false;
		}
	}

	bool inFunctionArgs = false;
	int bracketDepth = 0;
	std::stack<BracketInfo> brackets;
	for(int i = 0; i < elems.size(); i++) {
		Element& curr = elems.at(i);

		// Modify the tracking variables accordingly
		if(curr.isOpenBracket()) {
			bracketDepth++;
			inFunctionArgs = i > 0 ? elems.at(i - 1).type == FUNCTION : false;
			brackets.push(BracketInfo(i, inFunctionArgs, bracketDepth));
		} else if(curr.isCloseBracket()) {
			// Check if there is actually a bracket to close (aka if an opening bracket has come before this, at the same depth)
			if(brackets.empty()) {
				*err << "ERROR: Closing bracket before opening bracket " << std::endl;
				return false;
			}
			bracketDepth--;
			BracketInfo bracket = brackets.top();
			inFunctionArgs = bracket.hasFunction;
			brackets.pop();
		}

		if(i > 0) {
			Element& curr = elems.at(i);
			Element& prev = elems.at(i - 1);
			Element* pprev = i > 1 ? &elems.at(i - 2) : nullptr;

			// Check for misplaced ARGUMENT_SEPARATORs
			if(curr.type == ARGUMENT_SEPARATOR && !inFunctionArgs) {
				*err << "ERROR: Argument separator ',' not inside function" << std::endl;
				return false;
			}

			// Check for functions that don't have an opening bracket immediately after
			if(prev.type == FUNCTION && !curr.isOpenBracket()) {
				*err << "ERROR: Function " << prev.func_value << "must be followed by '('" << std::endl;
				return false;
			}

			// Check two numbers aren't somehow together
			if(curr.isNumber() && prev.isNumber()) {
				*err << "ERROR: Numbers " << curr.num_value << " and " << prev.num_value << " cannot be together" << std::endl;
				return false;
			}

			// Check if operators are in the correct places
			if(curr.isOperator('!')) {
				if(!(prev.isNumber() || prev.isCloseBracket())) { // If it's not after a number or closing bracket
					*err << "ERROR: Invalid placement of ! operator after " << prev.toString() << std::endl;
					return false;
				}
			} else if(elems.size() < 3 && curr.type == OPERATOR) { // In that case there's not enough room for an expression both sides of the operator
				*err << "ERROR: Operator " << curr.op_value << " must come after and be followed by an expression" << std::endl;
				return false;
			} else if(prev.type == OPERATOR && !prev.isOperator('!')) { // Check other operators
				if(pprev) {
					// This is the troublesome block of code, I think --
					// prev is the current operator
					// List of conditions this needs to satisfy (+ is any operator except !)
					// 0: a + b
					// 1: a + -b
					// 2: (...) + (...)
					// 3: (...) + b
					// 4: a + (...)
					// 5: a + -(...)
					// 6: (...) + -(...)
					// 7: a + fun(...)
					// 8: (...) + fun(...)
					// 9: a + -fun(...)
					//10: (...) + -fun(...)

					Element* next = i < elems.size() - 1 ? &elems.at(i + 1) : nullptr;

					// This method may not be the most efficient, but in this case it really doesn't matter, the difference would be negligible
					bool case0 = pprev->isNumber() && curr.isNumber();
					bool case1 = pprev->isNumber() && curr.isOperator('-') && (next ? next->isNumber() : false);
					bool case2 = pprev->isCloseBracket() && curr.isOpenBracket();
					bool case3 = pprev->isCloseBracket() && curr.isNumber();
					bool case4 = pprev->isNumber() && curr.isOpenBracket();
					bool case5 = pprev->isNumber() && curr.isOperator('-') && (next ? next->isOpenBracket() : false);
					bool case6 = pprev->isCloseBracket() && curr.isOperator('-') && (next ? next->isOpenBracket() : false);
					bool case7 = pprev->isNumber() && curr.type == FUNCTION; // Don't need to check if the function is valid another part of this method does that for me
					bool case8 = pprev->isCloseBracket() && curr.type == FUNCTION;
					bool case9 = pprev->isNumber() && curr.isOperator('-') && (next ? next->type == FUNCTION : false);
					bool case10= pprev->isCloseBracket() && curr.isOperator('-') && (next ? next->type == FUNCTION : false);

					bool isValidOperator = case0 || case1 || case2 || case3 || case4 || case5 || case6;
					if(!isValidOperator) {
						*err << "ERROR: Operator " << prev.op_value << " must come after and be followed by an expression" << std::endl;
						return false;
					}

					std::cout << "Successful Checking of Operator" << std::endl;
// 					bool isValidOperator = pprev->isCloseBracket() || pprev->isNumber() || pprev->isOperator('!');
// 					isValidOperator = isValidOperator && (curr.isOpenBracket() || curr.isNumber() || curr.type == FUNCTION);
// 					std::cout << "isValidOperator: " << isValidOperator << std::endl;
// 					bool isNegativeSymbolBracket = pprev->type == OPERATOR && prev.isOperator('-') && (curr.isOpenBracket()); // Need to explicitly check if this operator is actually a negative symbol for the bracket right after it
// 					std::cout << "isNegativeSymbolBracket: " << isNegativeSymbolBracket << std::endl;
// 					bool isBeforeNegative = i < elems.size() - 2 ? elems.at(i + 1).isOperator('-') : false;
// 					std::cout << "isBeforeNegative: " << isBeforeNegative << std::endl;
// 					isValidOperator = isBeforeNegative ? (pprev->isCloseBracket() || pprev->isNumber() || pprev->isOperator('!')) : isValidOperator; // If isBeforeNegative, then do some extra checks (the same as the first ones for isValidOperator)
// 					if(isValidOperator ^ isNegativeSymbolBracket) { // I have NEVER before needed to use XOR in my LIFE
// 						*err << "ERROR: Operator " << prev.op_value << " must come after and be followed by an expression" << std::endl;
// 						return false;
// 					}
					// --
				}
			}
		}
	}

	// Check if all the brackets were closed
	if(!brackets.empty()) {
		*err << "ERROR: Closing bracket missing" << std::endl;
		return false;
	}

	return true;
}