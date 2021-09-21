
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <boost/program_options.hpp>
#include <fstream>

#include "core/service_manager.h"
#include "grpc/registry_center_for_client.h"
#include "grpc/registry_center_for_server.h"
#include "include/log.h"

namespace service_kit {
namespace registry_center {
const char* OPTION_HELP = "help";
const char* OPTION_PORT = "port";

bool ParseCommandLine(int argc,
                      char** argv,
                      boost::program_options::variables_map& vm) {
  try {
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()(OPTION_HELP, "produce help message");
    desc.add_options()(
        OPTION_PORT,
        boost::program_options::value<uint16_t>()->default_value(50050),
        "service port");
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count(OPTION_HELP)) {
      INFO() << desc;
      return false;
    }
  } catch (boost::program_options::unknown_option e) {
    FATAL() << e.what();
    return false;
  } catch (boost::program_options::invalid_option_value e) {
    FATAL() << e.what();
    return false;
  }

  return true;
}

bool Initialize() {
  if (!InitializeLog())
    return false;
  return true;
}

void RunServer(uint16_t port) {
  std::string server_address = "0.0.0.0:" + std::to_string(port);
  ServiceManager service_manager;
  server::RegistryCenter registry_center_for_server(service_manager);
  client::RegistryCenter registry_center_for_client(service_manager);

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&registry_center_for_server);
  builder.RegisterService(&registry_center_for_client);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  INFO() << "Server listening on " << server_address;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

}  // namespace registry_center
}  // namespace service_kit

int main(int argc, char** argv) {
  boost::program_options::variables_map vm;
  if (!service_kit::registry_center::ParseCommandLine(argc, argv, vm)) {
    return -1;
  }

  if (!service_kit::registry_center::Initialize()) {
    return -2;
  }

  uint16_t port = vm[service_kit::registry_center::OPTION_PORT].as<uint16_t>();
  service_kit::registry_center::RunServer(port);

  return 0;
}
