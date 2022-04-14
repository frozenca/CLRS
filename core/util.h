#ifndef __CLRS4_UTIL_H__
#define __CLRS4_UTIL_H__

#include <filesystem>
#include <format>
#include <iostream>
#include <map>
#include <ranges>
#include <source_location>
#include <string>
#include <string_view>

namespace frozenca {

using namespace std;

enum class log_level {
  debug,
  error,
  all,
};

static const map<log_level, string> log_level_str = {{log_level::debug, "[D]"},
                                                     {log_level::error, "[E]"}};

#ifdef NDEBUG
static constexpr log_level curr_log_level = log_level::error;
#else
static constexpr log_level curr_log_level = log_level::debug;
#endif

namespace {
template <typename... Args>
constexpr void log_impl(const string_view message, log_level level,
                        const source_location location, ostream &os,
                        Args &&...args) {
  string formatted_message = vformat(message, make_format_args(args...));
  if (level >= curr_log_level) {
    if (level == log_level::all) {
      os << formatted_message << '\n';
    } else {
      filesystem::path path = filesystem::canonical(location.file_name());
      os << log_level_str.at(level) << ":" << path << " (" << location.line()
         << ":" << location.column() << ") " << location.function_name()
         << " : " << formatted_message << '\n';
    }
  }
}

} // anonymous namespace

template <typename... Args>
constexpr void log(const string_view message,
                   log_level level = log_level::debug, Args &&...args) {

  ostream& log_stream = level == log_level::all ? cout : clog;                   
  log_impl(message, level, source_location::current(), log_stream, args...);
}

template <ranges::input_range R> void print(R &&r, ostream &os = cout) {
  for (auto elem : r) {
    os << elem << ' ';
  }
  os << '\n';
}

} // namespace frozenca

#endif //__CLRS4_UTIL_H__
