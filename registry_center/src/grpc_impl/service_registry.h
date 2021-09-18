#pragma once

#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>
#include <grpcpp/grpcpp.h>

#include "core/service_manager.h"
#include "registry_center.grpc.pb.h"

namespace registry_center {
namespace grpc_impl {

class ServiceRegistry final : public service_kit::ServiceRegistry::Service {
 public:
  ServiceRegistry(core::ServiceManager& mgr);
  ~ServiceRegistry();

  grpc::Status Register(
      grpc::ServerContext* context,
      const service_kit::ServiceDefinitionWithSignature* request,
      service_kit::RegisterResult* response) override;
  grpc::Status HeartBeat(grpc::ServerContext* context,
                         const service_kit::ServiceDefinition* request,
                         google::protobuf::Empty* response) override;
  grpc::Status Unregister(grpc::ServerContext* context,
                          const service_kit::ServiceDefinition* request,
                          google::protobuf::Empty* response) override;

 private:
  core::ServiceManager& mgr_;
};

}  // namespace grpc_impl
}  // namespace registry_center
