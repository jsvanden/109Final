/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * File containing exception classes to be used by the SRI engine        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
#ifndef _SRI_exception_lib_hpp
#define _SRI_exception_lib_hpp

#include<stdexcept>

class SRI_exception : public std::exception
{
   public:
      const char* what();
};

class InvalidInput_exception : public SRI_exception
{
   public:
      const char* what();
};

#endif