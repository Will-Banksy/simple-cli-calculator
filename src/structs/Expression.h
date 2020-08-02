#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Element.h"
#include <vector>
#include <sstream>
#include <string>

using namespace structs;

/**
 * @todo write docs
 */
namespace structs {
	class Expression {
		public:
			/**
			 * Takes elements from a vector, or a subvector if the start and end values are changed
			 */
			explicit Expression(std::vector<Element> elems, int start = 0, int end = -1);
			~Expression() = default;
			int evaluate();
			bool isValid(std::stringstream* err = nullptr);
			std::string elemsToString();
			/**
			 * When we get the elements, they will be raw, as in things like negative numbers won't exist, we'll have a minus operator and the number, so we need to sort that out
			 */
			std::vector<Element>* parse();

			std::vector<Element> elems;

		private:
			bool parsed;
	};
}

#endif // EXPRESSION_H