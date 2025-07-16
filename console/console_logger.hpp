#pragma once
#include "../base/logger.hpp"
#include <filesystem>
#include <proc/process>
#include <sstream>

namespace fs = std::filesystem;

namespace logging {

/// <summary>
/// Logger for the console. Supports current process console logging aswell as
/// external console logging.
/// </summary>
class console_logger : public logger {
    bool external_ = false;

    std::ostream* out_channel(const log_level& level) {
        if (external_)
            return &external_console_.get_ostream();

        return level == log_level::Error || level == log_level::Critical
                   ? &std::cerr
                   : &std::cout;
    }

  public:
    console_logger() = default;

    console_logger(bool external_console)
        : logger(), external_(external_console), external_console_() {
        if (external_console) {
            external_console_.opts_
                .with_application(get_external_console_path())
                .with_creation_flags(CREATE_NEW_CONSOLE)
                .redirect_stdin();

            if (!external_console_.start())
                throw std::runtime_error("Error launching external console");
        }
    }

    void
    log(const std::string& message, log_level level, const char* file,
        int line) override {
        std::lock_guard<std::mutex> lock(mutex_);

        auto now                = std::chrono::system_clock::now();
        std::time_t now_time    = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm          = *std::localtime(&now_time);
        std::ostream* channel   = out_channel(level);

        std::string color_code  = log_level_color_code(level);
        std::string level_str   = log_level_to_string(level);
        std::string out_message = format_message_meta(level, true, file, line);

        //append(
        //    out_message, color_code, "[",
        //    std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S"), "] [", level_str, "]"
        //);

        //if (file != nullptr) {
        //    append(out_message, " [", file, ":", line, "]:");
        //}

        append(out_message, " ", message, "\033[0m", "\n");

        print(*channel, out_message);
    }

  private:
    fs::path get_external_console_path() {
        char buf[4096];
        GetCurrentDirectory(4096, (LPSTR)&buf);
        fs::path p = fs::path(buf) /
                     fs::path("lib/logging/console/external_console.exe");

        if (fs::exists(p))
            return p;
        throw std::runtime_error("Error external console binary not found.");
    }

    struct external_console {
        external_console() = default;
        proc::process_options opts_;
        proc::process process_;
        proc::pipe* pipe_ = nullptr;

        bool start() {
            process_ = proc::process(opts_);
            process_.start();

            if (process_.is_running()) {
                pipe_ = &process_.standard_in();
            }
            return process_.is_running();
        }

        proc::pipe_ostream& get_ostream() { return pipe_->write_stream(); }

        proc::pipe_istream& get_istream() { return pipe_->read_stream(); }

        void close() {
            process_.kill();
            process_.wait();
        }
    } external_console_;
};

} // namespace logging