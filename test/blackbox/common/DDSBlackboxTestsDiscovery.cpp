// Copyright 2020 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#include <cstdlib>
#include <ctime>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "BlackboxTests.hpp"
#include "PubSubParticipant.hpp"
#include "PubSubReader.hpp"
#include "PubSubWriter.hpp"

#include <fastdds/dds/core/policy/ParameterTypes.hpp>
#include <fastdds/dds/core/policy/QosPolicies.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastdds/rtps/builtin/data/ParticipantProxyData.h>
#include <fastdds/rtps/common/Locator.h>
#include <fastdds/rtps/participant/ParticipantDiscoveryInfo.h>
#include <fastdds/rtps/transport/test_UDPv4TransportDescriptor.h>
#include <fastrtps/xmlparser/XMLProfileManager.h>
#include <rtps/transport/test_UDPv4Transport.h>
#include <utils/SystemInfo.hpp>

// Regression test for redmine issue 11857
TEST(DDSDiscovery, IgnoreParticipantFlags)
{
    // This participant is created with:
    // - ignoreParticipantFlags = FILTER_SAME_PROCESS (will avoid discovery of p2)
    // - metatrafficUnicastLocatorList = 127.0.0.1:7399, 127.0.0.1:7398 (to ensure two listening threads are created)
    PubSubReader<HelloWorldPubSubType> p1(TEST_TOPIC_NAME);
    p1.set_xml_filename("discovery_participant_flags.xml");
    p1.set_participant_profile("participant_1");
    p1.init();
    EXPECT_TRUE(p1.isInitialized());

    // This participant is created with initialPeersList = 127.0.0.1:7399
    // When the announcements of this participant arrive to p1, they will be ignored, and thus p1 will not
    // announce itself back to p2.
    PubSubReader<HelloWorldPubSubType> p2(TEST_TOPIC_NAME);
    p2.set_xml_filename("discovery_participant_flags.xml");
    p2.set_participant_profile("participant_2");
    p2.init();
    EXPECT_TRUE(p2.isInitialized());
    EXPECT_FALSE(p2.wait_participant_discovery(1, std::chrono::seconds(1)));
    EXPECT_FALSE(p1.wait_participant_discovery(1, std::chrono::seconds(1)));

    // This participant is created with:
    // - initialPeersList = 127.0.0.1:7398
    // - a custom guid prefix
    // The announcements of this participant will arrive to p1 on a different listening thread.
    // Due to the custom prefix, they should not be ignored, and mutual discovery should happen
    PubSubReader<HelloWorldPubSubType> p3(TEST_TOPIC_NAME);
    p3.set_xml_filename("discovery_participant_flags.xml");
    p3.set_participant_profile("participant_3");
    p3.init();
    EXPECT_TRUE(p1.wait_participant_discovery());
    EXPECT_TRUE(p3.wait_participant_discovery());
}

/**
 * This test checks that adding servers to the Discovery Server list results in discovering those participants.
 * It does so by:
 *    1. Creating two servers and one client that is only connected to one server. At this point check discovery
 *       state.
 *    2. Then, connect the client to the other server and check discovery again.
 *    3. Finally connect the two servers by adding one of them to the others list
 */
