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
			/**
			 * Evaluates a section of an expression, or a 'Snippet'
			 * A snippet cannot contain functions, brackets or commas (argument separators)
			 */
			static void evalSnippet(std::vector<Element>& elems, int start, int end);
			static int findCloseBracket(std::vector<Element>& elems, int afterIndex, bool* hasArguments = nullptr);
	};
}

#endif // EVALUATEHANDLER_H
