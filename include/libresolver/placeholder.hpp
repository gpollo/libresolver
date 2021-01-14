#ifndef LIBRESOLVER_INCLUDE_PLACEHOLDER_HPP
#define LIBRESOLVER_INCLUDE_PLACEHOLDER_HPP

#include <string>

namespace libresolver::placeholder {

enum class reg { NONE, REG_1, REG_2, REG_3, REG_4, REG_5, REG_6, REG_7, REG_8, REG_9, RIP };

enum class value { NONE, VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5, VALUE_6, VALUE_7, VALUE_8, VALUE_9, VALUE_10 };

} /* namespace libresolver::placeholder */

namespace std {

string to_string(libresolver::placeholder::reg reg);

string to_string(libresolver::placeholder::value value);

} /* namespace std */

#endif /* LIBRESOLVER_INCLUDE_PLACEHOLDER_HPP */