TEST(DDSDiscovery, AddDiscoveryServerToListUDP)
{
    using namespace eprosima;
    using namespace eprosima::fastdds::dds;
    using namespace eprosima::fastrtps::rtps;

    /* Get random port from the environment */
    std::string value;
    if (eprosima::ReturnCode_t::RETCODE_OK != SystemInfo::get_env("W_UNICAST_PORT_RANDOM_NUMBER", value))
    {
        value = std::string("11811");
    }

    /* Create first server */
    PubSubParticipant<HelloWorldPubSubType> server_1(0u, 0u, 0u, 0u);
    // Set participant as server
    WireProtocolConfigQos server_1_qos;
    server_1_qos.builtin.discovery_config.discoveryProtocol = DiscoveryProtocol_t::SERVER;
    // Generate random GUID prefix
    srand(static_cast<unsigned>(time(nullptr)));
    GuidPrefix_t server_1_prefix;
    for (auto i = 0; i < 12; i++)
    {
        server_1_prefix.value[i] = eprosima::fastrtps::rtps::octet(rand() % 254);
    }
    server_1_qos.prefix = server_1_prefix;
    // Generate server's listening locator
    Locator_t locator_server_1;
    IPLocator::setIPv4(locator_server_1, 127, 0, 0, 1);
    uint32_t server_1_port = atol(value.c_str());
    locator_server_1.port = server_1_port;
    server_1_qos.builtin.metatrafficUnicastLocatorList.push_back(locator_server_1);
    // Init server
    ASSERT_TRUE(server_1.wire_protocol(server_1_qos).init_participant());

    /* Create second server */
    PubSubParticipant<HelloWorldPubSubType> server_2(0u, 0u, 0u, 0u);
    // Set participant as server
    WireProtocolConfigQos server_2_qos;
    server_2_qos.builtin.discovery_config.discoveryProtocol = DiscoveryProtocol_t::SERVER;
    // Generate random GUID prefix
    GuidPrefix_t server_2_prefix = server_1_prefix;
    server_2_prefix.value[11]++;
    server_2_qos.prefix = server_2_prefix;
    // Generate server's listening locator
    Locator_t locator_server_2;
    IPLocator::setIPv4(locator_server_2, 127, 0, 0, 1);
    uint32_t server_2_port = atol(value.c_str()) + 1;
    locator_server_2.port = server_2_port;
    server_2_qos.builtin.metatrafficUnicastLocatorList.push_back(locator_server_2);
    // Init server
    ASSERT_TRUE(server_2.wire_protocol(server_2_qos).init_participant());

    /* Create a client that connects to the first server from the beginning */
    PubSubParticipant<HelloWorldPubSubType> client(0u, 0u, 0u, 0u);
    // Set participant as client
    WireProtocolConfigQos client_qos;
    client_qos.builtin.discovery_config.discoveryProtocol = DiscoveryProtocol_t::CLIENT;
    // Connect to first server
    RemoteServerAttributes server_1_att;
    server_1_att.guidPrefix = server_1_prefix;
    server_1_att.metatrafficUnicastLocatorList.push_back(Locator_t(locator_server_1));
    client_qos.builtin.discovery_config.m_DiscoveryServers.push_back(server_1_att);
    // Init client
    ASSERT_TRUE(client.wire_protocol(client_qos).init_participant());

    /**
     * Check that server_1 and client_1 only know each other, and that server_2 does has not
     * discovered anyone
     */
    server_1.wait_discovery(std::chrono::seconds::zero(), 1, true);
    client.wait_discovery(std::chrono::seconds::zero(), 1, true);
    server_2.wait_discovery(std::chrono::seconds::zero(), 0, true);

    /* Add server_2 to client */
    RemoteServerAttributes server_2_att;
    server_2_att.guidPrefix = server_2_prefix;
    server_2_att.metatrafficUnicastLocatorList.push_back(Locator_t(locator_server_2));
    client_qos.builtin.discovery_config.m_DiscoveryServers.push_back(server_2_att);
    // Update client's servers list
    ASSERT_TRUE(client.update_wire_protocol(client_qos));

    /* Check that the servers only know about the client and that the client knows about both servers */
    server_1.wait_discovery(std::chrono::seconds::zero(), 1, true);
    client.wait_discovery(std::chrono::seconds::zero(), 2, true);
    server_2.wait_discovery(std::chrono::seconds::zero(), 1, true);

    /* Add server_2 to server_1 */
    server_1_qos.builtin.discovery_config.m_DiscoveryServers.push_back(server_2_att);
    ASSERT_TRUE(server_1.update_wire_protocol(server_1_qos));

    /* Check that they all know each other */
    server_1.wait_discovery(std::chrono::seconds::zero(), 2, true);
    client.wait_discovery(std::chrono::seconds::zero(), 2, true);
    server_2.wait_discovery(std::chrono::seconds::zero(), 2, true);
}

/**
 * This test checks that adding servers to the Discovery Server list results in discovering those participants.
 * It does so by:
 *    1. Creating two servers and two clients that are only connected to the first server. Discovery is checked
 *       at this state.
 *    2. Then, connect client_1 to the second server and check discovery again.
 *    3. Finally connect the two servers by adding one of them to the others list and check disvoery again.
 */
