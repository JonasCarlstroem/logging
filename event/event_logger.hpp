#pragma once
#include "../base/logger.hpp"
#include <winapi/handle>

namespace logging {

class event_logger : public logger {
    std::string event_source_name_;
    winapi::handle hEvLog;

    WORD log_level_to_event_type(log_level level) {
        switch (level) {
        case log_level::Debug:
        case log_level::Info:
            return EVENTLOG_INFORMATION_TYPE;
        case log_level::Warning:
            return EVENTLOG_WARNING_TYPE;
        case log_level::Error:
        case log_level::Critical:
            return EVENTLOG_ERROR_TYPE;
        default:
            0;
        }
    }

  public:
    event_logger(const std::string& source_name)
        : event_source_name_(source_name) {
        hEvLog = RegisterEventSourceA(nullptr, source_name.c_str());
    }

    ~event_logger() override {
        if (hEvLog) {
            DeregisterEventSource(hEvLog);
        }
    }

    void
    log(const std::string& message, log_level level, const char* file,
        int line) {
        std::lock_guard<std::mutex> lock(mutex_);

        std::string out_message = format_message_meta(level, false, file, line);
        append(out_message, " ", message, "\n");

        LPCSTR strings[1] = {out_message.c_str()};
        WORD type         = log_level_to_event_type(level);
        ReportEventA(hEvLog, type, 0, 0, nullptr, 1, 0, strings, nullptr);
    }
};

} // namespace logging