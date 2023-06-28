/**
 * Common utilities to log and assert
 */

#pragma once

#include <exception>
#include <experimental/source_location>
#include <string>

using source_location = std::experimental::source_location;

namespace ivory {
void Assert(bool condition, const std::string& message,
            const source_location location = source_location::current()) {
  if (!condition) {
    std::stringstream ss;
    ss << location.file_name() << ":" << location.line() << "` " << location.function_name()
       << "` : " << message;
    throw std::runtime_error(ss.str());
  }
}

}  // namespace ivory