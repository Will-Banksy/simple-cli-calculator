#include <iostream>
#include <string>
#include <sstream>
#include "helpers/Helper.h"
#include "handlers/ParseHandler.h"
#include "structs/Expression.h"
#include "handlers/EvaluateHandler.h"

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
    while(true) {
        std::cout << "Enter thing to calculate: ";
        std::string str;
        // Use getline() because that gets the whole line, 'cin >> var' apparently only gets until the first space
        getline(std::cin, str);

        // Trim all whitespaces from the string
        str = removeAll(&str); // Pass string pointer to method

		if(str == "exit") {
			break;
		}

        std::cout << "new string: " << str << std::endl;

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

		// Use EvaluateHandler to calculate the value of the expression
	}
}

		/*

        // Break loop if the string is "exit" (Note: the equality operator compares the actual string content)
        if(str == "exit") {
            break;
        } else if(str == "deg") {
            helpers::CalcHelper::mode = helpers::DEGREES;
            cout << "Switched to mode: DEGREES" << endl;
            continue;
        } else if(str == "rad") {
            helpers::CalcHelper::mode = helpers::RADIANS;
            cout << "Switched to mode: RADIANS" << endl;
            continue;
        }

        // Char array to hold the input
        char input[str.size()];
        // Copy str into input
        str.copy(input, str.size());
        // An array of characters requires a '\0' character to print properly, as an array of characters is a c-style string, where the end is the first '\0'
        // Actually no it doesn't, usually, but it's probably a good idea, and it doesn't hurt
        // For more info: https://stackoverflow.com/questions/14183445/what-does-0-mean
        // cout << input + '\0' << endl;

        int arrlen = sizeof(input) / sizeof(input[0]);
        double ans = helpers::CalcHelper::calculate(input, arrlen);
        cout << ">> " << ans << endl;
    }
}*/