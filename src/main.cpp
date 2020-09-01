#include <iostream>
#include <string>
#include <sstream>
#include "helpers/Helper.h"
#include "handlers/ParseHandler.h"
#include "structs/Expression.h"
#include "handlers/EvaluateHandler.h"
#include <array>

// Whitespace characters
char ws_[] = " \t\n\r\f\v";

bool isOneOf(char ch, char comp[], int complen) { // Take length of array as parameter
	for(int i = 0; i < complen; i++) {
		if(comp[i] == ch) {
			return true;
		}
	}
	return false;
}

// A whitespace trimming function (not from stack overflow, but here's a link that could be helpful) -- https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
std::string removeAll(std::string* str, int ws_arrsize = 6, char rem[] = ws_) { // Take length of array as parameter
	std::stringstream ss; // Automatically initialised

	for(int i = 0; i < str->length(); i++) {
		// If the current character is not a whitespace character, insert it into the stringstream
		if(!isOneOf(str->at(i), rem, ws_arrsize)) {
			ss << str->at(i);
		}
	}
	return ss.str();
}

// Start point
int main() {
	// Enter main loop
	while(true) {
		std::cout << "Enter thing to calculate: ";
		std::string str;
		// Use getline() because that gets the whole line, 'cin >> var' apparently only gets until the first whitespace
		getline(std::cin, str);

		// Trim all whitespaces from the string
		str = removeAll(&str); // Pass string pointer to method

		if(str == "exit") {
			break;
		}

		// Use ParseHandler to get a vector list of elements that make up an expression
		std::vector<Element> elems = handlers::ParseHandler::parse(str);

		std::stringstream* err = new std::stringstream();
		handlers::ParseHandler::check(elems, err);
		Expression expr = Expression(elems);

		std::cout << std::string(expr.elemsToString()) << std::endl;
		std::cout << err->str() << std::endl;

		handlers::EvaluateHandler::evaluate(elems);

		Expression expr2 = Expression(elems);
		std::cout << std::string(expr2.elemsToString()) << std::endl;
	}
}