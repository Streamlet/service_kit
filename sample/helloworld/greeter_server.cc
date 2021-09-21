#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "protos/helloworld.grpc.pb.h"
#include "service_kit/server_sdk.h"

const char* OPTION_HELP = "help";
const char* OPTION_REGISTRY_CENTER = "registry_center";
const char* OPTION_PORT = "port";

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
    desc.add_options()(
        OPTION_PORT,
        boost::program_options::value<uint16_t>()->default_value(50051),
        "service port");
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

// Logic and data behind the grpc::Server's behavior.
class GreeterServiceImpl final : public helloworld::Greeter::Service {
  grpc::Status SayHello(grpc::ServerContext* context,
                        const helloworld::HelloRequest* request,
                        helloworld::HelloReply* reply) override {
    std::cout << "helloworld::Greeter request from " << context->peer() << ": "
              << request->name() << std::endl;

    reply->set_message("Hello " + request->name() + " from " + context->peer());

    return grpc::Status::OK;
  }
};

void RunServer(const std::string& registry_center_address, uint16_t port) {
  std::string server_address = "0.0.0.0:" + std::to_string(port);
  GreeterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the Server.
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  // Register service to remote registry center
  if (!service_kit::server::RegisterService(
          registry_center_address, helloworld::Greeter::service_full_name(),
          port)) {
    std::cout << "failed to register service to " << registry_center_address
              << std::endl;
    server->Shutdown();
    return;
  }

  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the Server to shutdown. Note that some other thread must
  // be responsible for shutting down the Server for this call to ever
  // return.
  server->Wait();
}

int main(int argc, char** argv) {
  boost::program_options::variables_map vm;
  if (!ParseCommandLine(argc, argv, vm)) {
    return -1;
  }

  std::string registry_center = vm[OPTION_REGISTRY_CENTER].as<std::string>();
  uint16_t port = vm[OPTION_PORT].as<uint16_t>();

  RunServer(registry_center, port);

  return 0;
}
