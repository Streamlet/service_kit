#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace service_kit {
namespace registry_center {

class ServiceManager {
 public:
  ServiceManager();
  ~ServiceManager();

  bool AddService(const std::string& name,
                  const std::string& address,
                  const std::string& signature);
  bool QueryService(const std::string& name, std::string* address);
  bool RemoveService(const std::string& name, const std::string& address);

 private:
  std::string DumpResgistry() const;

  std::unordered_map<std::string, std::string> service_name_signature_map_;
  std::unordered_map<std::string, std::unordered_set<std::string>>
      service_name_addresses_map_;
};

}  // namespace registry_center
}  // namespace service_kit
