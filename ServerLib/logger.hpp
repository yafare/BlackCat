//
// logger.hpp
#ifndef SERVICES_LOGGER_HPP
#define SERVICES_LOGGER_HPP

#include "basic_logger.hpp"
#include "logger_service.hpp"

#include <stdarg.h>

#define NEW_LOG_FILE_INTERVAL 2 // hours

namespace services {

/// Typedef for typical logger usage.
class logger : public basic_logger<logger_service>
{
    typedef basic_logger<logger_service> base;
public:
    explicit logger(boost::asio::io_service& io_service, const std::string& identifier) :
        base(io_service, identifier),
        new_log_file_timer_(io_service)
    {
        use_file();
        ResetLoggerTimer();
    }

public:
    void Log(const char *fmt, ...)
    {
        if (strlen(fmt) > 1020) {
            return;
        }

        char buf[10 * 1024];
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(buf, sizeof(buf), fmt, ap);
        va_end(ap);

        log(buf);
    }

private:
    void Log(const std::string& str)
    {
        log(str);
    }

    void ResetLoggerTimer()
    {
        auto check_new_log_file = [&](boost::asio::deadline_timer * /*deadline*/) {
            use_file();
            ResetLoggerTimer();
        };

        new_log_file_timer_.expires_from_now(boost::posix_time::hours(NEW_LOG_FILE_INTERVAL));
        new_log_file_timer_.async_wait(std::bind(check_new_log_file, &new_log_file_timer_));
    }

private:
    // logger timer
    boost::asio::deadline_timer new_log_file_timer_;
};

} // namespace services

extern std::shared_ptr<services::logger> logger_;
#define LOG(...) \
{ \
    if (logger_) { \
        logger_->Log(__VA_ARGS__); \
    } \
}

#endif // SERVICES_LOGGER_HPP
