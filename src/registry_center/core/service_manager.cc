#include "service_manager.h"

#include <cstdlib>
#include <ctime>
#include <sstream>

#include "include/log.h"

namespace service_kit {
namespace registry_center {

ServiceManager::ServiceManager() {
  std::srand(std::time(nullptr));
}
ServiceManager::~ServiceManager() {}

bool ServiceManager::AddService(const std::string& name,
                                const std::string& address,
                                const std::string& signature) {
  TRACE() << "Adding service: " << name;

  auto it_signature = service_name_signature_map_.find(name);
  if (it_signature == service_name_signature_map_.end()) {
    service_name_signature_map_.insert({name, signature});
  } else {
    if (it_signature->second != signature) {
      ERROR() << "Signature not match";
      return false;
    }
  }

  service_name_addresses_map_[name].insert(address);
  DEBUG() << "current registey: " << DumpResgistry();

  return true;
}

bool ServiceManager::QueryService(const std::string& name,
                                  std::string* address) {
  TRACE() << "Querying service: " << name;
  DEBUG() << "current registey: " << DumpResgistry();

  auto it = service_name_addresses_map_.find(name);
  if (it == service_name_addresses_map_.end() || it->second.empty()) {
    WARNING() << "Not found";
    return false;
  }

  const auto& addresses = it->second;
  // TODO: implement more load balance policies
  // currently random find one with 1/N possibility
  int c = 0;
  for (const auto& addr : addresses) {
    if (std::rand() * (c + 1ULL) < RAND_MAX + 1ULL) {
      *address = addr;
    }
    ++c;
  }

  TRACE() << "Found service: " << *address;
  return true;
}

bool ServiceManager::RemoveService(const std::string& name,
                                   const std::string& address) {
  TRACE() << "Removing service: " << name;

  auto it_addresses = service_name_addresses_map_.find(name);
  if (it_addresses == service_name_addresses_map_.end()) {
    WARNING() << "Not found";
    return false;
  }

  size_t removed = it_addresses->second.erase(address);
  if (removed == 0) {
    ERROR() << "No address removed";
    return false;
  }

  if (it_addresses->second.empty()) {
    service_name_addresses_map_.erase(it_addresses);
    service_name_signature_map_.erase(name);
  }

  DEBUG() << "current registey: " << DumpResgistry();
  return true;
}

std::string ServiceManager::DumpResgistry() const {
  std::stringstream ss;
  for (const auto& name_addresses : service_name_addresses_map_) {
    ss << name_addresses.first << ": ";
    for (const auto& address : name_addresses.second) {
      ss << address << " ";
    }
    ss << std::endl;
  }
  return ss.str();
}

}  // namespace registry_center
}  // namespace service_kit
