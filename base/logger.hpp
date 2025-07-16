#pragma once
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>

namespace logging {

enum class log_level { Debug, Info, Warning, Error, Critical };

/// <summary>
/// Abstract logger
/// </summary>
class logger {
  protected:
    std::mutex mutex_;

    std::string log_level_to_string(log_level level) {
        switch (level) {
        case log_level::Debug:
            return "DEBUG";
        case log_level::Info:
            return "INFO";
        case log_level::Warning:
            return "WARNING";
        case log_level::Error:
            return "ERROR";
        case log_level::Critical:
            return "CRITICAL";
        default:
            return "UNKNOWN";
        }
    }

    std::string log_level_color_code(log_level level) {
        switch (level) {
        case log_level::Debug:
            return "\033[36m";
        case log_level::Info:
            return "\033[0m";
        case log_level::Warning:
            return "\033[33m";
        case log_level::Error:
            return "\033[31m";
        case log_level::Critical:
            return "\033[35m";
        default:
            return "\033[0m";
        }
    }

    inline static void print(std::ostream& os, const std::string& message) {
        os << message;
    }

    template <typename... T>
    std::string format(const T&... args) {
        std::ostringstream oss;
        ((oss << args), ...);
        return oss.str();
    }

    template <typename... T>
    void append(std::string& s, T&&... args) {
        std::ostringstream oss;
        ((oss << std::forward<T>(args)), ...);
        s.append(oss.str());
    }

    std::string format_message_meta(
      const log_level& level, bool use_color, const char* file, int line
    ) {
        std::string formatted;
        auto now               = std::chrono::system_clock::now();
        std::time_t now_time   = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm         = *std::localtime(&now_time);

        std::string color_code = log_level_color_code(level);
        std::string level_str  = log_level_to_string(level);

        if (use_color) {
            append(
              formatted, color_code, " [", get_now(), "] [", level_str, "]"
            );
        } else {
            append(formatted, "[", get_now(), "] [", level_str, "]");
        }

        if (file != nullptr) {
            append(formatted, " [", file, ":", line, "]:");
        }

        return formatted;
    }

    std::string get_now(const std::string& format = "%Y-%m-%d %H:%M:%S") {
        auto now             = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm       = *std::localtime(&now_time);

        std::ostringstream oss;
        oss << std::put_time(&now_tm, format.c_str());

        return oss.str();
    }

  public:
    virtual ~logger() {}
    virtual void log(
      const std::string& message, log_level level, const char* file, int line
    ) = 0;

#define LOG(logger_, level, message)                                           \
    (logger_).log(message, level, __FILE__, __LINE__)
#define LOG_DEBUG(logger_, message)                                            \
    LOG(logger_, logging::log_level::Debug, message)
#define LOG_INFO(logger_, message)                                             \
    LOG(logger_, logging::log_level::Info, message)
#define LOG_WARNING(logger_, message)                                          \
    LOG(logger_, logging::log_level::Warning, message)
#define LOG_ERROR(logger_, message)                                            \
    LOG(logger_, logging::log_level::Error, message)
#define LOG_CRITICAL(logger_, message)                                         \
    LOG(logger_, logging::log_level::Critical, message)
};

} // namespace logging