#include "core/registry_center_client.h"
#include "include/client_sdk.h"

namespace service_kit {
namespace client {

bool QueryService(const std::string& registry_center_address,
                  const std::string& service_name,
                  std::string* service_address) {
  RegistryCenterClient registry_center_client(grpc::CreateChannel(
      registry_center_address, grpc::InsecureChannelCredentials()));
  return registry_center_client.Query(service_name, service_address);
}

}  // namespace client
}  // namespace service_kit
