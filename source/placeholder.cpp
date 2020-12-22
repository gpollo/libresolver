#include <libresolver/placeholder.hpp>

namespace std {

string to_string(libresolver::placeholder::reg reg) {
    switch (reg) {
    case libresolver::placeholder::reg::NONE:
        return "NONE";
    case libresolver::placeholder::reg::REG_1:
        return "REG_1";
    case libresolver::placeholder::reg::REG_2:
        return "REG_2";
    case libresolver::placeholder::reg::REG_3:
        return "REG_3";
    case libresolver::placeholder::reg::REG_4:
        return "REG_4";
    case libresolver::placeholder::reg::REG_5:
        return "REG_5";
    case libresolver::placeholder::reg::REG_6:
        return "REG_6";
    case libresolver::placeholder::reg::REG_7:
        return "REG_7";
    case libresolver::placeholder::reg::REG_8:
        return "REG_8";
    case libresolver::placeholder::reg::REG_9:
        return "REG_9";
    case libresolver::placeholder::reg::RIP:
        return "RIP";
    }

    return "(INVALID-REG)";
}

string to_string(libresolver::placeholder::value value) {
    switch (value) {
    case libresolver::placeholder::value::NONE:
        return "NONE";
    case libresolver::placeholder::value::VALUE_1:
        return "VALUE_1";
    case libresolver::placeholder::value::VALUE_2:
        return "VALUE_2";
    case libresolver::placeholder::value::VALUE_3:
        return "VALUE_3";
    case libresolver::placeholder::value::VALUE_4:
        return "VALUE_4";
    case libresolver::placeholder::value::VALUE_5:
        return "VALUE_5";
    case libresolver::placeholder::value::VALUE_6:
        return "VALUE_6";
    case libresolver::placeholder::value::VALUE_7:
        return "VALUE_7";
    case libresolver::placeholder::value::VALUE_8:
        return "VALUE_8";
    case libresolver::placeholder::value::VALUE_9:
        return "VALUE_9";
    }

    return "(INVALID-VALUE)";
}

} /* namespace std */