#include "service_registry.h"
#include "log.h"

namespace registry_center {
namespace grpc_impl {

namespace {

bool PeerToIP(const std::string& peer, std::string* ip) {
  size_t first = peer.find_first_of(':');
  size_t last = peer.find_last_of(':');
  if (first == std::string::npos || last == std::string::npos) {
    return false;
  }

  *ip = peer.substr(first + 1, last - first - 1);
  return true;
}
static bool PeerPortToAddress(const std::string& peer,
                              uint16_t port,
                              std::string* address) {
  std::string ip;
  if (!PeerToIP(peer, &ip)) {
    return false;
  }
  *address = ip + ":" + std::to_string(port);
  return true;
}

}  // namespace

ServiceRegistry::ServiceRegistry(core::ServiceManager& mgr) : mgr_(mgr) {}

ServiceRegistry::~ServiceRegistry() {}

::grpc::Status ServiceRegistry::Register(
    ::grpc::ServerContext* context,
    const service_kit::ServiceDefinitionWithSignature* request,
    service_kit::RegisterResult* response) {
  TRACE() << "Service " << request->name() << " register from "
          << context->peer() << std::endl;

  std::string address;
  if (!PeerPortToAddress(context->peer(), request->port(), &address)) {
    response->set_ok(false);
    response->set_message("internal error");
    ERROR() << "cannot extract pi from peer string '" + context->peer() + "'";
    return grpc::Status::OK;
  }

  std::string signature;
  if (!request->proto().SerializeToString(&signature)) {
    response->set_ok(false);
    response->set_message("internal error");
    ERROR() << "cannot serialize service " + request->proto().name();
    return grpc::Status::OK;
  }
  DEBUG() << "Signature: " << signature << std::endl;

  if (!mgr_.AddService(request->name(), address, signature)) {
    response->set_ok(false);
    response->set_message("service name '" + request->name() +
                          "' already registered with a different signature");
    return grpc::Status::OK;
  }

  response->set_ok(true);
  INFO() << "Service provider " << address << " successfully registered for "
         << request->name() << std::endl;

  return grpc::Status::OK;
}

grpc::Status ServiceRegistry::HeartBeat(
    grpc::ServerContext* context,
    const service_kit::ServiceDefinition* request,
    google::protobuf::Empty* response) {
  TRACE() << "Service " << request->name() << " heart beated from "
          << context->peer() << std::endl;

  return grpc::Status::OK;
}

grpc::Status ServiceRegistry::Unregister(
    grpc::ServerContext* context,
    const service_kit::ServiceDefinition* request,
    google::protobuf::Empty* response) {
  TRACE() << "Service " << request->name() << " stopped from "
          << context->peer() << std::endl;

  std::string address;
  if (!PeerPortToAddress(context->peer(), request->port(), &address)) {
    ERROR() << "cannot extract pi from peer string '" + context->peer() + "'";
    return grpc::Status::OK;
  }

  if (!mgr_.RemoveService(request->name(), address)) {
  }
  INFO() << "Service provider " << address << " successfully removed for "
         << request->name() << std::endl;

  return grpc::Status::OK;
}

}  // namespace grpc_impl
}  // namespace registry_center
