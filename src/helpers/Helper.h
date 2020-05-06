// These two preprocessors, and the #endif at the end of the file, is a header guard - https://www.learncpp.com/cpp-tutorial/header-guards/
#ifndef HELPER_HEADER
#define HELPER_HEADER

#include <string>
#include <vector>
#include "../structs/Element.h"

using namespace std;
using namespace structs;

namespace helpers {
    class Helper {
        public:
            static string elemsToStr(vector<Element> elems);
            static double factorial(double num); // Actually uses the gamma function
            static int find_last_bracket(vector<Element> elems, bool isopen);
            static void tolower(string& str);
            static double deg(double* rad);
            static double rad(double* deg);
    };
}

#endif