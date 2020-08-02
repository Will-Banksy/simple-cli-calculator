#ifndef EVALUATEHANDLER_H
#define EVALUATEHANDLER_H

#include <vector>
#include "structs/Element.h"

using namespace structs;

namespace handlers {
	class EvaluateHandler {
		public:
			static double evaluate(std::vector<Element>& elems, std::stringstream* errStream = nullptr);

		private:
			static void applyOperator(std::vector<Element>& elems, int opIndex, short& numRemoved);
	};
}

#endif // EVALUATEHANDLER_H
