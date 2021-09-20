#include "service_manager.h"
#include "log.h"

namespace registry_center {
namespace grpc_impl {

ServiceManager::ServiceManager(core::ServiceManager& mgr) : mgr_(mgr) {}

ServiceManager::~ServiceManager() {}

grpc::Status ServiceManager::Query(grpc::ServerContext* context,
                                   const service_kit::ServiceIdentify* request,
                                   service_kit::ServiceProvider* response) {
  TRACE() << "Queri service " << request->name() << " from " << context->peer()
          << std::endl;

  std::string address;
  if (!mgr_.QueryService(request->name(), &address)) {
    WARNING() << "no privider found." << std::endl;
    return grpc::Status::OK;
  }

  response->set_address(address);
  INFO() << "Query service" << request->name() << " returned address "
         << address << std::endl;

  return grpc::Status::OK;
}

}  // namespace grpc_impl
}  // namespace registry_center
