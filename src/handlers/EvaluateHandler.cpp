#include "EvaluateHandler.h"
#include "ParseHandler.h"

using namespace handlers;

double EvaluateHandler::evaluate(std::vector<Element>& elems) {
	ParseHandler::check(elems);
	return 0;
}