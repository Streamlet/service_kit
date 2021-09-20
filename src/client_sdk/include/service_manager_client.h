#pragma once

#include <grpcpp/grpcpp.h>

#include "service_kit/registry_center.grpc.pb.h"

namespace service_kit {

class ServiceManagerClient {
 public:
  ServiceManagerClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(service_kit::ServiceManager::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  template <typename ServiceType>
  bool Query(std::string* address) {
    // Data we are sending to the server.
    service_kit::ServiceIdentify service_identify;
    service_identify.set_name(ServiceType::service_full_name());

    service_kit::ServiceProvider service_provider;
    grpc::ClientContext context;

    grpc::Status status =
        stub_->Query(&context, service_identify, &service_provider);

    if (!status.ok()) {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return false;
    }

    if (service_provider.address().empty()) {
      return false;
    }

    *address = service_provider.address();
    return true;
  }

 private:
  std::unique_ptr<service_kit::ServiceManager::Stub> stub_;
};

}  // namespace service_kit