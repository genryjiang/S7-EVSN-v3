#pragma once

#include <chrono>
#include <memory>
#include <string>

struct SupervisionConfig {
    std::string entity_name;
    std::chrono::milliseconds heartbeat_period{1000};
    int missed_heartbeat_low{3};
    int missed_heartbeat_high{5};
    std::string restart_command;
    std::string heartbeat_high_command;
};

class NodeSupervisor {
public:
    virtual ~NodeSupervisor() = default;

    virtual const SupervisionConfig& config() const = 0;
    virtual void heartbeat() = 0;
    virtual void shutdown() = 0;
};

std::string make_supervision_entity_name(const std::string& node_name);
std::unique_ptr<NodeSupervisor> create_node_supervisor(const std::string& node_name);
