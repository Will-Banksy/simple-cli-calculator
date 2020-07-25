#ifndef CALCULATIONHANDLER_H
#define CALCULATIONHANDLER_H

#include <vector>
#include <map>
#include <string>

/**
 * @todo write docs
 */
class CalculationHandler {
	public:
		static std::vector<char> operators;
		static std::vector<char> ops_bodmas;
		static std::map<std::string, double> constants;

	private:
		static void initialiseConstants();
};

#endif // CALCULATIONHANDLER_H
