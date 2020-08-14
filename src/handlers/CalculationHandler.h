#ifndef CALCULATIONHANDLER_H
#define CALCULATIONHANDLER_H

#include <vector>
#include <map>
#include <string>
#include "structs/Function.h"

using namespace structs;

namespace handlers {
	typedef std::vector<char> OpList;

	class CalculationHandler {
		public:
			static OpList operators;
			static std::vector<OpList> ops_bodmas; // The closer to the start of the vector a list of operators is, the higher precedence for each of those operators. The index is the precedence
			static std::map<std::string, double> constants;
			static std::vector<Function> functions;

		private:
			static std::vector<Function> getFunctions();
			static std::map<std::string, double> getConstants();
			static std::vector<OpList> getBODMASOperators();
	};
}

#endif // CALCULATIONHANDLER_H