TEST(DDSDiscovery, AddDiscoveryServerToListTCP)
{
    using namespace eprosima;
    using namespace eprosima::fastdds::dds;
    using namespace eprosima::fastrtps::rtps;

    // TCP default DS port
    constexpr uint16_t W_UNICAST_PORT_RANDOM_NUMBER_STR = 42100;

    /* Create first server */
    PubSubParticipant<HelloWorldPubSubType> server_1(0u, 0u, 0u, 0u);
    // Set participant as server
    WireProtocolConfigQos server_1_qos;
    // Generate random GUID prefix
    srand(static_cast<unsigned>(time(nullptr)));
    GuidPrefix_t server_1_prefix;
    for (auto i = 0; i < 12; i++)
    {
        server_1_prefix.value[i] = eprosima::fastrtps::rtps::octet(rand() % 254);
    }
    uint16_t server_1_port = W_UNICAST_PORT_RANDOM_NUMBER_STR;
    Locator_t locator_server_1;
    // Add TCP transport
    auto descriptor_1 = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
    descriptor_1->add_listener_port(server_1_port);

    // Init server
    ASSERT_TRUE(server_1.fill_server_qos(server_1_qos, server_1_prefix, locator_server_1, server_1_port,
            LOCATOR_KIND_TCPv4)
                    .disable_builtin_transport()
                    .add_user_transport_to_pparams(descriptor_1)
                    .init_participant());

    /* Create second server */
    PubSubParticipant<HelloWorldPubSubType> server_2(0u, 0u, 0u, 0u);
    // Set participant as server
    WireProtocolConfigQos server_2_qos;
    GuidPrefix_t server_2_prefix = server_1_prefix;
    server_2_prefix.value[11]++;
    Locator_t locator_server_2;
    uint16_t server_2_port = server_1_port + 1;
    // Add TCP transport
    auto descriptor_2 = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
    descriptor_2->add_listener_port(server_2_port);

    // Init server
    ASSERT_TRUE(server_2.fill_server_qos(server_2_qos, server_2_prefix, locator_server_2, server_2_port,
            LOCATOR_KIND_TCPv4)
                    .disable_builtin_transport()
                    .add_user_transport_to_pparams(descriptor_2)
                    .init_participant());


    /* Create a client that connects to the first server from the beginning with higher listening_port*/
    PubSubParticipant<HelloWorldPubSubType> client_1(0u, 0u, 0u, 0u);
    // Set participant as client
    WireProtocolConfigQos client_qos_1;
    client_qos_1.builtin.discovery_config.discoveryProtocol = DiscoveryProtocol_t::CLIENT;
    // Connect to first server
    RemoteServerAttributes server_1_att;
    server_1_att.guidPrefix = server_1_prefix;
    server_1_att.metatrafficUnicastLocatorList.push_back(Locator_t(locator_server_1));
    client_qos_1.builtin.discovery_config.m_DiscoveryServers.push_back(server_1_att);
    auto descriptor_3 = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
    uint16_t client_1_port = server_1_port + 10;
    descriptor_3->add_listener_port(client_1_port);
    // Init client
    ASSERT_TRUE(client_1.wire_protocol(client_qos_1)
                    .disable_builtin_transport()
                    .add_user_transport_to_pparams(descriptor_3)
                    .init_participant());

    /* Create a client that connects to the first server from the beginning with lower listening_port*/
    PubSubParticipant<HelloWorldPubSubType> client_2(0u, 0u, 0u, 0u);
    // Set participant as client
    WireProtocolConfigQos client_qos_2;
    client_qos_2.builtin.discovery_config.discoveryProtocol = DiscoveryProtocol_t::CLIENT;
    // Connect to first server
    client_qos_2.builtin.discovery_config.m_DiscoveryServers.push_back(server_1_att);
    auto descriptor_4 = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
    uint16_t client_2_port = server_1_port - 10;
    descriptor_4->add_listener_port(client_2_port);
    // Init client
    ASSERT_TRUE(client_2.wire_protocol(client_qos_2)
                    .disable_builtin_transport()
                    .add_user_transport_to_pparams(descriptor_4)
                    .init_participant());

    server_1.wait_discovery(std::chrono::seconds::zero(), 2, true); // Knows client1 and client2
    client_1.wait_discovery(std::chrono::seconds::zero(), 1, true); // Knows server1
    client_2.wait_discovery(std::chrono::seconds::zero(), 1, true); // Knows server1
    server_2.wait_discovery(std::chrono::seconds::zero(), 0, true); // Knows no one

    /* Add server_2 to client */
    RemoteServerAttributes server_2_att;
    server_2_att.guidPrefix = server_2_prefix;
    server_2_att.metatrafficUnicastLocatorList.push_back(Locator_t(locator_server_2));
    client_qos_1.builtin.discovery_config.m_DiscoveryServers.push_back(server_2_att);
    // Update client_1's servers list
    ASSERT_TRUE(client_1.update_wire_protocol(client_qos_1));

    server_1.wait_discovery(std::chrono::seconds::zero(), 2, true); // Knows client1 and client2
    client_1.wait_discovery(std::chrono::seconds::zero(), 2, true); // Knows server1 and server2
    client_2.wait_discovery(std::chrono::seconds::zero(), 1, true); // Knows server1
    server_2.wait_discovery(std::chrono::seconds::zero(), 1, true); // Knows client1

    /* Add server_2 to server_1 */
    server_1_qos.builtin.discovery_config.m_DiscoveryServers.push_back(server_2_att);
    ASSERT_TRUE(server_1.update_wire_protocol(server_1_qos));

    server_1.wait_discovery(std::chrono::seconds::zero(), 3, true); // Knows client1, client2 and server2
    client_1.wait_discovery(std::chrono::seconds::zero(), 2, true); // Knows server1 and server2
    client_2.wait_discovery(std::chrono::seconds::zero(), 1, true); // Knows server1
    server_2.wait_discovery(std::chrono::seconds::zero(), 2, true); // Knows client1 and server1
}

