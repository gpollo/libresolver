#include <cstdlib>

#include <libresolver/cases/case_1.hpp>
#include <libresolver/cases/case_10.hpp>
#include <libresolver/cases/case_11.hpp>
#include <libresolver/cases/case_12.hpp>
#include <libresolver/cases/case_13.hpp>
#include <libresolver/cases/case_14.hpp>
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

    auto case_1 = std::make_shared<libresolver::cases::case_1>();
    if (!(tree.add(case_1->get_pattern(), case_1))) {
        return -1;
    }

    auto case_2 = std::make_shared<libresolver::cases::case_2>();
    if (!(tree.add(case_2->get_pattern(), case_2))) {
        return -1;
    }

    auto case_3 = std::make_shared<libresolver::cases::case_3>();
    if (!(tree.add(case_3->get_pattern(), case_3))) {
        return -1;
    }

    auto case_4 = std::make_shared<libresolver::cases::case_4>();
    if (!(tree.add(case_4->get_pattern(), case_4))) {
        return -1;
    }

    auto case_5 = std::make_shared<libresolver::cases::case_5>();
    if (!(tree.add(case_5->get_pattern(), case_5))) {
        return -1;
    }

    auto case_6 = std::make_shared<libresolver::cases::case_6>();
    if (!(tree.add(case_6->get_pattern(), case_6))) {
        return -1;
    }

    auto case_7 = std::make_shared<libresolver::cases::case_7>();
    if (!(tree.add(case_7->get_pattern(), case_7))) {
        return -1;
    }

    auto case_8 = std::make_shared<libresolver::cases::case_8>();
    if (!(tree.add(case_8->get_pattern(), case_8))) {
        return -1;
    }

    auto case_9 = std::make_shared<libresolver::cases::case_9>();
    if (!(tree.add(case_9->get_pattern(), case_9))) {
        return -1;
    }

    auto case_10 = std::make_shared<libresolver::cases::case_10>();
    if (!(tree.add(case_10->get_pattern(), case_10))) {
        return -1;
    }

    auto case_11 = std::make_shared<libresolver::cases::case_11>();
    if (!(tree.add(case_11->get_pattern(), case_11))) {
        return -1;
    }

    auto case_12 = std::make_shared<libresolver::cases::case_12>();
    if (!(tree.add(case_12->get_pattern(), case_12))) {
        return -1;
    }

    auto case_13 = std::make_shared<libresolver::cases::case_13>();
    if (!(tree.add(case_13->get_pattern(), case_13))) {
        return -1;
    }

    auto case_14 = std::make_shared<libresolver::cases::case_14>();
    if (!(tree.add(case_14->get_pattern(), case_14))) {
        return -1;
    }

    libresolver::pattern::matcher matcher(tree);
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