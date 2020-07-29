#ifndef CALCULATIONHANDLER_H
#define CALCULATIONHANDLER_H

#include <vector>
#include <map>
#include <string>
#include "structs/Function.h"

using namespace structs;

namespace handlers {
	class CalculationHandler {
		public:
			static std::vector<char> operators;
			static std::vector<char> ops_bodmas;
			static std::map<std::string, double> constants;
			static std::vector<Function> functions;

		private:
			static void initialiseConstants();
			static std::vector<Function> getFunctions();

	};
}

#endif // CALCULATIONHANDLER_H