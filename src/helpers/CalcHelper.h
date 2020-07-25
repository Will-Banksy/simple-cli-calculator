// These two preprocessors, and the #endif at the end of the file, is a header guard - https://www.learncpp.com/cpp-tutorial/header-guards/
#ifndef CALCHELPER_HEADER
#define CALCHELPER_HEADER

#include <vector>
#include <map>
#include "../structs/Element.h" // Relative path to Element header file

using namespace structs;

namespace helpers {
    enum AngleMode {
        RADIANS,
        DEGREES
    };

    class CalcHelper {
        public:
            static AngleMode mode;

            static double doCalculations(vector<Element> &elements);
            static vector<Element> getElements(char chars[], int arrlen);
            static double calculate(char chars[], int arrlen);

        private:
            static vector<char> operators;
            static vector<char> ops_bodmas;
            static map<string, double> constants;

            static bool isPartOfNum(char ch);
            static bool isLetter(char ch);
            static bool isOperator(char ch);
            static bool isOpeningBracket(char ch);
            static bool isClosingBracket(char ch);
            static map<string, double> createMap();
    };
}

#endif