TEST(DDSDiscovery, ServersConnectionTCP)
{
    using namespace eprosima;
    using namespace eprosima::fastdds::dds;
    using namespace eprosima::fastrtps::rtps;

    // TCP default DS port
    constexpr uint16_t W_UNICAST_PORT_RANDOM_NUMBER_STR = 41100;

    /* Create first server */
    PubSubParticipant<HelloWorldPubSubType> server_1(0u, 0u, 0u, 0u);
    // Set participant as server
    WireProtocolConfigQos server_1_qos;
    // Generate random GUID prefix
    srand(static_cast<unsigned>(time(nullptr)));
    GuidPrefix_t server_1_prefix;
    for (auto i = 0; i < 12; i++)
    {
        server_1_prefix.value[i] = eprosima::fastrtps::rtps::octet(rand() % 254);
    }
    Locator_t locator_server_1;
    uint16_t server_1_port = W_UNICAST_PORT_RANDOM_NUMBER_STR;
    // Add TCP transport
    auto descriptor_1 = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
    descriptor_1->add_listener_port(server_1_port);
    // Init server
    ASSERT_TRUE(server_1.fill_server_qos(server_1_qos, server_1_prefix, locator_server_1, server_1_port,
            LOCATOR_KIND_TCPv4)
                    .disable_builtin_transport()
                    .add_user_transport_to_pparams(descriptor_1)
                    .init_participant());

    /* Create second server */
    PubSubParticipant<HelloWorldPubSubType> server_2(0u, 0u, 0u, 0u);
    // Set participant as server
    WireProtocolConfigQos server_2_qos;
    GuidPrefix_t server_2_prefix = server_1_prefix;
    server_2_prefix.value[11]++;
    Locator_t locator_server_2;
    uint16_t server_2_port = server_1_port + 1;
    // Add TCP transport
    auto descriptor_2 = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
    descriptor_2->add_listener_port(server_2_port);

    // Connect to first server
    RemoteServerAttributes server_1_att;
    server_1_att.guidPrefix = server_1_prefix;
    server_1_att.metatrafficUnicastLocatorList.push_back(Locator_t(locator_server_1));
    server_2_qos.builtin.discovery_config.m_DiscoveryServers.push_back(server_1_att);

    // Init server
    ASSERT_TRUE(server_2.fill_server_qos(server_2_qos, server_2_prefix, locator_server_2, server_2_port,
            LOCATOR_KIND_TCPv4)
                    .disable_builtin_transport()
                    .add_user_transport_to_pparams(descriptor_2)
                    .init_participant());

    /* Create third server */
    PubSubParticipant<HelloWorldPubSubType> server_3(0u, 0u, 0u, 0u);
    // Set participant as server
    WireProtocolConfigQos server_3_qos;
    GuidPrefix_t server_3_prefix = server_1_prefix;
    server_3_prefix.value[11]--;
    Locator_t locator_server_3;
    uint16_t server_3_port = server_1_port - 1;
    // Add TCP transport
    auto descriptor_3 = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
    descriptor_3->add_listener_port(server_3_port);
    // Connect to first server
    server_3_qos.builtin.discovery_config.m_DiscoveryServers.push_back(server_1_att);

    // Init server
    ASSERT_TRUE(server_3.fill_server_qos(server_3_qos, server_3_prefix, locator_server_3, server_3_port,
            LOCATOR_KIND_TCPv4)
                    .disable_builtin_transport()
                    .add_user_transport_to_pparams(descriptor_3)
                    .init_participant());

    // Check adding servers before initialization
    server_1.wait_discovery(std::chrono::seconds::zero(), 2, true); // Knows server2 and server3
    server_2.wait_discovery(std::chrono::seconds::zero(), 1, true); // Knows server1
    server_3.wait_discovery(std::chrono::seconds::zero(), 1, true); // Knows server1

    /* Add server_3 to server_2 */
    RemoteServerAttributes server_3_att;
    server_3_att.guidPrefix = server_3_prefix;
    server_3_att.metatrafficUnicastLocatorList.push_back(Locator_t(locator_server_3));
    server_2_qos.builtin.discovery_config.m_DiscoveryServers.push_back(server_3_att);
    ASSERT_TRUE(server_2.update_wire_protocol(server_2_qos));

    // Check adding servers after initialization
    server_1.wait_discovery(std::chrono::seconds::zero(), 2, true); // Knows server2 and server3
    server_2.wait_discovery(std::chrono::seconds::zero(), 2, true); // Knows server1 and server3
    server_3.wait_discovery(std::chrono::seconds::zero(), 2, true); // Knows server1 and server2
}

