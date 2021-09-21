#include "core/registry_center_client.h"
#include "include/server_sdk.h"

namespace service_kit {
namespace server {

bool RegisterService(const std::string& registry_center_address,
                     const std::string& service_name,
                     uint16_t service_port) {
  RegistryCenterClient registry_center_client(grpc::CreateChannel(
      registry_center_address, grpc::InsecureChannelCredentials()));
  return registry_center_client.Register(service_name, service_port);
}

}  // namespace server
}  // namespace service_kit