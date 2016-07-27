#include "ssf/layer/physical/udp_helpers.h"

#include <string>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>

#include <boost/system/error_code.hpp>

#include "ssf/error/error.h"
#include "ssf/utils/map_helpers.h"

#include "ssf/layer/parameters.h"

namespace ssf {
namespace layer {
namespace physical {
namespace detail {

boost::asio::ip::udp::endpoint make_udp_endpoint(
    boost::asio::io_service& io_service, const LayerParameters& parameters,
    boost::system::error_code& ec) {
  auto addr = helpers::GetField<std::string>("addr", parameters);
  auto port = helpers::GetField<std::string>("port", parameters);

  if (!port.empty()) {
    if (!addr.empty()) {
      boost::asio::ip::udp::resolver resolver(io_service);
      boost::asio::ip::udp::resolver::query query(addr, port);
      boost::asio::ip::udp::resolver::iterator iterator(
          resolver.resolve(query, ec));

      if (!ec) {
        return boost::asio::ip::udp::endpoint(*iterator);
      }
    } else {
      try {
        return boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(),
                                              (uint16_t)std::stoul(port));
      } catch (const std::exception&) {
        ec.assign(ssf::error::bad_address, ssf::error::get_ssf_category());
        return boost::asio::ip::udp::endpoint();
      }
    }
  }

  return boost::asio::ip::udp::endpoint();
}

}  // detail
}  // physical
}  // layer
}  // ssf
