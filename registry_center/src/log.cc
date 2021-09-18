#include "log.h"
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <fstream>

namespace registry_center {

bool InitializeLog() {
  boost::log::add_common_attributes();
  std::ifstream log_settings_file("log_settings.ini");
  if (log_settings_file.is_open()) {
    boost::log::init_from_stream(log_settings_file);
  }
  return true;
}

}  // namespace registry_center