/**
 * This test checks the addition of network interfaces at run-time.
 *
 * After launching the reader with the network interfaces enabled,
 * the writer is launched with the transport simulating that there
 * are no interfaces.
 * No participant discovery occurs, nor is communication established.
 *
 * In a second step, the flag to simulate no interfaces is disabled and
 * DomainParticipant::set_qos() called to add the "new" interfaces.
 * Discovery is succesful and communication is established.
 */
TEST(DDSDiscovery, DDSNetworkInterfaceChangesAtRunTime)
{
    using namespace eprosima::fastdds::rtps;

    PubSubWriter<HelloWorldPubSubType> datawriter(TEST_TOPIC_NAME);
    PubSubReader<HelloWorldPubSubType> datareader(TEST_TOPIC_NAME);

    // datareader is initialized with all the network interfaces
    datareader.durability_kind(eprosima::fastrtps::TRANSIENT_LOCAL_DURABILITY_QOS).history_depth(100).
            reliability(eprosima::fastrtps::RELIABLE_RELIABILITY_QOS).init();
    ASSERT_TRUE(datareader.isInitialized());

    // datawriter: launch without interfaces
    test_UDPv4Transport::simulate_no_interfaces = true;
    auto test_transport = std::make_shared<test_UDPv4TransportDescriptor>();
    datawriter.disable_builtin_transport().add_user_transport_to_pparams(test_transport).history_depth(100).init();
    ASSERT_TRUE(datawriter.isInitialized());

    // no discovery
    datawriter.wait_discovery(std::chrono::seconds(3));
    datareader.wait_discovery(std::chrono::seconds(3));
    EXPECT_EQ(datawriter.get_matched(), 0u);
    EXPECT_EQ(datareader.get_matched(), 0u);

    // send data
    auto complete_data = default_helloworld_data_generator();
    size_t samples = complete_data.size();

    datareader.startReception(complete_data);

    datawriter.send(complete_data);
    EXPECT_TRUE(complete_data.empty());

    // no data received
    EXPECT_EQ(datareader.block_for_all(std::chrono::seconds(3)), 0u);

    // enable interfaces
    test_UDPv4Transport::simulate_no_interfaces = false;
    datawriter.participant_set_qos();

    // Wait for discovery
    datawriter.wait_discovery(std::chrono::seconds(3));
    datareader.wait_discovery(std::chrono::seconds(3));
    ASSERT_EQ(datawriter.get_matched(), 1u);
    ASSERT_EQ(datareader.get_matched(), 1u);

    // data received
    EXPECT_EQ(datareader.block_for_all(std::chrono::seconds(3)), samples);

    datareader.destroy();
    datawriter.destroy();
}

/*
 * This tests checks that DataReader::get_subscription_matched_status() and
 * DataWriter::get_publication_matched_status() return the correct last_publication_handle
 * and last_subscription_handle respectively; that is, the handle to the last DataWriter/DataReader which
 * discovery/un-discovery triggered a change in the DataReader/DataWriter MatchedStatus.
 *
 * It does so by creating two pairs of DataReader-DataWriter at different times, waiting for matching/unmatching
 * and check the last_publication_handle and last_subscription_handle respectively:
 *
 * 1. Create a DR and DW in the same partition and wait for discovery
 * 2. Check that the last_*_handle is the correct one
 * 3. Create another DR and DW in the same partition and wait for discovery
 * 4. Check that old DR and DW report the new DW and DR as last_*_handle
 * 5. Change old DW to a different partition and wait for undiscovery
 * 6. Check that both DR report the old DW as last_publication_handle
 * 7. Change old DR to a partition different than the other two and wait for undiscovery
 * 8. Check that old DR and new DW report each other as last_*_handle
 */
