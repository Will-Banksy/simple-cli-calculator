#include "EvaluateHandler.h"
#include "ParseHandler.h"
#include <iostream>
#include "helpers/Helper.h"
#include <cmath>

using namespace handlers;

double EvaluateHandler::evaluate(std::vector<Element>& elems, std::stringstream* errStream) {
	bool toStdout = false;
	if(!errStream) {
		errStream = new std::stringstream();
		toStdout = true; // If a stringstream hasn't been provided, print errors to standard output
	}
	std::stringstream& err = *errStream;

	bool syntaxErrors = !ParseHandler::check(elems, &err);
	if(syntaxErrors) {
		if(toStdout) {
			std::cout << err.str();
			std::cout.flush();
		}
		return 0;
	}

	// Now we can do all the calculations under the assumption there will be no syntax errors. Cause if there is then there's something wrong with my checker
	// We do have to check for runtime errors such as division by zero. (Not this but it's good: https://fantasticmetropolis.com/i/division)
	// Also some of the maths functions will throw errors under certain situations, such as overflow (too big numbers) - so I do need to do error checking there

	// Do calculations ---

	// First substitute any constants
	for(int i = 0; i < elems.size(); i++) {
		if(elems.at(i).type == CONSTANT) {
			if(CalculationHandler::constants.count(elems.at(i).const_value) > 0) {
				elems[i] = Element(NUMBER, CalculationHandler::constants[elems.at(i).const_value]);
			} else {
				(toStdout ? std::cout : err) << "ERROR: Constant " << elems.at(i).const_value << " not defined" << std::endl;
				return 0;
			}
		}
	}

	// Now how am I actually going to approach this?
	// Maybe I can get a list of all the brackets, and evaluate them that way? Or each iteration, get the innermost bracket pair?
	// Or I could split it into vector of snippets, that I can then evaluate one by one?

	// TODO: Implement approach
	// Current approach decided:
	// Get a list of all brackets
	// Loop, evaluating the innermost pair each iteration
	// When there are no brackets left, evaluate the whole expression the same as the contents of the brackets

	// A list of all bracket pairs will be useful
	std::vector<BracketInfo> brackets;
	int bracketDepth = 0;
	for(int i = 0; i < elems.size(); i++) {
		Element& curr = elems.at(i);

		if(curr.isOpenBracket()) {
			bracketDepth++;
			bool function = i > 0 ? elems.at(i - 1).type == FUNCTION : false;
			BracketInfo currBracket = BracketInfo(i, function, bracketDepth);
			currBracket.closePos = findCloseBracket(elems, i);
			if(currBracket.closePos != -1) {
				brackets.push_back(currBracket);
			}
		}
	}

	std::cout << "Brackets Len: " << brackets.size() << std::endl;

	// Enter main loop for evaluating. Maybe.
	int maxIterations = 1000;
	int iteration = 0;
	while(elems.size() > 1) {
		// Then evaluate any factorials
// 		for(int i = 0; i < elems.size(); i++) {
// 			if(elems.at(i).isOperator('!') && !elems.at(i - 1).isCloseBracket()) { // Check it's not a '(...)!' situation
// 				short rem = 1;
// 				applyOperator(elems, i, rem);
// 				continue;
// 			}
// 		}

		// We need the innermost bracket pair, so that there won't be any brackets inside them
		BracketInfo* innermost = nullptr;
		int innermostIndex = -1;
		for(int i = 0; i < brackets.size(); i++) {
			bool isMoreInner = false;
			if(!innermost) {
				isMoreInner = true;
			} else if(brackets.at(i).depth > innermost->depth) {
				isMoreInner = true;
			}
			if(isMoreInner) {
				innermost = &brackets.at(i);
				innermostIndex = i;
			}
		}

		// If an innermost bracket has been found
		if(innermost) {
			if(!innermost->hasFunction) { // FIXME: Elements are not removed/replaced correctly after evaluating a snippet
				evalSnippet(elems, innermost->openPos, innermost->closePos); // FIXME: Problem with factorials
				elems[innermost->openPos] = elems.at(innermost->openPos + 1);
				elems.erase(elems.begin() + innermost->openPos + 1);
				std::cout << helpers::Helper::elemsToStr(elems) << std::endl;
				brackets.erase(brackets.begin() + innermostIndex);
			} else {
				// TODO: Handle functions with arguments
			}
		}

		// Break out of loop if we've looped more than 1000 times - if we've not got the answer by now we probably won't in the future
		iteration++;
		if(iteration > maxIterations) {
			std::cout << "Could not calculate answer" << std::endl;
			break;
		}
	}

	return 0;
}

void EvaluateHandler::applyOperator(std::vector<Element>& elems, int opIndex, short& numRemoved) {
	Element& op = elems.at(opIndex);
	Element& before = elems.at(opIndex - 1);

	if(op.isOperator('!')) {
		before.num_value = helpers::Helper::factorial(before.num_value);
		elems.erase(elems.begin() + opIndex);
		numRemoved = 1;
		return;
	}

	Element& after = elems.at(opIndex + 1);

	double newNum = 0;
	switch(op.op_value) {
		case '^':
			newNum = pow(before.num_value, after.num_value);
			goto replaceElements;

		case '*':
			newNum = before.num_value * after.num_value;
			goto replaceElements;

		case '%':
			newNum = fmod(before.num_value, after.num_value); // fmod seems to be the floating-point equivalent to modulo - https://stackoverflow.com/questions/9138790/cant-use-modulus-on-doubles#9138794
			goto replaceElements;

		case '/':
			newNum = before.num_value / after.num_value;
			goto replaceElements;

		case '+':
			newNum = before.num_value + after.num_value;
			goto replaceElements;

		case '-':
			newNum = before.num_value - after.num_value;
			goto replaceElements;

		default:
			replaceElements:
			before.num_value = newNum;
			elems.erase(elems.begin() + opIndex, elems.begin() + opIndex + 1);
			numRemoved = 2;
			break;
	}
}

int EvaluateHandler::findCloseBracket(std::vector<Element>& elems, int afterIndex) {
	int depth = 0;
	// afterIndex + 1 so we don't start directly at the opening bracket
	for(int i = afterIndex + 1; i < elems.size(); i++) {
		if(elems.at(i).isOpenBracket()) {
			depth++;
		} else if(elems.at(i).isCloseBracket()) {
			if(depth == 0) {
				return i;
			}
		}
	}
	return -1;
}

void EvaluateHandler::evalSnippet(std::vector<Element>& elems, int start, int end) {
	std::cout << "len: " << elems.size() << " start: " << start << " end: " << end << std::endl;
	for(int i = start; i < end; i++) {
		Element& curr = elems.at(i);

		if(curr.type == OPERATOR) {
			short numRem = 0;
			applyOperator(elems, i, numRem);
			i -= numRem;
		}
	}
}
