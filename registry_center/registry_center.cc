#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/empty.pb.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "../protos/registry_center.grpc.pb.h"
#else
#include "registry_center.grpc.pb.h"
#endif

using cpp_micro_service::RegisterResult;
using cpp_micro_service::ServiceDefinition;
using cpp_micro_service::ServiceIdentify;
using cpp_micro_service::ServiceProvider;
using cpp_micro_service::ServiceQuerier;
using cpp_micro_service::ServiceRegisterer;
using google::protobuf::Descriptor;
using google::protobuf::Empty;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

std::unordered_map<std::string, std::string> service_name_peer_map_;

class ServiceRegistererImpl final : public ServiceRegisterer::Service {
  Status Register(ServerContext* context,
                  const ServiceDefinition* request,
                  RegisterResult* response) {
    std::cout << "Service " << request->full_name() << " register from "
              << context->peer() << std::endl;

    auto it = service_name_peer_map_.find(request->full_name());
    if (it != service_name_peer_map_.end()) {
      response->set_ok(false);
      response->set_message("service '" + request->full_name() +
                            "' already registered.");
      std::cout << "Duplicated service name" << std::endl;

      return Status::OK;
    }

    std::string peer = context->peer();
    size_t first = peer.find_first_of(':');
    size_t last = peer.find_last_of(':');
    if (first == grpc::string::npos || last == grpc::string::npos) {
      response->set_ok(false);
      response->set_message("Failed to extract ip from peer '" + peer + "'");
      std::cout << "Duplicated service name" << std::endl;

      return Status::OK;
    }

    std::string ip = peer.substr(first + 1, last - first - 1);
    service_name_peer_map_.insert(
        {request->full_name(), ip + ":" + std::to_string(request->port())});
    response->set_ok(true);
    std::cout << "Service provider " << ip << ":" << request->port()
              << " registered" << std::endl;

    return Status::OK;
  }

  Status HeartBeat(ServerContext* context,
                   const ServiceIdentify* request,
                   Empty* response) {
    std::cout << "Service " << request->full_name() << " heart beated from "
              << context->peer() << std::endl;

    return Status::OK;
  }

  Status Unregister(ServerContext* context,
                    const ServiceIdentify* request,
                    Empty* response) {
    std::cout << "Service " << request->full_name() << " stopped from "
              << context->peer() << std::endl;

    size_t removed = service_name_peer_map_.erase(request->full_name());
    std::cout << removed << " peers removed" << std::endl;

    return Status::OK;
  }
};

class ServiceQuerierImpl final : public ServiceQuerier::Service {
  Status Query(ServerContext* context,
               const ServiceIdentify* request,
               ServiceProvider* response) {
    std::cout << "Queri service " << request->full_name() << " from "
              << context->peer() << std::endl;

    auto it = service_name_peer_map_.find(request->full_name());
    if (it != service_name_peer_map_.end()) {
      response->set_peer(it->second);
      std::cout << "return peer " << it->second << std::endl;
    } else {
      std::cout << "no privider found." << std::endl;
    }

    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50050");
  ServiceRegistererImpl service_registerer;
  ServiceQuerierImpl service_querier;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service_registerer);
  builder.RegisterService(&service_querier);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