TEST(DDSDiscovery, UpdateMatchedStatus)
{
    /* Create DataReaders and DataWriters */
    PubSubWriter<HelloWorldPubSubType> datawriter_1(TEST_TOPIC_NAME);
    PubSubReader<HelloWorldPubSubType> datareader_1(TEST_TOPIC_NAME);
    PubSubWriter<HelloWorldPubSubType> datawriter_2(TEST_TOPIC_NAME);
    PubSubReader<HelloWorldPubSubType> datareader_2(TEST_TOPIC_NAME);

    /* Init first pair of DataReader-DataWriter */
    datareader_1
            .partition("A")
            .init();
    ASSERT_TRUE(datareader_1.isInitialized());

    datawriter_1
            .partition("A")
            .init();
    ASSERT_TRUE(datawriter_1.isInitialized());

    /* Wait for discovery */
    datareader_1.wait_discovery(std::chrono::seconds(3), 1);
    datawriter_1.wait_discovery(1, std::chrono::seconds(3));
    ASSERT_EQ(datareader_1.get_matched(), 1u);
    ASSERT_EQ(datawriter_1.get_matched(), 1u);

    /* Check that the reported last_*_handle are correct */
    ASSERT_EQ(datareader_1.get_subscription_matched_status().last_publication_handle,
            datawriter_1.datawriter_ihandle());
    ASSERT_EQ(datawriter_1.get_publication_matched_status().last_subscription_handle,
            datareader_1.datareader_ihandle());

    /* Init second pair of DataReader-DataWriter */
    datareader_2
            .partition("A")
            .init();
    ASSERT_TRUE(datareader_2.isInitialized());

    datawriter_2
            .partition("A")
            .init();
    ASSERT_TRUE(datawriter_2.isInitialized());

    /*
     * Wait for discovery:
     *   - DR_1: DW_1, DW_2
     *   - DR_2: DW_1, DW_2
     *   - DW_1: DR_1, DR_2
     *   - DW_2: DR_1, DR_2
     */
    datareader_1.wait_discovery(std::chrono::seconds(3), 2);
    datawriter_1.wait_discovery(2, std::chrono::seconds(3));
    datareader_2.wait_discovery(std::chrono::seconds(3), 2);
    datawriter_2.wait_discovery(2, std::chrono::seconds(3));

    ASSERT_EQ(datareader_1.get_matched(), 2u);
    ASSERT_EQ(datawriter_1.get_matched(), 2u);
    ASSERT_EQ(datareader_2.get_matched(), 2u);
    ASSERT_EQ(datawriter_2.get_matched(), 2u);

    /* Check that the reported last_*_handle are correct */
    ASSERT_EQ(datareader_1.get_subscription_matched_status().last_publication_handle,
            datawriter_2.datawriter_ihandle());
    ASSERT_EQ(datawriter_1.get_publication_matched_status().last_subscription_handle,
            datareader_2.datareader_ihandle());

    /*
     * Change DW_1's partition and wait for undiscovery:
     *   - DR_1: DW_2
     *   - DR_2: DW_2
     *   - DW_1:
     *   - DW_2: DR_1, DR_2
     */
    datawriter_1.update_partition("B");
    datawriter_1.wait_reader_undiscovery(0);
    datareader_1.wait_writer_undiscovery(1);
    datareader_2.wait_writer_undiscovery(1);

    /* Check that the reported last_*_handle are correct */
    ASSERT_EQ(datareader_1.get_subscription_matched_status().last_publication_handle,
            datawriter_1.datawriter_ihandle());
    ASSERT_EQ(datareader_2.get_subscription_matched_status().last_publication_handle,
            datawriter_1.datawriter_ihandle());

    /*
     * Change DR_1 partition and wait for undiscovery:
     *   - DR_1:
     *   - DR_2: DW_2
     *   - DW_1:
     *   - DW_2: DR_2
     */
    datareader_1.update_partition("C");
    datareader_1.wait_writer_undiscovery(0);
    datawriter_2.wait_reader_undiscovery(1);

    /* Check that the reported last_*_handle are correct */
    ASSERT_EQ(datareader_1.get_subscription_matched_status().last_publication_handle,
            datawriter_2.datawriter_ihandle());
    ASSERT_EQ(datawriter_2.get_publication_matched_status().last_subscription_handle,
            datareader_1.datareader_ihandle());

    /* Clean up entities */
    datareader_1.destroy();
    datareader_2.destroy();
    datawriter_1.destroy();
    datawriter_2.destroy();
}

/**
 * This test checks that the physical properties are correctly sent on the DATA[p], and that the
 * ParticipantProxyData on the receiver side has the correct values.
 *
 * This is done by creating two different participants, one of them overloading
 * DomainParticipantListener::on_participant_discovery(). This callback is used to check the
 * ParticipantProxyData from the other participant, asserting that the received physical information
 * is the same as the one in the sending participant.
 *
 * Additionally, it checks that whenever the properties are not in the QoS, they are not in the
 * received ParticipantProxyData either.
 *
 */
