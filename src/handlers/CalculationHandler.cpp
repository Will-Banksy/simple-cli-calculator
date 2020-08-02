#include "CalculationHandler.h"
#include <math.h>

using namespace handlers;

std::vector<char> CalculationHandler::operators = { ',', '+', '-', '*', '/', '^', '!', '%' };
// std::vector<char> CalculationHandler::ops_bodmas = { '^', '/', '*', '-', '+' };
std::map<std::string, double> CalculationHandler::constants = std::map<std::string, double>();
std::vector<Function> CalculationHandler::functions = getFunctions();

void CalculationHandler::initialiseConstants() {
	constants = std::map<std::string, double>();
	constants["pi"] = M_PI;
	constants["e"] = M_E;
	constants["tau"] = M_PI * 2;
}

std::vector<Function> handlers::CalculationHandler::getFunctions() { // Maybe include versine and others: https://en.wikipedia.org/wiki/Versine | https://en.wikipedia.org/wiki/Cofunction
	std::vector<Function> fns;

	// Trigonometric functions
	fns.push_back(Function("sin", 1, [](std::vector<double> args) {
		return sin(args.at(0));
	}));
	fns.push_back(Function("cos", 1, [](std::vector<double> args) {
		return cos(args.at(0));
	}));
	fns.push_back(Function("tan", 1, [](std::vector<double> args) {
		return tan(args.at(0));
	}));
	fns.push_back(Function("asin", 1, [](std::vector<double> args) {
		return asin(args.at(0));
	}));
	fns.push_back(Function("acos", 1, [](std::vector<double> args) {
		return acos(args.at(0));
	}));
	fns.push_back(Function("atan", 1, [](std::vector<double> args) {
		return atan(args.at(0));
	}));
	fns.push_back(Function("atan2", 2, [](std::vector<double> args) {
		return atan2(args.at(0), args.at(1));
	}));

	// More Trigonometric functions
	fns.push_back(Function("csc", 2, [](std::vector<double> args) { // cosec
		return 1 / sin(args.at(0));
	}));
	fns.push_back(Function("sec", 2, [](std::vector<double> args) {
		return 1 / cos(args.at(0));
	}));
	fns.push_back(Function("cot", 2, [](std::vector<double> args) {
		return 1 / tan(args.at(0));
	}));

	// Hyperbolic functions
	fns.push_back(Function("sinh", 1, [](std::vector<double> args) {
		return sinh(args.at(0));
	}));
	fns.push_back(Function("cosh", 1, [](std::vector<double> args) {
		return cosh(args.at(0));
	}));
	fns.push_back(Function("tanh", 1, [](std::vector<double> args) {
		return tanh(args.at(0));
	}));
	fns.push_back(Function("asinh", 1, [](std::vector<double> args) {
		return asinh(args.at(0));
	}));
	fns.push_back(Function("acosh", 1, [](std::vector<double> args) {
		return acosh(args.at(0));
	}));
	fns.push_back(Function("atanh", 1, [](std::vector<double> args) {
		return atanh(args.at(0));
	}));

	// Logorithmic & Exponential functions
	fns.push_back(Function("ln", 1, [](std::vector<double> args) {
		return log(args.at(0));
	}));
	fns.push_back(Function("log10", 1, [](std::vector<double> args) { // log base 10
		return log10(args.at(0));
	}));
	fns.push_back(Function("log", 2, [](std::vector<double> args) { // log base of first argument: https://ekuatio.com/en/how-to-calculate-logarithms-of-any-base-formula-of-the-base-change/
		return log(args.at(0)) / log(args.at(1));
	}));
	fns.push_back(Function("exp", 1, [](std::vector<double> args) { // exponential function - E ^ first argument
		return exp(args.at(0));
	}));

	// Indices functions
	fns.push_back(Function("sqrt", 1, [](std::vector<double> args) {
		return sqrt(args.at(0));
	}));
	fns.push_back(Function("cbrt", 1, [](std::vector<double> args) { // cube root
		return cbrt(args.at(0));
	}));
	fns.push_back(Function("rt", 1, [](std::vector<double> args) {
		return pow(args.at(0), 1 / args.at(1));
	}));
	fns.push_back(Function("hypot", 1, [](std::vector<double> args) {
		return hypot(args.at(0), args.at(1));
	}));

	// Error & Gamma functions
	fns.push_back(Function("erf", 1, [](std::vector<double> args) {
		return erf(args.at(0));
	}));
	fns.push_back(Function("gamma", 1, [](std::vector<double> args) {
		return tgamma(args.at(0));
	}));

	// Rounding functions
	fns.push_back(Function("round", 1, [](std::vector<double> args) {
		return round(args.at(0));
	}));
	fns.push_back(Function("ceil", 1, [](std::vector<double> args) {
		return ceil(args.at(0));
	}));
	fns.push_back(Function("floor", 1, [](std::vector<double> args) {
		return floor(args.at(0));
	}));

	return fns;
}