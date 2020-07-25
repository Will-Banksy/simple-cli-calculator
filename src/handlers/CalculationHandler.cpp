#include "CalculationHandler.h"
#include <cmath>

std::vector<char> CalculationHandler::operators = { '+', '-', '*', '/', '^', '!' };
std::vector<char> CalculationHandler::ops_bodmas = { '^', '/', '*', '-', '+' };
std::map<std::string, double> CalculationHandler::constants = std::map<std::string, double>();

void CalculationHandler::initialiseConstants() {
	constants = std::map<std::string, double>();
	constants["pi"] = M_PI;
	constants["e"] = M_E;
	constants["tau"] = M_PI * 2;
}