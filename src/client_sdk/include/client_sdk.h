#pragma once

#include <string>

namespace service_kit {
namespace client {

bool QueryService(const std::string& registry_center_address,
                  const std::string& service_name,
                  std::string* service_address);

}  // namespace client
}  // namespace service_kit
