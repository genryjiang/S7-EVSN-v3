#include "node_supervisor.hpp"

#include <utility>

namespace {

class NoopNodeSupervisor final : public NodeSupervisor {
public:
    explicit NoopNodeSupervisor(std::string node_name) {
        config_.entity_name = make_supervision_entity_name(node_name);
    }

    const SupervisionConfig& config() const override {
        return config_;
    }

    void heartbeat() override {
    }

    void shutdown() override {
    }

private:
    SupervisionConfig config_{};
};

}

std::unique_ptr<NodeSupervisor> create_node_supervisor(const std::string& node_name) {
    return std::make_unique<NoopNodeSupervisor>(node_name);
}
