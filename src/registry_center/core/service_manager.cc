#include "service_manager.h"

#include <cstdlib>
#include <ctime>
#include <sstream>

#include "log.h"

namespace registry_center {
namespace core {

ServiceManager::ServiceManager() {
  std::srand(std::time(nullptr));
}
ServiceManager::~ServiceManager() {}

bool ServiceManager::AddService(const std::string& name,
                                const std::string& address,
                                const std::string& signature) {
  TRACE() << "Adding service: " << name << std::endl;

  auto it_signature = service_name_signature_map_.find(name);
  if (it_signature == service_name_signature_map_.end()) {
    service_name_signature_map_.insert({name, signature});
  } else {
    if (it_signature->second != signature) {
      ERROR() << "Signature not match" << std::endl;
      return false;
    }
  }

  service_name_addresses_map_[name].insert(address);
  DEBUG() << "current registey: " << std::endl << DumpResgistry();

  return true;
}

bool ServiceManager::QueryService(const std::string& name,
                                  std::string* address) {
  TRACE() << "Querying service: " << name << std::endl;
  DEBUG() << "current registey: " << std::endl << DumpResgistry();

  auto it = service_name_addresses_map_.find(name);
  if (it == service_name_addresses_map_.end() || it->second.empty()) {
    WARNING() << "Not found" << std::endl;
    return false;
  }

  const auto& addresses = it->second;
  // TODO: 实现更多的负载均衡策略
  // 当前用蓄水池抽样算法以 1/N 的概率找一个出来
  int c = 0;
  for (const auto& addr : addresses) {
    if (std::rand() * (c + 1ULL) < RAND_MAX + 1ULL) {
      *address = addr;
    }
    ++c;
  }

  TRACE() << "Found service: " << *address << std::endl;
  return true;
}

bool ServiceManager::RemoveService(const std::string& name,
                                   const std::string& address) {
  TRACE() << "Removing service: " << name << std::endl;

  auto it_addresses = service_name_addresses_map_.find(name);
  if (it_addresses == service_name_addresses_map_.end()) {
    WARNING() << "Not found" << std::endl;
    return false;
  }

  size_t removed = it_addresses->second.erase(address);
  if (removed == 0) {
    ERROR() << "No address removed" << std::endl;
    return false;
  }

  if (it_addresses->second.empty()) {
    service_name_addresses_map_.erase(it_addresses);
    service_name_signature_map_.erase(name);
  }

  DEBUG() << "current registey: " << std::endl << DumpResgistry();
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

}  // namespace core
}  // namespace registry_center
