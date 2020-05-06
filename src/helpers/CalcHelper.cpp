#include "CalcHelper.h"
#include <sstream>
#include <iostream>
#include "Helper.h"
#include <math.h>

// Remember that #including something #includes everything #included by that something
// Also remember that using something uses everything used by that something

using namespace helpers;
using namespace structs;

vector<char> CalcHelper::operators = vector<char>({ '+', '-', '*', '/', '^', '!' });
vector<char> CalcHelper::ops_bodmas = vector<char>({ '^', '/', '*', '-', '+' });
map<string, double> CalcHelper::constants = createMap();
AngleMode CalcHelper::mode = DEGREES;

double CalcHelper::calculate(char chars[], int arrlen) {
    // auto is like var in Java or C# or whatever
    auto elems = CalcHelper::getElements(chars, arrlen);
    return CalcHelper::doCalculations(elems);
}

double CalcHelper::doCalculations(std::vector<Element> &elements) { // TODO: Allow multi-parameter functions. Also expand functions. Add constants too
    // Do a check for illegal element combinations

    // Constants will be assumed to be functions, so here we can sort that out

    stringstream* err = NULL;
    int numopenbr = 0;
    int numclosebr = 0;
    for(int i = 0; i < elements.size(); i++) {
        if(i < elements.size() - 1) {
            if(elements[i].type == FUNCTION && elements[i + 1].type == FUNCTION) { // If two functions are next to each other
                if(!err) { err = new stringstream(); }
                *err << "ERROR: Functions " << elements[i].func_value << " and " << elements[i + 1].func_value << " cannot be adjacent\n";
            }
            if(elements[i].type == NUMBER && elements[i + 1].type == NUMBER) { // If two numbers are next to each other
                if(!err) { err = new stringstream(); }
                *err << "ERROR: Numbers " << elements[i].num_value << " and " << elements[i + 1].num_value << " cannot be adjacent\n";
            }
            if(elements[i].type == BRACKET && elements[i + 1].type == BRACKET && elements[i].bracket_isopen && !elements[i + 1].bracket_isopen) { // If the pattern '()' appears
                if(!err) { err = new stringstream(); }
                *err << "ERROR: Brackets cannot appear as '()'\n";
            }
            if(elements[i].type == ARGUMENT_SEPARATOR && (elements[i + 1].type == ARGUMENT_SEPARATOR || (elements[i + 1].type == BRACKET && !elements[i + 1].bracket_isopen))) {
                if(!err) { err = new stringstream(); }
                *err << "ERROR: Empty argument";
            }
            if(elements[i].type == BRACKET && elements[i].bracket_isopen && elements[i + 1].type == ARGUMENT_SEPARATOR) {
                if(!err) { err = new stringstream(); }
                *err << "ERROR: Empty argument";
            }
        }
        if(elements[i].type == OPERATOR && elements[i].op_value != '!') { // If this element is an operator that is not '!' and is followed by a ')' or is at the end of the string
            if(i == elements.size() - 1) {
                goto submitError;
            }
            if(i < elements.size() - 1) {
                if(elements[i + 1].type == BRACKET && !elements[i + 1].bracket_isopen) {
                    submitError:
                    if(!err) { err = new stringstream(); }
                    *err << "ERROR: Operator " << elements[i].op_value << " must be followed by an expression\n";
                }
            }
        }
        if(elements[i].type == FUNCTION) { // If this element is a function and the next element isn't '(', we'll assume it was meant to be a constant
            if(i == elements.size() - 1) {
                goto fnAtEnd;
            } else if(!(elements[i + 1].type == BRACKET && elements[i + 1].bracket_isopen)) {
                fnAtEnd:
                int count = constants.count(elements[i].func_value); // Can only return 0 and 1, because of the nature of a map
                if(count == 1) {
                    double val = constants[elements[i].func_value];
                    elements[i] = Element(NUMBER, val);
                } else {
                    if(!err) { err = new stringstream(); }
                    *err << "ERROR: Constant " << elements[i].func_value << " does not exist";
                }
            }
        }
        if(elements[i].type == BRACKET) {
            if(elements[i].bracket_isopen) {
                numopenbr++;
            } else {
                numclosebr++;
            }
        }
    }
    if(numclosebr != numopenbr) { // If the number of opening brackets is not the same as the number of closing brackets
        if(!err) { err = new stringstream(); }
        *err << "ERROR: Number of closing and opening brackets must be equal\n";
    }

    double ans = 0;
    while(true) {
        if(err) { // Check for errors
            handleErrors:
            err->flush();
            cout << err->str() << endl; // endl flushes the stream
            return 0;
        }

        if(elements.size() == 1 && elements[0].type == NUMBER) {
            ans = elements[0].num_value;
            break;
        }

        for(int i = 1; i < elements.size() - 1; i++) { // Replace '(n)' with 'n'. Also do any functions
            if(elements[i].type == NUMBER && elements[i - 1].type == BRACKET && elements[i - 1].bracket_isopen && elements[i + 1].type == BRACKET && !elements[i + 1].bracket_isopen) {
                if(i > 1) {
                    if(elements[i - 2].type == FUNCTION) {
                        string elemStr = elements[i - 2].func_value;
                        Helper::tolower(elemStr); // Convert elemStr to lowercase
                        double ans = elements[i].num_value;
                        double ang = mode == DEGREES ? Helper::rad(&elements[i].num_value) : elements[i].num_value;
                        if(elemStr == "sin") { // switch doesn't accept strings
                            ans = sin(ang);
                        } else if(elemStr == "cos") {
                            ans = cos(ang);
                        } else if(elemStr == "tan") {
                            ans = tan(ang);
                        } else {
                            if(!err) { err = new stringstream(); }
                            *err << "ERROR: Function " << elements[i - 2].func_value << " does not exist\n";
                            goto handleErrors;
                        }
                        elements[i].num_value = ans;
                        elements.erase(elements.begin() + i - 2);
                        i--;
                    }
                }
                
                elements.erase(elements.begin() + i + 1);
                elements.erase(elements.begin() + i - 1);
            }
        }

        for(int i = 0; i < elements.size(); i++) { // Evaluate factorials
            if(elements[i].type == NUMBER && elements[i + 1].type == OPERATOR && elements[i + 1].op_value == '!') {
                double num = Helper::factorial(elements[i].num_value);
                elements[i].num_value = num;
                elements.erase(elements.begin() + i + 1); // vector.erase takes an iterator, NOT an index. elements.begin() is an iterator pointing to the start of the vector, and we can just add to it to get to the index we want
            }
        }

        for(int i = 0; i < elements.size() - 1; i++) { // Deal with negatives
            if(elements[i].type == OPERATOR && elements[i].op_value == '-' && elements[i + 1].type == NUMBER) {
                if(i == 0) {
                    goto dealWithNegative;
                } else if(elements[i - 1].type == OPERATOR || (elements[i - 1].type == BRACKET && elements[i - 1].bracket_isopen) || elements[i - 1].type == ARGUMENT_SEPARATOR) {
                    dealWithNegative:
                    elements[i + 1].num_value *= -1; // Make the number negative
                    elements.erase(elements.begin() + i); // And delete the '-'
                }
            }
        }

        // Find the innermost 'section' i.e. bracket pair or between two ARGUMENT_SEPARATORs (commas)
        int start = Helper::find_last_bracket(elements, true);
        if(start == -1) { start = 0; }
        int end = elements.size();
        vector<int> arg_seps;
        for(int i = start; i < elements.size(); i++) {
            if(elements[i].type == ARGUMENT_SEPARATOR) {
                arg_seps.push_back(i);
            }
            if(elements[i].type == BRACKET && !elements[i].bracket_isopen) {
                end = i;
                break;
            }
        }
        
        cout << "start: " << start << " end: "<< end << " elemsSize: " << elements.size() << endl;
        for(int o = 0; o < ops_bodmas.size(); o++) {
            for(int i = start + 1; i < end - 1; i++) { // Now, do all the calculations in that little section
                if(elements[i].type == OPERATOR && elements[i - 1].type == NUMBER && elements[i + 1].type == NUMBER && elements[i].op_value == ops_bodmas[o]) {
                    double ans = 0;
                    switch (ops_bodmas[o])
                    {
                        case '^':
                            ans = pow(elements[i - 1].num_value, elements[i + 1].num_value);
                            break;

                        case '/':
                            if(elements[i + 1].num_value == 0) {
                                if(!err) { err = new stringstream(); }
                                *err << "ERROR: Attempted division of " << elements[i - 1].num_value << " by zero";
                                goto handleErrors;
                            }
                            ans = elements[i - 1].num_value / elements[i + 1].num_value;
                            break;

                        case '*':
                            ans = elements[i - 1].num_value * elements[i + 1].num_value;
                            break;

                        case '-':
                            ans = elements[i - 1].num_value - elements[i + 1].num_value;
                            break;

                        case '+':
                            ans = elements[i - 1].num_value + elements[i + 1].num_value;
                            break;
                    }
                    elements[i - 1].num_value = ans;
                    elements.erase(elements.begin() + i + 1);
                    elements.erase(elements.begin() + i);
                    cout << "ERASE" << endl;
                }
            }
        }
    }

    cout << "New elems: " << Helper::elemsToStr(elements);

    return ans; // TODO: Maybe actually do something here
}

