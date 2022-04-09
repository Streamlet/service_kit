#include "registry_center_for_client.h"
#include "include/log.h"

namespace service_kit {
namespace registry_center {
namespace client {

RegistryCenter::RegistryCenter(ServiceManager& mgr) : mgr_(mgr) {}

RegistryCenter::~RegistryCenter() {}

grpc::Status RegistryCenter::Query(
    grpc::ServerContext* context,
    const service_kit::client::ServiceIdentify* request,
    service_kit::client::ServiceProvider* response) {
  TRACE() << "Query service " << request->name() << " from " << context->peer();

  std::string address;
  if (!mgr_.QueryService(request->name(), &address)) {
    WARNING() << "no privider found.";
    return grpc::Status::OK;
  }

  response->set_address(address);
  INFO() << "Query service" << request->name() << " returned address "
         << address;

  return grpc::Status::OK;
}

}  // namespace client
}  // namespace registry_center
}  // namespace service_kit
