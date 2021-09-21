#include "registry_center_client.h"

#include "include/log.h"

namespace service_kit {
namespace client {

RegistryCenterClient::RegistryCenterClient(
    std::shared_ptr<grpc::Channel> channel)
    : stub_(service_kit::client::RegistryCenter::NewStub(channel)) {}

bool RegistryCenterClient::Query(const std::string& service_name,
                                 std::string* service_address) {
  INFO() << "Querying service: " << service_name;

  service_kit::client::ServiceIdentify service_identify;
  service_identify.set_name(service_name);

  service_kit::client::ServiceProvider service_provider;
  grpc::ClientContext context;

  grpc::Status status =
      stub_->Query(&context, service_identify, &service_provider);

  if (!status.ok()) {
    ERROR() << status.error_code() << ": " << status.error_message();
    return false;
  }

  if (service_provider.address().empty()) {
    return false;
  }

  *service_address = service_provider.address();

  INFO() << "Returned service address: " << *service_address;
  return true;
}

}  // namespace client
}  // namespace service_kit