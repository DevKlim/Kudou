#ifndef _TVG_STR_H_
#define _TVG_STR_H_

#include <cstddef>

namespace tvg
{

float strToFloat(const char *nPtr, char **endPtr);  //convert to float
char* strDuplicate(const char *str, size_t n);      //copy the string
char* strAppend(char* lhs, const char* rhs, size_t n);  //append the rhs to the lhs
char* strDirname(const char* path);                 //return the full directory name

}
#endif //_TVG_STR_H_
