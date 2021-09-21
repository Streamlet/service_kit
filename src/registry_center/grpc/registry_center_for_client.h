#pragma once

#include <grpcpp/grpcpp.h>

#include "core/service_manager.h"
#include "protos/registry_center_for_client.grpc.pb.h"

namespace service_kit {
namespace registry_center {
namespace client {

class RegistryCenter final
    : public service_kit::client::RegistryCenter::Service {
 public:
  RegistryCenter(ServiceManager& mgr);
  ~RegistryCenter();

  grpc::Status Query(grpc::ServerContext* context,
                     const service_kit::client::ServiceIdentify* request,
                     service_kit::client::ServiceProvider* response) override;

 private:
  ServiceManager& mgr_;
};

}  // namespace client
}  // namespace registry_center
}  // namespace service_kit
