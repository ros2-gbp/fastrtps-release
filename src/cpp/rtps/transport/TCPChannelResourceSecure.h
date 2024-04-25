// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _FASTDDS_TCP_CHANNEL_RESOURCE_SECURE_
#define _FASTDDS_TCP_CHANNEL_RESOURCE_SECURE_

#define OPENSSL_API_COMPAT 10101

#include <asio.hpp>
#include <asio/ssl.hpp>
#include <asio/strand.hpp>
#include <rtps/transport/TCPChannelResource.h>

namespace eprosima {
namespace fastdds {
namespace rtps {

class TCPChannelResourceSecure : public TCPChannelResource
{
public:

    // Constructor called when trying to connect to a remote server (secure version)
    TCPChannelResourceSecure(
            TCPTransportInterface* parent,
            asio::io_service& service,
            asio::ssl::context& ssl_context,
            const Locator& locator,
            uint32_t maxMsgSize);

    // Constructor called when local server accepted connection (secure version)
    TCPChannelResourceSecure(
            TCPTransportInterface* parent,
            asio::io_service& service,
            asio::ssl::context& ssl_context,
            std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> socket,
            uint32_t maxMsgSize);

    virtual ~TCPChannelResourceSecure();

    void connect(
            const std::shared_ptr<TCPChannelResource>& myself) override;

    void disconnect() override;

    uint32_t read(
            fastrtps::rtps::octet* buffer,
            std::size_t size,
            asio::error_code& ec) override;

    size_t send(
            const fastrtps::rtps::octet* header,
            size_t header_size,
            const fastrtps::rtps::octet* data,
            size_t size,
            asio::error_code& ec) override;

    asio::ip::tcp::endpoint remote_endpoint() const override;
    asio::ip::tcp::endpoint local_endpoint() const override;

    void set_options(
            const TCPTransportDescriptor* options) override;

    void set_tls_verify_mode(
            const TCPTransportDescriptor* options);

    void cancel() override;
    void close() override;
    void shutdown(
            asio::socket_base::shutdown_type what) override;

    inline std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> secure_socket()
    {
        return secure_socket_;
    }

private:

    TCPChannelResourceSecure(
            const TCPChannelResource&) = delete;
    TCPChannelResourceSecure& operator =(
            const TCPChannelResource&) = delete;

    asio::io_service& service_;
    asio::ssl::context& ssl_context_;
    asio::io_service::strand strand_read_;
    asio::io_service::strand strand_write_;
    std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> secure_socket_;
};


} // namespace rtps
} // namespace fastdds
} // namespace eprosima

#endif // _FASTDDS_TCP_CHANNEL_RESOURCE_SECURE_
