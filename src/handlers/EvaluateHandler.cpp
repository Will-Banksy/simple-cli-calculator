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
	// Of course, I could create my own number type that scales it's max/min values indefinetely, but that'd be a lot of work - probably unjustified. Especially since it'd complicate everything so much. Yeah not worth it XD

	// Do calculations ---

	// First substitute any constants
	for(int i = 0; i < elems.size(); i++) {
		if(elems.at(i).type == CONSTANT) {
			if(CalculationHandler::constants.count(elems.at(i).const_value) > 0) {
				elems[i] = Element(NUMBER, CalculationHandler::constants[elems.at(i).const_value]);
			}
		}
	}

	// Then evaluate any factorials
	for(int i = 0; i < elems.size(); i++) {
		if(elems.at(i).isOperator('!') && !elems.at(i - 1).isCloseBracket()) { // Check it's not a '(...)!' situation
			short rem = 1;
			applyOperator(elems, i, rem);
			continue;
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