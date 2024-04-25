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

#ifndef _FASTDDS_TCP_TRANSPORT_INTERFACE_H_
#define _FASTDDS_TCP_TRANSPORT_INTERFACE_H_

#include <fastdds/rtps/transport/TransportInterface.h>
#include <fastdds/rtps/transport/TCPTransportDescriptor.h>
#include <fastrtps/utils/IPFinder.h>
#include <rtps/transport/tcp/RTCPHeader.h>
#include <rtps/transport/TCPChannelResourceBasic.h>
#include <rtps/transport/TCPAcceptorBasic.h>
#if TLS_FOUND
#define OPENSSL_API_COMPAT 10101
#include <rtps/transport/TCPAcceptorSecure.h>
#include <asio/ssl.hpp>
#endif // if TLS_FOUND

#include <statistics/rtps/messages/OutputTrafficManager.hpp>

#include <asio.hpp>
#include <asio/steady_timer.hpp>
#include <thread>
#include <vector>
#include <map>
#include <memory>
#include <mutex>

namespace eprosima {
namespace fastdds {
namespace rtps {

class RTCPMessageManager;
class TCPChannelResource;

/**
 * This is a default TCP Interface implementation.
 *    - Opening an output channel by passing a remote locator will try to open a TCP conection with the endpoint.
 *       If there is created a connection with the same endpoint, the transport will use the same one.
 *
 *    - It is possible to provide a white list at construction, which limits the interfaces the transport
 *       will ever be able to interact with. If left empty, all interfaces are allowed.
 *
 *    - Opening an input channel by passing a locator will open a socket listening on the given physical port on every
 *       whitelisted interface, it will wait for incoming connections until the receiver closes the channel.
 *       Several endpoints can connect to other to the same physical port, because the OS creates a connection socket
 *       after each establishment.
 * @ingroup TRANSPORT_MODULE
 */
class TCPTransportInterface : public TransportInterface
{
    class ReceiverInUseCV
    {
    public:

        uint16_t in_use = 0;

        std::condition_variable cv;
    };

    std::atomic<bool> alive_;

protected:

    std::vector<fastrtps::rtps::IPFinder::info_IP> current_interfaces_;
    asio::io_service io_service_;
    asio::io_service io_service_timers_;
    std::unique_ptr<asio::ip::tcp::socket> initial_peer_local_locator_socket_;
    uint16_t initial_peer_local_locator_port_;
    /**
     * Whether to use non-blocking calls to send().
     *
     * When set to true, calls to send() will return immediately if the send buffer is full.
     * This may happen when receive buffer on reader's side is full. No error will be returned
     * to the upper layer. This means that the application will behave
     * as if the datagram is sent but lost (i.e. throughput may be reduced). This value is
     * specially useful on high-frequency writers.
     *
     * When set to false, calls to send() will block until the send buffer has space for the
     * datagram. This may cause application lock.
     */
    bool non_blocking_send_;

#if TLS_FOUND
    asio::ssl::context ssl_context_;
#endif // if TLS_FOUND
    std::shared_ptr<std::thread> io_service_thread_;
    std::shared_ptr<std::thread> io_service_timers_thread_;
    std::shared_ptr<RTCPMessageManager> rtcp_message_manager_;
    std::mutex rtcp_message_manager_mutex_;
    std::condition_variable rtcp_message_manager_cv_;
    // Mutex to control access to channel_resources_
    mutable std::mutex sockets_map_mutex_;
    // Mutex to control access to unbound_channel_resources_
    mutable std::mutex unbound_map_mutex_;

    std::map<Locator, std::shared_ptr<TCPChannelResource>> channel_resources_; // The key is the "Physical locator"
    std::vector<std::shared_ptr<TCPChannelResource>> unbound_channel_resources_;
    // The key is the logical port
    std::map<uint16_t, std::pair<TransportReceiverInterface*, ReceiverInUseCV*>> receiver_resources_;

    std::vector<std::pair<TCPChannelResource*, uint64_t>> sockets_timestamp_;

