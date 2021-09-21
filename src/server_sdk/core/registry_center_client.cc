#include "registry_center_client.h"

#include <google/protobuf/descriptor.h>

#include "include/log.h"

namespace service_kit {
namespace server {

RegistryCenterClient::RegistryCenterClient(
    std::shared_ptr<grpc::Channel> channel)
    : stub_(service_kit::server::RegistryCenter::NewStub(channel)) {}

bool RegistryCenterClient::Register(const std::string& service_name,
                                    int32_t service_port) {
  INFO() << "Registering service: " << service_name;

  const google::protobuf::ServiceDescriptor* service_descriptor =
      google::protobuf::DescriptorPool::generated_pool()->FindServiceByName(
          service_name);
  google::protobuf::ServiceDescriptorProto* service_descriptor_proto =
      new google::protobuf::ServiceDescriptorProto;
  service_descriptor->CopyTo(service_descriptor_proto);

  // Data we are sending to the grpc::Server.
  service_kit::server::ServiceDefinitionWithSignature service_definition;
  service_definition.set_name(service_name);
  service_definition.set_port(service_port);
  service_definition.set_allocated_proto(service_descriptor_proto);

  service_kit::server::RegisterResult result;
  grpc::ClientContext context;

  grpc::Status status = stub_->Register(&context, service_definition, &result);

  if (!status.ok()) {
    ERROR() << status.error_code() << ": " << status.error_message();
    return false;
  }

  INFO() << "Service registered";
  return true;
}

}  // namespace server
}  // namespace service_kit
