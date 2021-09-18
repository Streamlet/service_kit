#pragma once

#include <google/protobuf/descriptor.h>
#include <grpcpp/grpcpp.h>

#include "registry_center.grpc.pb.h"

namespace registry_center {

class ServiceRegistryClient {
 public:
  ServiceRegistryClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(service_kit::ServiceRegistry::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the grpc::Server.
  template <typename ServiceType>
  bool Register(int32_t port) {
    std::cout << "Registering service: " << ServiceType::service_full_name()
              << std::endl;

    const google::protobuf::ServiceDescriptor* service_descriptor =
        google::protobuf::DescriptorPool::generated_pool()->FindServiceByName(
            ServiceType::service_full_name());
    google::protobuf::ServiceDescriptorProto* service_descriptor_proto =
        new google::protobuf::ServiceDescriptorProto;
    service_descriptor->CopyTo(service_descriptor_proto);

    // Data we are sending to the grpc::Server.
    service_kit::ServiceDefinitionWithSignature service_definition;
    service_definition.set_name(ServiceType::service_full_name());
    service_definition.set_port(port);
    service_definition.set_allocated_proto(service_descriptor_proto);

    service_kit::RegisterResult result;
    grpc::ClientContext context;

    grpc::Status status =
        stub_->Register(&context, service_definition, &result);

    if (!status.ok()) {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return false;
    }

    std::cout << "Service registered." << std::endl;
    return true;
  }

 private:
  std::unique_ptr<service_kit::ServiceRegistry::Stub> stub_;
};

}  // namespace registry_center
