#include <grpcpp/grpcpp.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "protos/helloworld.grpc.pb.h"
#include "service_kit/client_sdk.h"

const char* OPTION_HELP = "help";
const char* OPTION_REGISTRY_CENTER = "registry_center";

bool ParseCommandLine(int argc,
                      char** argv,
                      boost::program_options::variables_map& vm) {
  try {
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()(OPTION_HELP, "produce help message");
    desc.add_options()(
        OPTION_REGISTRY_CENTER,
        boost::program_options::value<std::string>()->default_value(
            "localhost:50050"),
        "registry center address");
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count(OPTION_HELP)) {
      std::cout << desc << std::endl;
      return false;
    }
  } catch (boost::program_options::unknown_option e) {
    std::cout << e.what() << std::endl;
    return false;
  } catch (boost::program_options::invalid_option_value e) {
    std::cout << e.what() << std::endl;
    return false;
  }

  return true;
}

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(helloworld::Greeter::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& user) {
    // Data we are sending to the server.
    helloworld::HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    helloworld::HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    grpc::ClientContext context;

    // The actual RPC.
    grpc::Status status = stub_->SayHello(&context, request, &reply);

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
  std::unique_ptr<helloworld::Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
  boost::program_options::variables_map vm;
  if (!ParseCommandLine(argc, argv, vm)) {
    return -1;
  }

  std::string registry_center_address =
      vm[OPTION_REGISTRY_CENTER].as<std::string>();
  std::string service_address;
  if (!service_kit::client::QueryService(
          registry_center_address, helloworld::Greeter::service_full_name(),
          &service_address)) {
    std::cout << "No service provider found from " << registry_center_address
              << std::endl;
    return -1;
  }
  std::cout << "Service provider: " << service_address << std::endl;

  GreeterClient greeter(
      grpc::CreateChannel(service_address, grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;

  return 0;
}
