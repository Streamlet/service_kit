#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <boost/program_options.hpp>

#include "registry_center.grpc.pb.h"

class ServiceRegistryImpl final : public service_kit::ServiceRegistry::Service,
                                  public service_kit::ServiceManager::Service {
  grpc::Status Register(grpc::ServerContext* context,
                        const service_kit::ServiceDefinition* request,
                        service_kit::RegisterResult* response) {
    std::cout << "Service " << request->full_name() << " register from "
              << context->peer() << std::endl;

    auto it = service_name_peer_map_.find(request->full_name());
    if (it != service_name_peer_map_.end()) {
      response->set_ok(false);
      response->set_message("service '" + request->full_name() +
                            "' already registered.");
      std::cout << "Duplicated service name" << std::endl;

      return grpc::Status::OK;
    }

    std::string peer = context->peer();
    size_t first = peer.find_first_of(':');
    size_t last = peer.find_last_of(':');
    if (first == grpc::string::npos || last == grpc::string::npos) {
      response->set_ok(false);
      response->set_message("Failed to extract ip from peer '" + peer + "'");
      std::cout << "Duplicated service name" << std::endl;

      return grpc::Status::OK;
    }

    std::string ip = peer.substr(first + 1, last - first - 1);
    service_name_peer_map_.insert(
        {request->full_name(), ip + ":" + std::to_string(request->port())});
    response->set_ok(true);
    std::cout << "Service provider " << ip << ":" << request->port()
              << " registered" << std::endl;

    return grpc::Status::OK;
  }

  grpc::Status HeartBeat(grpc::ServerContext* context,
                         const service_kit::ServiceIdentify* request,
                         google::protobuf::Empty* response) {
    std::cout << "Service " << request->full_name() << " heart beated from "
              << context->peer() << std::endl;

    return grpc::Status::OK;
  }

  grpc::Status Unregister(grpc::ServerContext* context,
                          const service_kit::ServiceIdentify* request,
                          google::protobuf::Empty* response) {
    std::cout << "Service " << request->full_name() << " stopped from "
              << context->peer() << std::endl;

    size_t removed = service_name_peer_map_.erase(request->full_name());
    std::cout << removed << " peers removed" << std::endl;

    return grpc::Status::OK;
  }

  grpc::Status Query(grpc::ServerContext* context,
                     const service_kit::ServiceIdentify* request,
                     service_kit::ServiceProvider* response) {
    std::cout << "Queri service " << request->full_name() << " from "
              << context->peer() << std::endl;

    auto it = service_name_peer_map_.find(request->full_name());
    if (it != service_name_peer_map_.end()) {
      response->set_peer(it->second);
      std::cout << "return peer " << it->second << std::endl;
    } else {
      std::cout << "no privider found." << std::endl;
    }

    return grpc::Status::OK;
  }

 private:
  std::unordered_map<std::string, std::string> service_name_peer_map_;
};

void RunServer(uint16_t port) {
  std::string server_address = "0.0.0.0:" + std::to_string(port);
  ServiceRegistryImpl service_registry;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(
      static_cast<service_kit::ServiceRegistry::Service*>(&service_registry));
  builder.RegisterService(
      static_cast<service_kit::ServiceManager::Service*>(&service_registry));
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

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

int main(int argc, char** argv) {
  boost::program_options::variables_map vm;
  if (!ParseCommandLine(argc, argv, vm)) {
    return -1;
  }

  uint16_t port = vm[OPTION_PORT].as<uint16_t>();
  RunServer(port);

  return 0;
}