TEST(DDSDiscovery, ParticipantProxyPhysicalData)
{
    using namespace eprosima::fastdds::dds;
    using namespace eprosima::fastrtps::rtps;

    class CustomDomainParticipantListener : public DomainParticipantListener
    {
    public:

        CustomDomainParticipantListener(
                std::condition_variable* cv,
                std::mutex* mtx,
                std::atomic<bool>* found)
            : remote_participant_info(nullptr)
            , cv_(cv)
            , mtx_(mtx)
            , found_(found)
        {
        }

        ~CustomDomainParticipantListener()
        {
            if (nullptr != remote_participant_info)
            {
                delete remote_participant_info;
            }
            remote_participant_info = nullptr;
            cv_ = nullptr;
            mtx_ = nullptr;
            found_ = nullptr;
        }

        void on_participant_discovery(
                DomainParticipant* participant,
                ParticipantDiscoveryInfo&& info)
        {
            std::unique_lock<std::mutex> lck(*mtx_);
            if (info.status ==
                    eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERY_STATUS::DISCOVERED_PARTICIPANT)
            {
                static_cast<void>(participant);
                if (nullptr != remote_participant_info)
                {
                    delete remote_participant_info;
                }
                remote_participant_info = new ParticipantProxyData(info.info);
                found_->store(true);
                cv_->notify_one();
            }
        }

        ParticipantProxyData* remote_participant_info;

    private:

        std::condition_variable* cv_;

        std::mutex* mtx_;

        std::atomic<bool>* found_;
    };

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int domain_id = std::rand() % 100;

    std::vector<std::string> physical_property_names =
    {
        parameter_policy_physical_data_host,
        parameter_policy_physical_data_user,
        parameter_policy_physical_data_process
    };

    DomainParticipantQos qos;
#ifndef FASTDDS_STATISTICS
    // Make sure the physical properties are there even when there are no statistics
    for (auto physical_property_name : physical_property_names)
    {
        qos.properties().properties().emplace_back(physical_property_name, "");
    }
#endif // ifndef FASTDDS_STATISTICS

    std::atomic<bool> participant_found = {false};
    std::condition_variable cv;
    std::mutex listener_mutex;
    CustomDomainParticipantListener listener(&cv, &listener_mutex, &participant_found);

    /* Positive case, i.e. the properties are in the QoS and thus in the ParticipantProxyData */
    DomainParticipant* part_1 = DomainParticipantFactory::get_instance()->create_participant(domain_id, qos);
    DomainParticipant* part_2 = DomainParticipantFactory::get_instance()->create_participant(domain_id, qos, &listener);

    // This loops runs until part_2 receives a on_participant_discovery holding information about part_1
    while (true)
    {
        // Wait until some participant is found
        std::unique_lock<std::mutex> lck(listener_mutex);
        cv.wait(lck, [&]
                {
                    return participant_found.load() == true;
                });
        // Reset participant found flag
        participant_found.store(false);

        // Prevent assertion on spurious discovery of a participant from elsewhere
        if (part_1->guid() == listener.remote_participant_info->m_guid)
        {
            // Check that all three properties are present in the ParticipantProxyData, and that their value
            // is that of the property in part_1 (the original property value)
            for (auto physical_property_name : physical_property_names)
            {
                // Find property in ParticipantProxyData
                auto received_property = std::find_if(
                    listener.remote_participant_info->m_properties.begin(),
                    listener.remote_participant_info->m_properties.end(),
                    [&](const ParameterProperty_t& property)
                    {
                        return property.first() == physical_property_name;
                    });
                ASSERT_NE(received_property, listener.remote_participant_info->m_properties.end());

                // Find property in first participant
                auto part_1_property = PropertyPolicyHelper::find_property(
                    part_1->get_qos().properties(), physical_property_name);
                ASSERT_NE(nullptr, part_1_property);

                // Check that the property in the first participant has the same value as the one in
                // the ParticipantProxyData representing the first participant in the second one
                ASSERT_EQ(received_property->second(), *part_1_property);
            }
            break;
        }
    }

    DomainParticipantFactory::get_instance()->delete_participant(part_1);
    DomainParticipantFactory::get_instance()->delete_participant(part_2);

    /* Negative case, i.e. the properties are in not the QoS and thus not in the ParticipantProxyData */
    // Remove properties from QoS
    qos.properties().properties().clear();
    part_1 = DomainParticipantFactory::get_instance()->create_participant(domain_id, qos);
    part_2 = DomainParticipantFactory::get_instance()->create_participant(domain_id, qos, &listener);

    // Check that first participant does not have the properties
    for (auto physical_property_name : physical_property_names)
    {
        // Find property in first participant
        auto part_1_property = PropertyPolicyHelper::find_property(
            part_1->get_qos().properties(), physical_property_name);
        ASSERT_EQ(nullptr, part_1_property);
    }

    // This loops runs until part_2 receives a on_participant_discovery holding information about part_1
    while (true)
    {
        // Wait until some participant is found
        std::unique_lock<std::mutex> lck(listener_mutex);
        cv.wait(lck, [&]
                {
                    return participant_found.load() == true;
                });
        // Reset participant found flag
        participant_found.store(false);

        // Prevent assertion on spurious discovery of a participant from elsewhere
        if (part_1->guid() == listener.remote_participant_info->m_guid)
        {
            // Check that none of the three properties are present in the ParticipantProxyData.
            for (auto physical_property_name : physical_property_names)
            {
                // Look for property in ParticipantProxyData
                auto received_property = std::find_if(
                    listener.remote_participant_info->m_properties.begin(),
                    listener.remote_participant_info->m_properties.end(),
                    [&](const ParameterProperty_t& property)
                    {
                        return property.first() == physical_property_name;
                    });
                ASSERT_EQ(received_property, listener.remote_participant_info->m_properties.end());
            }
            break;
        }
    }

    DomainParticipantFactory::get_instance()->delete_participant(part_1);
    DomainParticipantFactory::get_instance()->delete_participant(part_2);
}

