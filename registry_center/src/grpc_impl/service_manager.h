#pragma once

#include <grpcpp/grpcpp.h>

#include "core/service_manager.h"
#include "registry_center.grpc.pb.h"

namespace registry_center {
namespace grpc_impl {

class ServiceManager final : public service_kit::ServiceManager::Service {
 public:
  ServiceManager(core::ServiceManager& mgr);
  ~ServiceManager();

  grpc::Status Query(grpc::ServerContext* context,
                     const service_kit::ServiceIdentify* request,
                     service_kit::ServiceProvider* response) override;

 private:
  core::ServiceManager& mgr_;
};

}  // namespace grpc_impl
}  // namespace registry_center
