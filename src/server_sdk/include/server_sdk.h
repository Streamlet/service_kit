#pragma once

#include <string>

namespace service_kit {
namespace server {

bool RegisterService(const std::string& registry_center_address,
                     const std::string& service_name,
                     uint16_t service_port);

}  // namespace server
}  // namespace service_kit