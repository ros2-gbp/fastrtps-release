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

#include "BlackboxTests.hpp"
#include "TCPReqRepHelloWorldRequester.hpp"
#include "TCPReqRepHelloWorldReplier.hpp"

#include <gtest/gtest.h>

#include <fastrtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/transport/TCPv6TransportDescriptor.h>

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

#if TLS_FOUND
static const char* certs_path = nullptr;
#endif // if TLS_FOUND

enum communication_type
{
    TRANSPORT
};

class TransportTCP : public testing::TestWithParam<std::tuple<communication_type, bool>>
{
public:

    void SetUp() override
    {
        test_transport_.reset();
        use_ipv6 = std::get<1>(GetParam());
        if (use_ipv6)
        {
            test_transport_ = std::make_shared<TCPv6TransportDescriptor>();
        }
        else
        {
            test_transport_ = std::make_shared<TCPv4TransportDescriptor>();
        }
    }

    void TearDown() override
    {
        use_ipv6 = false;
    }

    std::shared_ptr<TCPTransportDescriptor> test_transport_;
};

// TCP and Domain management with logical ports tests
TEST_P(TransportTCP, TCPDomainHelloWorld_P0_P1_D0_D0)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;
    const uint16_t nmsgs = 5;

    requester.init(0, 0, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(1, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery.
    requester.wait_discovery();
    replier.wait_discovery();

    ASSERT_TRUE(requester.is_matched());
    ASSERT_TRUE(replier.is_matched());

    for (uint16_t count = 0; count < nmsgs; ++count)
    {
        requester.send(count);
        requester.block();
    }
}

TEST_P(TransportTCP, TCPDomainHelloWorld_P0_P1_D0_D1)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(0, 0, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(1, 1, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery. They must not discover each other.
    requester.wait_discovery(std::chrono::seconds(10));

    ASSERT_FALSE(requester.is_matched());
    ASSERT_FALSE(replier.is_matched());
}

TEST_P(TransportTCP, TCPDomainHelloWorld_P0_P1_D1_D0)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(0, 1, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(1, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery. They must not discover each other.
    requester.wait_discovery(std::chrono::seconds(10));

    ASSERT_FALSE(requester.is_matched());
    ASSERT_FALSE(replier.is_matched());

}

TEST_P(TransportTCP, TCPDomainHelloWorld_P0_P3_D0_D0)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;
    const uint16_t nmsgs = 5;

    requester.init(0, 0, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(3, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery.
    requester.wait_discovery();
    replier.wait_discovery();

    for (uint16_t count = 0; count < nmsgs; ++count)
    {
        requester.send(count);
        requester.block();
    }

}

TEST_P(TransportTCP, TCPDomainHelloWorld_P0_P3_D0_D1)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(0, 0, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(3, 1, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery. They must not discover each other.
    requester.wait_discovery(std::chrono::seconds(10));

    ASSERT_FALSE(requester.is_matched());
    ASSERT_FALSE(replier.is_matched());
}

TEST_P(TransportTCP, TCPDomainHelloWorld_P0_P3_D1_D0)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(0, 1, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(3, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery. They must not discover each other.
    requester.wait_discovery(std::chrono::seconds(10));

    ASSERT_FALSE(requester.is_matched());
    ASSERT_FALSE(replier.is_matched());

}

TEST_P(TransportTCP, TCPDomainHelloWorld_P3_P0_D0_D0)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;
    const uint16_t nmsgs = 5;

    requester.init(3, 0, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(0, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery.
    requester.wait_discovery();
    replier.wait_discovery();

    ASSERT_TRUE(requester.is_matched());
    ASSERT_TRUE(replier.is_matched());

    for (uint16_t count = 0; count < nmsgs; ++count)
    {
        requester.send(count);
        requester.block();
    }

}

TEST_P(TransportTCP, TCPDomainHelloWorld_P3_P0_D0_D1)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(3, 0, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(0, 1, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery. They must not discover each other.
    requester.wait_discovery(std::chrono::seconds(10));

    ASSERT_FALSE(requester.is_matched());
    ASSERT_FALSE(replier.is_matched());
}

TEST_P(TransportTCP, TCPDomainHelloWorld_P3_P0_D1_D0)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(3, 1, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(0, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery. They must not discover each other.
    requester.wait_discovery(std::chrono::seconds(10));

    ASSERT_FALSE(requester.is_matched());
    ASSERT_FALSE(replier.is_matched());

}

TEST_P(TransportTCP, TCPDomainHelloWorld_P2_P3_D0_D0)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;
    const uint16_t nmsgs = 5;

    requester.init(2, 0, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(3, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery.
    requester.wait_discovery();
    replier.wait_discovery();

    for (uint16_t count = 0; count < nmsgs; ++count)
    {
        requester.send(count);
        requester.block();
    }

}

TEST_P(TransportTCP, TCPDomainHelloWorld_P2_P3_D0_D1)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(2, 0, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(3, 1, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery. They must not discover each other.
    requester.wait_discovery(std::chrono::seconds(10));

    ASSERT_FALSE(requester.is_matched());
    ASSERT_FALSE(replier.is_matched());
}

TEST_P(TransportTCP, TCPDomainHelloWorld_P2_P3_D1_D0)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(2, 1, global_port);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(3, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery. They must not discover each other.
    requester.wait_discovery(std::chrono::seconds(10));

    ASSERT_FALSE(requester.is_matched());
    ASSERT_FALSE(replier.is_matched());
}

TEST_P(TransportTCP, TCPMaxInitialPeer_P0_4_P3)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(0, 0, global_port, 4);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(3, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery.
    requester.wait_discovery();
    replier.wait_discovery();

    ASSERT_TRUE(requester.is_matched());
    ASSERT_TRUE(replier.is_matched());
}

TEST_P(TransportTCP, TCPMaxInitialPeer_P0_4_P4)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(0, 0, global_port, 4);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(4, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery.
    requester.wait_discovery(std::chrono::seconds(10));

    ASSERT_FALSE(requester.is_matched());
    ASSERT_FALSE(replier.is_matched());
}

TEST_P(TransportTCP, TCPMaxInitialPeer_P0_5_P4)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(0, 0, global_port, 5);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(4, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery.
    requester.wait_discovery();
    replier.wait_discovery();

    ASSERT_TRUE(requester.is_matched());
    ASSERT_TRUE(replier.is_matched());
}

#if TLS_FOUND
TEST_P(TransportTCP, TCP_TLS)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;

    requester.init(0, 0, global_port, 5, certs_path);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(4, 0, global_port, 5, certs_path);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery.
    requester.wait_discovery();
    replier.wait_discovery();

    ASSERT_TRUE(requester.is_matched());
    ASSERT_TRUE(replier.is_matched());
}

void tls_init()
{
    certs_path = std::getenv("CERTS_PATH");

    if (certs_path == nullptr)
    {
        std::cout << "Cannot get enviroment variable CERTS_PATH" << std::endl;
        exit(-1);
    }
}

#endif // if TLS_FOUND

// Regression test for ShrinkLocators/transform_remote_locators mechanism.
TEST_P(TransportTCP, TCPLocalhost)
{
    TCPReqRepHelloWorldRequester requester;
    TCPReqRepHelloWorldReplier replier;
    const uint16_t nmsgs = 5;

    requester.init(0, 0, global_port, 0, nullptr, true);

    ASSERT_TRUE(requester.isInitialized());

    replier.init(1, 0, global_port);

    ASSERT_TRUE(replier.isInitialized());

    // Wait for discovery.
    requester.wait_discovery();
    replier.wait_discovery();

    ASSERT_TRUE(requester.is_matched());
    ASSERT_TRUE(replier.is_matched());

    for (uint16_t count = 0; count < nmsgs; ++count)
    {
        requester.send(count);
        requester.block();
    }
}

// Test for ==operator TCPTransportDescriptor is not required as it is an abstract class and in TCPv6 is same method
// Test for copy TCPTransportDescriptor is not required as it is an abstract class and in TCPv6 is same method

// Test == operator for TCPv4
TEST_P(TransportTCP, TCPv4_equal_operator)
{
    // TCPv4TransportDescriptor
    TCPv4TransportDescriptor tcpv4_transport_1;
    TCPv4TransportDescriptor tcpv4_transport_2;

    // Compare equal in defult values
    ASSERT_EQ(tcpv4_transport_1, tcpv4_transport_2);

    // Modify default values in 1
    tcpv4_transport_1.set_WAN_address("80.80.99.45");

    ASSERT_FALSE(tcpv4_transport_1 == tcpv4_transport_2); // operator== != operator!=, using operator== == false instead

    // Modify default values in 2
    tcpv4_transport_2.set_WAN_address("80.80.99.45");

    ASSERT_EQ(tcpv4_transport_1, tcpv4_transport_2);
}

// Test copy constructor and copy assignment for TCPv4
TEST_P(TransportTCP, TCPv4_copy)
{
    TCPv4TransportDescriptor tcpv4_transport;
    tcpv4_transport.set_WAN_address("80.80.99.45");

    // Copy constructor
    TCPv4TransportDescriptor tcpv4_transport_copy_constructor(tcpv4_transport);
    EXPECT_EQ(tcpv4_transport, tcpv4_transport_copy_constructor);

    // Copy assignment
    TCPv4TransportDescriptor tcpv4_transport_copy = tcpv4_transport;
    EXPECT_EQ(tcpv4_transport_copy, tcpv4_transport);
}

// Test == operator for TCPv6
TEST_P(TransportTCP, TCPv6_equal_operator)
{
    // TCPv6TransportDescriptor
    TCPv6TransportDescriptor tcpv6_transport_1;
    TCPv6TransportDescriptor tcpv6_transport_2;

    // Compare equal in defult values
    ASSERT_EQ(tcpv6_transport_1, tcpv6_transport_2);

    // Modify some default values in 1
    tcpv6_transport_1.enable_tcp_nodelay = !tcpv6_transport_1.enable_tcp_nodelay; // change default value
    tcpv6_transport_1.max_logical_port = tcpv6_transport_1.max_logical_port + 10; // change default value
    tcpv6_transport_1.add_listener_port(123u * 98u);

    ASSERT_FALSE(tcpv6_transport_1 == tcpv6_transport_2); // operator== != operator!=, using operator== == false instead


    // Modify some default values in 2
    tcpv6_transport_2.enable_tcp_nodelay = !tcpv6_transport_2.enable_tcp_nodelay; // change default value
    tcpv6_transport_2.max_logical_port = tcpv6_transport_2.max_logical_port + 10; // change default value
    tcpv6_transport_2.add_listener_port(123u * 98u);

    ASSERT_EQ(tcpv6_transport_1, tcpv6_transport_2);
}

// Test copy constructor and copy assignment for TCPv6
TEST_P(TransportTCP, TCPv6_copy)
{
    // Change some varibles in order to check the non default cretion
    TCPv6TransportDescriptor tcpv6_transport;
    tcpv6_transport.enable_tcp_nodelay = !tcpv6_transport.enable_tcp_nodelay; // change default value
    tcpv6_transport.max_logical_port = tcpv6_transport.max_logical_port + 10; // change default value
    tcpv6_transport.add_listener_port(123u * 98u);

    // Copy constructor
    TCPv6TransportDescriptor tcpv6_transport_copy_constructor(tcpv6_transport);
    EXPECT_EQ(tcpv6_transport, tcpv6_transport_copy_constructor);

    // Copy assignment
    TCPv6TransportDescriptor tcpv6_transport_copy = tcpv6_transport;
    EXPECT_EQ(tcpv6_transport_copy, tcpv6_transport);
}

// Test connection is successfully restablished after dropping and relaunching a TCP client (requester)
// Issue -> https://github.com/eProsima/Fast-DDS/issues/2409
TEST(TransportTCP, Client_reconnection)
{
    TCPReqRepHelloWorldReplier* replier;
    TCPReqRepHelloWorldRequester* requester;
    const uint16_t nmsgs = 5;

    replier = new TCPReqRepHelloWorldReplier;
    replier->init(1, 0, global_port);

    ASSERT_TRUE(replier->isInitialized());

    requester = new TCPReqRepHelloWorldRequester;
    requester->init(0, 0, global_port);

    ASSERT_TRUE(requester->isInitialized());

    // Wait for discovery.
    replier->wait_discovery();
    requester->wait_discovery();

    ASSERT_TRUE(replier->is_matched());
    ASSERT_TRUE(requester->is_matched());

    for (uint16_t count = 0; count < nmsgs; ++count)
    {
        requester->send(count);
        requester->block();
    }

    // Release TCP client resources.
    delete requester;

    // Wait until unmatched.
    replier->wait_unmatched();
    ASSERT_FALSE(replier->is_matched());

    // Create new TCP client instance.
    requester = new TCPReqRepHelloWorldRequester;
    requester->init(0, 0, global_port);

    ASSERT_TRUE(requester->isInitialized());

    // Wait for discovery.
    replier->wait_discovery();
    requester->wait_discovery();

    ASSERT_TRUE(replier->is_matched());
    ASSERT_TRUE(requester->is_matched());

    for (uint16_t count = 0; count < nmsgs; ++count)
    {
        requester->send(count);
        requester->block();
    }

    delete replier;
    delete requester;
}

#ifdef INSTANTIATE_TEST_SUITE_P
#define GTEST_INSTANTIATE_TEST_MACRO(x, y, z, w) INSTANTIATE_TEST_SUITE_P(x, y, z, w)
#else
#define GTEST_INSTANTIATE_TEST_MACRO(x, y, z, w) INSTANTIATE_TEST_CASE_P(x, y, z, w)
#endif // ifdef INSTANTIATE_TEST_SUITE_P

GTEST_INSTANTIATE_TEST_MACRO(TransportTCP,
        TransportTCP,
        testing::Combine(testing::Values(TRANSPORT), testing::Values(false, true)),
        [](const testing::TestParamInfo<TransportTCP::ParamType>& info)
        {
            bool ipv6 = std::get<1>(info.param);
            std::string suffix = ipv6 ? "TCPv6" : "TCPv4";
            switch (std::get<0>(info.param))
            {
                case TRANSPORT:
                default:
                    return "Transport" + suffix;
            }

        });
