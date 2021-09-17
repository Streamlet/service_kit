/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <google/protobuf/descriptor.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#include "examples/protos/registry_center.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#include "registry_center.grpc.pb.h"
#endif

using cpp_micro_service::RegisterResult;
using cpp_micro_service::ServiceDefinition;
using cpp_micro_service::ServiceRegisterer;
using google::protobuf::DescriptorPool;
using google::protobuf::ServiceDescriptor;
using google::protobuf::ServiceDescriptorProto;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class ServiceRegistererClient {
 public:
  ServiceRegistererClient(std::shared_ptr<Channel> channel)
      : stub_(ServiceRegisterer::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  template <typename ServiceType>
  bool Register(int32_t port) {
    std::cout << "Registering service: " << ServiceType::service_full_name()
              << std::endl;

    const ServiceDescriptor* service_descriptor =
        DescriptorPool::generated_pool()->FindServiceByName(
            ServiceType::service_full_name());
    ServiceDescriptorProto* service_descriptor_proto =
        new ServiceDescriptorProto;
    service_descriptor->CopyTo(service_descriptor_proto);

    // Data we are sending to the server.
    ServiceDefinition service_definition;
    service_definition.set_full_name(ServiceType::service_full_name());
    service_definition.set_port(port);
    service_definition.set_allocated_proto(service_descriptor_proto);

    RegisterResult result;
    ClientContext context;

    Status status = stub_->Register(&context, service_definition, &result);

    if (status.ok()) {
      std::cout << "Service registered." << std::endl;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
    }
    return status.ok();
  }

 private:
  std::unique_ptr<ServiceRegisterer::Stub> stub_;
};

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context,
                  const HelloRequest* request,
                  HelloReply* reply) override {
    std::cout << "Greeter request from " << context->peer() << ": "
              << request->name() << std::endl;

    reply->set_message("Hello " + request->name() + " from " + context->peer());

    return Status::OK;
  }
};

void RunServer() {
  std::string registry_center_address("localhost:50050");
  ServiceRegistererClient registry_center_client(grpc::CreateChannel(
      registry_center_address, grpc::InsecureChannelCredentials()));

  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());

  // Register service to remote registry center
  if (!registry_center_client.Register<Greeter>(50051)) {
    std::cout << "Server register error." << std::endl;
    server->Shutdown();
    return;
  }

  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must
  // be responsible for shutting down the server for this call to ever
  // return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
