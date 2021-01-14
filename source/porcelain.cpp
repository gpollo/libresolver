#include <cstdlib>

#include <libresolver/cases/case_1.hpp>
#include <libresolver/cases/case_10.hpp>
#include <libresolver/cases/case_11.hpp>
#include <libresolver/cases/case_12.hpp>
#include <libresolver/cases/case_13.hpp>
#include <libresolver/cases/case_14.hpp>
#include <libresolver/cases/case_15.hpp>
#include <libresolver/cases/case_16.hpp>
#include <libresolver/cases/case_2.hpp>
#include <libresolver/cases/case_3.hpp>
#include <libresolver/cases/case_4.hpp>
#include <libresolver/cases/case_5.hpp>
#include <libresolver/cases/case_6.hpp>
#include <libresolver/cases/case_7.hpp>
#include <libresolver/cases/case_8.hpp>
#include <libresolver/cases/case_9.hpp>
#include <libresolver/memory.hpp>
#include <libresolver/pattern/matcher.hpp>
#include <libresolver/porcelain.hpp>
#include <tests/pattern/tree.hpp>

template <typename Case, typename Next = void, typename... Args>
static inline bool add_cases(libresolver::pattern::tree& tree, std::true_type /* is_last */) {
    return true;
}

template <typename Case, typename Next = void, typename... Args>
static inline bool add_cases(libresolver::pattern::tree& tree, std::false_type /* is_last */) {
    auto new_case = std::make_shared<Case>();
    if (!(tree.add(new_case->get_pattern(), new_case))) {
        return false;
    }

    return add_cases<Next, Args...>(tree, std::is_void<Next>());
}

template <typename Next = void, typename... Args>
static inline bool add_cases(libresolver::pattern::tree& tree) {
    return add_cases<Next, Args...>(tree, std::is_void<Next>());
}

int libresolver_x86_resolve(csh handle, cs_insn* insns, size_t insn_count, size_t index, uint64_t** results,
                            void* memory) {
    std::vector<std::reference_wrapper<cs_insn>> insns_vec;
    for (int i = index; i >= 0; i--) {
        insns_vec.push_back(insns[i]);
    }

    libresolver::process_memory process_memory;
    libresolver::memory* memory_interface;
    if (memory != nullptr) {
        memory_interface = (libresolver::memory*)memory;
    } else {
        memory_interface = &process_memory;
    }

    libresolver::pattern::tree tree;

    bool success = add_cases<libresolver::cases::case_1, libresolver::cases::case_2, libresolver::cases::case_3,
                             libresolver::cases::case_4, libresolver::cases::case_5, libresolver::cases::case_6,
                             libresolver::cases::case_7, libresolver::cases::case_8, libresolver::cases::case_9,
                             libresolver::cases::case_10, libresolver::cases::case_11, libresolver::cases::case_12,
                             libresolver::cases::case_13, libresolver::cases::case_14, libresolver::cases::case_15,
                             libresolver::cases::case_16>(tree);
    if (!success) {
        return -1;
    }


    libresolver::pattern::matcher matcher(tree, true);
    auto matches = matcher.match_instructions(insns_vec);
    if (matches.size() == 0) {
        return -1;
    }

    auto values = matches[0]->evaluate(matcher.get_context(), *memory_interface);
    if (values.size() == 0) {
        return -1;
    }

    *results = (uint64_t*)malloc(values.size() * sizeof(uint64_t));

    std::vector<uint64_t> values_vec(values.begin(), values.end());
    for (size_t i = 0; i < values_vec.size(); i++) {
        (*results)[i] = values_vec[i];

        if (getenv("LIBRESOLVER_PRINT_TARGETS") != nullptr) {
            std::cout << "target jump location: " << values_vec[i] << std::endl;
        }
    }

    return values.size();
}