#ifndef INCLUDE_LIBRESOLVER_LOG_HPP
#define INCLUDE_LIBRESOLVER_LOG_HPP

#include <iostream>
#include <regex>
#include <string>

#define LIBRESOLVER_LOG_BASE(output, msg)                   \
    do {                                                    \
        std::string func(__PRETTY_FUNCTION__);              \
        std::regex name_regex(".* (.*)\\(.*\\)");           \
        std::smatch matches;                                \
                                                            \
        if (std::regex_search(func, matches, name_regex)) { \
            func = matches[1];                              \
        } else {                                            \
            func = __PRETTY_FUNCTION__;                     \
        }                                                   \
                                                            \
        output << "[" << func << "] " << msg << std::endl;  \
    } while (0);

#define LIBRESOLVER_LOG(msg) LIBRESOLVER_LOG_BASE(std::cout, msg)
#define LIBRESOLVER_ERR(msg) LIBRESOLVER_LOG_BASE(std::cerr, msg)

#ifdef LIBRESOLVER_BUILD
#define LOG LIBRESOLVER_LOG
#define ERR LIBRESOLVER_ERR
#else
#define LOG
#define ERR
#endif

#endif /* INCLUDE_LIBRESOLVER_LOG_HPP */