    asio::steady_timer keep_alive_event_;

    std::map<Locator, std::shared_ptr<TCPAcceptor>> acceptors_;

    eprosima::fastdds::statistics::rtps::OutputTrafficManager statistics_info_;

    // Map containging the logical ports that must be added to a channel that has not been created yet. This could happen
    // with acceptor channels that are created after their output channel has been opened (LARGE_DATA case).
    // The key is the physical locator associated with the sender resource, and later to the channel.
    std::map<Locator, std::set<uint16_t>> channel_pending_logical_ports_;
    std::mutex channel_pending_logical_ports_mutex_;

    TCPTransportInterface(
            int32_t transport_kind);

    virtual bool compare_locator_ip(
            const Locator& lh,
            const Locator& rh) const = 0;

    virtual bool compare_locator_ip_and_port(
            const Locator& lh,
            const Locator& rh) const = 0;

    virtual void fill_local_ip(
            Locator& loc) const = 0;

    //! Methods to manage the TCP headers and their CRC values.
    bool check_crc(
            const TCPHeader& header,
            const fastrtps::rtps::octet* data,
            uint32_t size) const;

    void calculate_crc(
            TCPHeader& header,
            const fastrtps::rtps::octet* data,
            uint32_t size) const;

    void fill_rtcp_header(
            TCPHeader& header,
            const fastrtps::rtps::octet* send_buffer,
            uint32_t send_buffer_size,
            uint16_t logical_port) const;

    //! Closes the given p_channel_resource and unbind it from every resource.
    void close_tcp_socket(
            std::shared_ptr<TCPChannelResource>& channel);

    //! Creates a TCP acceptor to wait for incoming connections by the given locator.
    uint16_t create_acceptor_socket(
            const Locator& locator);

    virtual void get_ips(
            std::vector<fastrtps::rtps::IPFinder::info_IP>& loc_names,
            bool return_loopback = false) const = 0;

    bool is_input_port_open(
            uint16_t port) const;

    //! Functions to be called from new threads, which takes cares of performing a blocking receive
    void perform_listen_operation(
            std::weak_ptr<TCPChannelResource> channel,
            std::weak_ptr<RTCPMessageManager> rtcp_manager);

    bool read_body(
            fastrtps::rtps::octet* receive_buffer,
            uint32_t receive_buffer_capacity,
            uint32_t* bytes_received,
            std::shared_ptr<TCPChannelResource>& channel,
            std::size_t body_size);

    virtual void set_receive_buffer_size(
            uint32_t size) = 0;
    virtual void set_send_buffer_size(
            uint32_t size) = 0;

    void clean(); // Must be called on childs destructors!

    virtual void endpoint_to_locator(
            const asio::ip::tcp::endpoint& endpoint,
            Locator& locator) const = 0;

    /**
     * Shutdown method to close the connections of the transports.
     */
    void shutdown() override;

    /**
     * Applies TLS configuration to ssl_context
     *
     * @return true if everything worked fine, false otherwise.
     */
    bool apply_tls_config();

    /**
     * Aux method to retrieve cert password as a callback
     */
    std::string get_password() const;

    /**
     * Send a buffer to a destination indicated by the locator.
     * There must exist a channel bound to the locator, otherwise the send will be skipped.
     */
    bool send(
            const fastrtps::rtps::octet* send_buffer,
            uint32_t send_buffer_size,
            const eprosima::fastrtps::rtps::Locator_t& locator,
            const Locator& remote_locator);

public:

    friend class RTCPMessageManager;

    virtual ~TCPTransportInterface();

    //! Stores the binding between the given locator and the given TCP socket. Server side.
    void bind_socket(
            std::shared_ptr<TCPChannelResource>&);

    //! Removes the listening socket for the specified port.
    bool CloseInputChannel(
            const Locator&) override;

    //! Resets the locator bound to the sender resource.
    void CloseOutputChannel(
            fastrtps::rtps::Locator_t& locator);

