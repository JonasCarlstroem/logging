#pragma once
#include "../base/logger.hpp"
#include <fstream>

namespace logging {

class file_logger : public logger {
    std::string file_name_;
    std::fstream fs_;

  public:
    file_logger(const std::string &file_name) : file_name_(file_name) {
        fs_ = std::fstream(file_name, fs_.out);
        if (!fs_.is_open()) {
            throw std::runtime_error("Error opening file.");
        }
    }

    void
    log(const std::string &message, log_level level, const char *file,
        int line) override {
        std::lock_guard<std::mutex> lock(mutex_);

        std::string out_message = format_message_meta(level, false, file, line);
        append(out_message, " ", message, "\n");
        print(fs_, out_message);
    }
};

} // namespace logging