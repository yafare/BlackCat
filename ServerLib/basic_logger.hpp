//
// basic_logger.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERVICES_BASIC_LOGGER_HPP
#define SERVICES_BASIC_LOGGER_HPP

#include <string>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>

namespace services {

/// Class to provide simple logging functionality. Use the services::logger
/// typedef.
template <typename Service>
class basic_logger
  : private boost::noncopyable
{
public:
  /// The type of the service that will be used to provide timer operations.
  typedef Service service_type;

  /// The native implementation type of the timer.
  typedef typename service_type::impl_type impl_type;

  /// Constructor.
  /**
   * This constructor creates a logger.
   *
   * @param io_service The io_service object used to locate the logger service.
   *
   * @param identifier An identifier for this logger.
   */
  explicit basic_logger(boost::asio::io_service& io_service,
      const std::string& identifier)
    : service_(boost::asio::use_service<Service>(io_service)),
      impl_(service_.null())
  {
    service_.create(impl_, identifier);
  }

  /// Destructor.
  ~basic_logger()
  {
    service_.destroy(impl_);
  }

  /// Get the io_service associated with the object.
  boost::asio::io_service& get_io_service()
  {
    return service_.get_io_service();
  }

  /// Set the output file for all logger instances.
  void use_file()
  {
      std::string dir = "./logs";
      if (boost::filesystem::exists(dir) == false) {
          boost::filesystem::create_directories(dir);
      }
      char filename[256];
      time_t t = time(0);
      tm* lt = localtime(&t);
      sprintf(filename, "./logs/log%04d%02d%02d_%02d%02d%02d.txt", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);

      std::string file = filename;
      service_.use_file(impl_, file);
  }

  /// Log a message.
  void log(const std::string& message)
  {
    service_.log(impl_, message);
  }

private:
  /// The backend service implementation.
  service_type& service_;

  /// The underlying native implementation.
  impl_type impl_;
};

} // namespace services

#endif // SERVICES_BASIC_LOGGER_HPP