// Regression test for redmine issue 20409
TEST(DDSDiscovery, DataracePDP)
{
    using namespace eprosima;
    using namespace eprosima::fastdds::dds;
    using namespace eprosima::fastdds::rtps;

    class CustomDomainParticipantListener : public DomainParticipantListener
    {
    public:

        CustomDomainParticipantListener()
            : DomainParticipantListener()
            , discovery_future(discovery_promise.get_future())
            , destruction_future(destruction_promise.get_future())
            , undiscovery_future(undiscovery_promise.get_future())
        {
        }

        void on_participant_discovery(
                DomainParticipant* /*participant*/,
                eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info) override
        {
            if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERED_PARTICIPANT)
            {
                try
                {
                    discovery_promise.set_value();
                }
                catch (std::future_error&)
                {
                    // do nothing
                }
                destruction_future.wait();
            }
            else if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::REMOVED_PARTICIPANT ||
                    info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DROPPED_PARTICIPANT)
            {
                try
                {
                    undiscovery_promise.set_value();
                }
                catch (std::future_error&)
                {
                    // do nothing
                }
            }
        }

        std::promise<void> discovery_promise;
        std::future<void> discovery_future;

        std::promise<void> destruction_promise;
        std::future<void> destruction_future;

        std::promise<void> undiscovery_promise;
        std::future<void> undiscovery_future;
    };

    // Disable intraprocess
    auto settings = fastrtps::xmlparser::XMLProfileManager::library_settings();
    auto prev_intraprocess_delivery = settings.intraprocess_delivery;
    settings.intraprocess_delivery = fastrtps::INTRAPROCESS_OFF;
    fastrtps::xmlparser::XMLProfileManager::library_settings(settings);

    // DDS Domain Id
    const unsigned int DOMAIN_ID = (uint32_t)GET_PID() % 230;

    // This is a non deterministic test, so we will run it several times to increase probability of data race detection
    // if it exists.
    const unsigned int N_ITER = 10;
    unsigned int iter_idx = 0;
    while (iter_idx < N_ITER)
    {
        iter_idx++;

        DomainParticipantQos qos;
        qos.transport().use_builtin_transports = false;
        auto udp_transport = std::make_shared<UDPv4TransportDescriptor>();
        qos.transport().user_transports.push_back(udp_transport);

        // Create discoverer participant (the one where a data race on PDP might occur)
        CustomDomainParticipantListener participant_listener;
        DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(DOMAIN_ID, qos,
                        &participant_listener);

        DomainParticipantQos aux_qos;
        aux_qos.transport().use_builtin_transports = false;
        auto aux_udp_transport = std::make_shared<test_UDPv4TransportDescriptor>();
        aux_qos.transport().user_transports.push_back(aux_udp_transport);

        // Create auxiliary participant to be discovered
        aux_qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(1, 0);
        aux_qos.wire_protocol().builtin.discovery_config.leaseDuration = Duration_t(1, 10);
        DomainParticipant* aux_participant = DomainParticipantFactory::get_instance()->create_participant(DOMAIN_ID,
                        aux_qos);

        // Wait for discovery
        participant_listener.discovery_future.wait();

        // Shutdown auxiliary participant's network, so it will be removed after lease duration
        test_UDPv4Transport::test_UDPv4Transport_ShutdownAllNetwork = true;
        DomainParticipantFactory::get_instance()->delete_participant(aux_participant);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Wait for longer than lease duration

        try
        {
            // NOTE: at this point, the discoverer participant is stuck in a UDP discovery thread (unicast or multicast).
            // At the same time, the events thread is stuck at PDP::remove_remote_participant (lease duration expired
            // and so the discovered participant is removed), trying to acquire the callback mutex taken by the
            // discovery thread.

            // If we now signal the discovery thread to continue, a data race might occur if the received
            // ParticipantProxyData, which is further being processed in the discovery thread (assignRemoteEndpoints),
            // gets deleted/cleared by the events thread at the same time.
            // Note that a similar situation might arise in other scenarios, such as on the concurrent reception of a
            // data P and data uP each on a different thread (unicast and multicast), however these are harder to
            // reproduce in a regression test.
            participant_listener.destruction_promise.set_value();
        }
        catch (std::future_error&)
        {
            // do nothing
        }

        participant_listener.undiscovery_future.wait();
        DomainParticipantFactory::get_instance()->delete_participant(participant);
    }

    // Reestablish previous intraprocess configuration
    settings.intraprocess_delivery = prev_intraprocess_delivery;
    fastrtps::xmlparser::XMLProfileManager::library_settings(settings);
}