vector<Element> CalcHelper::getElements(char chars[], int arrlen) {
    vector<Element> elems;
    bool isInNum = false;
    bool isInStr = false;
    stringstream stream; // A string stream. Like how I'd use a Java StringBuilder, 
    for(int i = 0; i < arrlen; i++) {
        char ch = chars[i];
        if(isOpeningBracket(ch)) {
            elems.push_back(Element(BRACKET, true)); // If the character is a bracket, add it to the end of the vector
        } else if(isClosingBracket(ch)) {
            elems.push_back(Element(BRACKET, false));
        } else if(isOperator(ch)) {
            if(ch == ',') {
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
            if(i == arrlen - 1) {
                goto endOfStr; // We want to do the same stuff as in the next else if, so just goto that label. This is my version of a 'safe' OR, at least in this context
            } else if(!(/*isLetter(chars[i + 1]) || */(!isOpeningBracket(chars[i + 1]) && !isClosingBracket(chars[i + 1]) && !isOperator(chars[i + 1])))) {
                endOfStr:
                isInStr = false;
                elems.push_back(Element(FUNCTION, stream.str()));
            }
        } else if(isPartOfNum(ch) && !isInStr) { // If the current character is part of a number
            if(!isInNum) {
                isInNum = true;
                stream.str(""); // stringstream.clear doesn't clear the stream contents, this does
            }
            // Insert character into stream
            stream << ch;

            // If we have reached the end of the number, do the appropriate things
            if(i == arrlen - 1) {
                goto endOfNum; // We want to do the same stuff as in the next else if, so just goto that label. This is my version of a 'safe' OR, at least in this context
            } else if(!isPartOfNum(chars[i + 1])) {
                endOfNum:
                isInNum = false;
                double num = stod(stream.str()); // Parse the string from the stringstream as a double
                elems.push_back(Element(NUMBER, num));
            }
        }
    }

    // Just a debugging thing
    cout << "getElements: " << Helper::elemsToStr(elems);

    return elems;
}

bool CalcHelper::isLetter(char ch) {
    return isalpha(ch);
}

bool CalcHelper::isOperator(char ch) {
    for(int i = 0; i < operators.size(); i++) {
        if(ch == operators[i]) {
            return true;
        }
    }
    return ch == ',';
}

bool CalcHelper::isOpeningBracket(char ch) {
    return ch == '(';
}

bool CalcHelper::isClosingBracket(char ch) {
    return ch == ')';
}

bool CalcHelper::isPartOfNum(char ch) {
    return (isdigit(ch) || ch == '.' || ch == 'e' || ch == 'E'); // Want to include '.' and e or E for scientific notation
}

map<string, double> CalcHelper::createMap() {
    map<string, double> map;
    map["pi"] = M_PI;
    map["e"] = M_E;
    map["tau"] = M_PI * 2;
    return map;
}