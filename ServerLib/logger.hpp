//
// logger.hpp
#ifndef SERVICES_LOGGER_HPP
#define SERVICES_LOGGER_HPP

#include "basic_logger.hpp"
#include "logger_service.hpp"

#include <stdarg.h>

namespace services {

/// Typedef for typical logger usage.
class logger : public basic_logger<logger_service>
{
    typedef basic_logger<logger_service> base;
public:
    explicit logger(boost::asio::io_service& io_service,
        const std::string& identifier)
        : base(io_service, identifier)
    {
    }

public:
    void Log(const std::string& str)
    {
        log(str);
    }

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
};

} // namespace services

extern std::shared_ptr<services::logger> logger_;

#endif // SERVICES_LOGGER_HPP
