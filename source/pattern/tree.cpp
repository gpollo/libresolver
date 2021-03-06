#include <stack>

#include <libresolver/log.hpp>
#include <libresolver/pattern/tree.hpp>

namespace libresolver::pattern {

bool tree::add(const tree::pattern& pattern, const std::shared_ptr<match> match) {
    node* current_node = &root_;

    for (const auto& [instruction, actions] : pattern) {
        if (!current_node->insert(instruction, actions)) {
            ERR("failed to insert new node");
            return false;
        }

        auto existing_node = current_node->get(instruction);
        if (!existing_node.has_value()) {
            ERR("failed to get existing node");
            return false;
        }

        current_node = &existing_node.value().get();
        if (current_node == nullptr) {
            ERR("current node is null");
            return false;
        }
    }

    if (!current_node->insert(match)) {
        ERR("failed to insert match to node");
        return false;
    }

    return true;
}

const node& tree::get_root() const {
    return root_;
}

} /* namespace libresolver::pattern */

namespace std {

string to_string(const libresolver::pattern::tree& tree) {
    string str = "";

    using node_ref  = std::reference_wrapper<libresolver::pattern::node>;
    using node_data = std::tuple<libresolver::instruction::base, node_ref, unsigned int>;

    std::stack<node_data> nodes;
    for (auto child_instruction : tree.get_root().get_child_instructions()) {
        auto child_node_opt = tree.get_root().get(child_instruction);
        if (!child_node_opt.has_value()) {
            ERR("failed to obtain node");
            return "";
        }

        nodes.push(make_tuple(child_instruction, child_node_opt.value(), 0));
    }

    while (!nodes.empty()) {
        auto [instruction, node, indent] = nodes.top();
        nodes.pop();

        for (unsigned int i = 0; i < indent; i++) {
            str.append("  ");
        }

        str.append(to_string(instruction));
        str.append(to_string(node.get().get_actions()));
        str.append("\n");

        for (auto child_instruction : node.get().get_child_instructions()) {
            auto child_node_opt = node.get().get(child_instruction);
            if (!child_node_opt.has_value()) {
                ERR("failed to obtain node");
                return "";
            }

            nodes.push(make_tuple(child_instruction, child_node_opt.value(), indent + 1));
        }

        for (auto match : node.get().get_matches()) {
            for (unsigned int i = 0; i < indent + 1; i++) {
                str.append("  ");
            }

            str.append("[" + match->get_name() + "]\n");
        }
    }

    return str;
}

} /* namespace std */