    //! Reports whether Locators correspond to the same port.
    bool DoInputLocatorsMatch(
            const Locator&,
            const Locator&) const override;

    virtual asio::ip::tcp::endpoint generate_endpoint(
            uint16_t port) const = 0;

    virtual asio::ip::tcp::endpoint generate_endpoint(
            const Locator& loc,
            uint16_t port) const = 0;

    virtual asio::ip::tcp::endpoint generate_local_endpoint(
            Locator& loc,
            uint16_t port) const = 0;

    virtual asio::ip::tcp generate_protocol() const = 0;

    virtual asio::ip::tcp get_protocol_type() const = 0;

    virtual uint16_t GetLogicalPortIncrement() const = 0;

    virtual uint16_t GetLogicalPortRange() const = 0;

    virtual uint16_t GetMaxLogicalPort() const = 0;

    bool init(
            const fastrtps::rtps::PropertyPolicy* properties = nullptr) override;

    //! Checks whether there are open and bound sockets for the given port.
    bool IsInputChannelOpen(
            const Locator&) const override;

    //! Checks if the interfaces white list is empty.
    virtual bool is_interface_whitelist_empty() const = 0;

    /**
     * Checks if the given locator is allowed by the white list.
     * @param loc locator to check.
     * @return True if the locator passes the white list.
     */
    virtual bool is_interface_allowed(
            const Locator& loc) const = 0;

    virtual bool is_interface_allowed(
            const std::string& interface) const = 0;

    //! Checks for TCP kinds.
    bool IsLocatorSupported(
            const Locator&) const override;

    //! Checks whether there are open and bound sockets for the given port.
    bool is_output_channel_open_for(
            const Locator&) const;

    /** Opens an input channel to receive incoming connections.
     *   If there is an existing channel it registers the receiver resource.
     */
    virtual bool OpenInputChannel(
            const Locator&,
            TransportReceiverInterface*,
            uint32_t) override;

    //! Opens a socket on the given address and port (as long as they are white listed).
    bool OpenOutputChannel(
            SendResourceList& send_resource_list,
            const Locator&) override;

    /**
     * Converts a given remote locator (that is, a locator referring to a remote
     * destination) to the main local locator whose channel can write to that
     * destination. In this case it will return a 0.0.0.0 address on that port.
     */
    Locator RemoteToMainLocal(
            const Locator&) const override;

    /**
     * Transforms a remote locator into a locator optimized for local communications.
     *
     * If the remote locator corresponds to one of the local interfaces, it is converted
     * to the corresponding local address.
     *
     * @param [in]  remote_locator Locator to be converted.
     * @param [out] result_locator Converted locator.
     *
     * @return false if the input locator is not supported/allowed by this transport, true otherwise.
     */
    bool transform_remote_locator(
            const Locator& remote_locator,
            Locator& result_locator) const override;

    /**
     * Blocking Receive from the specified channel.
     * @param rtcp_manager pointer to the RTCP Manager.
     * @param channel pointer to the socket where the method is going to read the messages.
     * @param receive_buffer vector with enough capacity (not size) to accomodate a full receive buffer. That
     * capacity must not be less than the receive_buffer_size supplied to this class during construction.
     * @param receive_buffer_capacity maximum size of the buffer.
     * @param[out] receive_buffer_size Size of the packet received.
     * @param[out] remote_locator associated remote locator.
     */
    bool Receive(
            std::weak_ptr<RTCPMessageManager>& rtcp_manager,
            std::shared_ptr<TCPChannelResource>& channel,
            fastrtps::rtps::octet* receive_buffer,
            uint32_t receive_buffer_capacity,
            uint32_t& receive_buffer_size,
            Locator& remote_locator);

