#include "Helper.h"
#include <sstream>
#include <math.h>
#include <algorithm>
#include <cctype>

using namespace helpers;

std::string Helper::elemsToStr(std::vector<Element> elems) {
	std::stringstream ss;
	for(Element elem : elems) {
		ss << "Element[type:" << elem.type;
		ss << ",num_val:" << elem.num_value;
		ss << ",op_val:" << elem.op_value;
		ss << ",func_val:" << elem.func_value;
		ss << ",const_val:" << elem.const_value;
		ss << ",bracket:" << elem.bracket_isopen;
		ss << "] " << std::endl;
	}
	return ss.str();
}

// Actually uses the gamma function: (The link is about python but it applies) https://stackoverflow.com/questions/10056797/python-calculate-factorial-of-a-non-integral-number#10056808
// The gamma function extends factorials to positive real numbers
double Helper::factorial(double num) {
	return tgamma(num + 1);
}

int Helper::find_last_bracket(std::vector<Element> elems, bool isopen) {
	for(int i = elems.size() - 1; i >= 0; i--) {
		if(elems[i].type == BRACKET && elems[i].bracket_isopen == isopen) {
			return i;
		}
	}
	return -1;
}

void Helper::tolower(std::string& str) {
	std::for_each(str.begin(), str.end(), [](char& ch) {
		ch = std::tolower(static_cast<unsigned char>(ch)); // std::tolower expects unsigned chars
	});
}

double Helper::deg(double* rad) {
	return *rad * (180.0 / M_PI);
}

double Helper::rad(double* deg) {
	return *deg * (M_PI / 180.0);
}