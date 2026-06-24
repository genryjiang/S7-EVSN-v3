#include "node_supervisor.hpp"

#include <algorithm>
#include <cctype>

std::string make_supervision_entity_name(const std::string& node_name) {
    std::string entity_name{"evsn_node_"};
    entity_name.reserve(entity_name.size() + node_name.size());

    for (unsigned char character : node_name) {
        if (std::isalnum(character) || character == '_' || character == '-' || character == '.') {
            entity_name.push_back(static_cast<char>(character));
        } else {
            entity_name.push_back('_');
        }
    }

    if (entity_name == "evsn_node_") {
        entity_name += "node";
    }

    constexpr std::size_t max_ham_name_length = 200;
    if (entity_name.size() > max_ham_name_length) {
        entity_name.resize(max_ham_name_length);
    }

    return entity_name;
}
