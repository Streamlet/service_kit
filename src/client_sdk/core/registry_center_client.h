#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>

#include "protos/registry_center_for_client.grpc.pb.h"

namespace service_kit {
namespace client {

class RegistryCenterClient {
 public:
  RegistryCenterClient(std::shared_ptr<grpc::Channel> channel);
  bool Query(const std::string& service_name, std::string* service_address);

 private:
  std::unique_ptr<service_kit::client::RegistryCenter::Stub> stub_;
};

}  // namespace client
}  // namespace service_kit