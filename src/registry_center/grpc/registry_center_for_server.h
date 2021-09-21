#pragma once

#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>
#include <grpcpp/grpcpp.h>

#include "core/service_manager.h"
#include "protos/registry_center_for_server.grpc.pb.h"

namespace service_kit {
namespace registry_center {
namespace server {

class RegistryCenter final
    : public service_kit::server::RegistryCenter::Service {
 public:
  RegistryCenter(ServiceManager& mgr);
  ~RegistryCenter();

  grpc::Status Register(
      grpc::ServerContext* context,
      const service_kit::server::ServiceDefinitionWithSignature* request,
      service_kit::server::RegisterResult* response) override;
  grpc::Status HeartBeat(grpc::ServerContext* context,
                         const service_kit::server::ServiceDefinition* request,
                         google::protobuf::Empty* response) override;
  grpc::Status Unregister(grpc::ServerContext* context,
                          const service_kit::server::ServiceDefinition* request,
                          google::protobuf::Empty* response) override;

 private:
  ServiceManager& mgr_;
};

}  // namespace server
}  // namespace registry_center
}  // namespace service_kit
