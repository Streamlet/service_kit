#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>

#include "protos/registry_center_for_server.grpc.pb.h"

namespace service_kit {
namespace server {

class RegistryCenterClient {
 public:
  RegistryCenterClient(std::shared_ptr<grpc::Channel> channel);
  bool Register(const std::string& service_name, int32_t service_port);

 private:
  std::unique_ptr<service_kit::server::RegistryCenter::Stub> stub_;
};

}  // namespace server
}  // namespace service_kit
