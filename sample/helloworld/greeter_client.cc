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
#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#include "examples/protos/registry_center.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#include "registry_center.grpc.pb.h"
#endif

using cpp_micro_service::ServiceIdentify;
using cpp_micro_service::ServiceProvider;
using cpp_micro_service::ServiceQuerier;
using google::protobuf::DescriptorPool;
using google::protobuf::ServiceDescriptor;
using google::protobuf::ServiceDescriptorProto;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class ServiceQuerierClient {
 public:
  ServiceQuerierClient(std::shared_ptr<Channel> channel)
      : stub_(ServiceQuerier::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  template <typename ServiceType>
  std::string Query() {
    // Data we are sending to the server.
    ServiceIdentify service_identify;
    service_identify.set_full_name(ServiceType::service_full_name());

    ServiceProvider service_provider;
    ClientContext context;

    Status status = stub_->Query(&context, service_identify, &service_provider);

    if (status.ok()) {
      return service_provider.peer();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "";
    }
  }

 private:
  std::unique_ptr<ServiceQuerier::Stub> stub_;
};

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& user) {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayHello(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
  ServiceQuerierClient service_querier_client(grpc::CreateChannel(
      "localhost:50050", grpc::InsecureChannelCredentials()));
  std::string peer = service_querier_client.Query<Greeter>();
  if (peer.empty()) {
    std::cout << "No service provider." << std::endl;
    return -1;
  }
  std::cout << "Service provider: " << peer << std::endl;

  GreeterClient greeter(
      grpc::CreateChannel(peer, grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;

  return 0;
}
