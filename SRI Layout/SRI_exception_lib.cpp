#include "SRI_exception_lib.hpp"
#include <string>
// #include <stdexcept>

using namespace std;

const char* SRI_exception::what()
{
   return "SRI Error";
}

const char* InvalidInput_exception::what()
{
   return "Invalid Input";
}