    /**
     * Blocking Send through the channel inside channel_resources_ matching the locator provided.
     * @param send_buffer Slice into the raw data to send.
     * @param send_buffer_size Size of the raw data. It will be used as a bounds check for the previous argument.
     * It must not exceed the send_buffer_size fed to this class during construction.
     * @param locator Physical locator we're sending to.
     * @param destination_locators_begin pointer to destination locators iterator begin, the iterator can be advanced inside this fuction
     * so should not be reuse.
     * @param destination_locators_end pointer to destination locators iterator end, the iterator can be advanced inside this fuction
     * so should not be reuse.
     */
    bool send(
            const fastrtps::rtps::octet* send_buffer,
            uint32_t send_buffer_size,
            const fastrtps::rtps::Locator_t& locator,
            fastrtps::rtps::LocatorsIterator* destination_locators_begin,
            fastrtps::rtps::LocatorsIterator* destination_locators_end);

    /**
     * Performs the locator selection algorithm for this transport.
     *
     * It basically consists of the following steps
     *   - selector.transport_starts is called
     *   - transport handles the selection state of each locator
     *   - if a locator from an entry is selected, selector.select is called for that entry
     *
     * In the case of TCP, multicast locators are never selected. All TCPv6 unicast locators
     * are selected. For TCPv4, only locators on the same WAN as the transport or with the
     * WAN address of a connected channel are selected.
     *
     * @param [in, out] selector Locator selector.
     */
    void select_locators(
            fastrtps::rtps::LocatorSelector& selector) const override;

    //! Callback called each time that an incoming connection is accepted.
    void SocketAccepted(
            std::shared_ptr<asio::ip::tcp::socket> socket,
            const Locator& locator,
            const asio::error_code& error);

#if TLS_FOUND
    //! Callback called each time that an incoming connection is accepted (secure).
    void SecureSocketAccepted(
            std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> socket,
            const Locator& locator,
            const asio::error_code& error);
#endif // if TLS_FOUND

    //! Callback called each time that an outgoing connection is established.
    void SocketConnected(
            const std::weak_ptr<TCPChannelResource>& channel,
            const asio::error_code& error);

    /**
     * Method to get a list of binding interfaces.
     * @return Vector of interfaces in string format.
     */
    virtual std::vector<std::string> get_binding_interfaces_list() = 0;

    bool getDefaultMetatrafficMulticastLocators(
            LocatorList& locators,
            uint32_t metatraffic_multicast_port) const override;

    bool getDefaultMetatrafficUnicastLocators(
            LocatorList& locators,
            uint32_t metatraffic_unicast_port) const override;

    bool getDefaultUnicastLocators(
            LocatorList& locators,
            uint32_t unicast_port) const override;

    bool fillMetatrafficMulticastLocator(
            Locator& locator,
            uint32_t metatraffic_multicast_port) const override;

    bool fillMetatrafficUnicastLocator(
            Locator& locator,
            uint32_t metatraffic_unicast_port) const override;

    bool configureInitialPeerLocator(
            Locator& locator,
            const fastrtps::rtps::PortParameters& port_params,
            uint32_t domainId,
            LocatorList& list) const override;

    bool fillUnicastLocator(
            Locator& locator,
            uint32_t well_known_port) const override;

    uint32_t max_recv_buffer_size() const override
    {
        return configuration()->maxMessageSize;
    }

    void DeleteSocket(
            TCPChannelResource* channelResource);

    virtual const TCPTransportDescriptor* configuration() const = 0;

    virtual TCPTransportDescriptor* configuration() = 0;

    void keep_alive();

    void update_network_interfaces() override;

    /**
     * Method to fill local locator physical port.
     * @param locator locator to be filled.
     */
    void fill_local_physical_port(
            Locator& locator) const;

    bool get_non_blocking_send() const
    {
        return non_blocking_send_;
    }

    /**
     * Method to add the logical ports associated to a channel that was not available
     * when the logical ports were obtained.
     * @param channel Channel that might add the logical ports if available.
     */
    void send_channel_pending_logical_ports(
            std::shared_ptr<TCPChannelResource>& channel);
};

} // namespace rtps
} // namespace fastdds
} // namespace eprosima

#endif // _FASTDDS_TCP_TRANSPORT_INTERFACE_H_
