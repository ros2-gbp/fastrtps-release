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

/*!
 * @file TCPControlMessage.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _FASTDDS_TCP_CONTROL_MESSAGE_H_
#define _FASTDDS_TCP_CONTROL_MESSAGE_H_

#include <fastdds/rtps/common/Locator.h>
#include <fastdds/rtps/common/SerializedPayload.h>

namespace eprosima {
namespace fastcdr {
class Cdr;
} // namespace fastcdr
} // namespace eprosima


namespace eprosima {
namespace fastdds {
namespace rtps {

/*!
 * @brief This class represents the enumeration TCPCommonKind defined by the user in the IDL file.
 * @ingroup TCP_IDL.1
 */
enum TCPCommonKind : uint32_t
{
    BIND_CONNECTION,
    OPEN_LOGICAL_PORT,
    CHECK_LOGICAL_PORT,
    KEEP_ALIVE,
    UNBIND_CONNECTION,
    LOGICAL_PORT_IS_CLOSED
};
/*!
 * @brief This class represents the structure ConnectionRequest_t defined by the user in the IDL file.
 * @ingroup TCP_IDL.1
 */
class ConnectionRequest_t
{
public:

    /*!
     * @brief Default constructor.
     */
    ConnectionRequest_t();

    /*!
     * @brief Default destructor.
     */
    ~ConnectionRequest_t();

    /*!
     * @brief This function sets a value in member protocolVersion
     * @param _protocolVersion New value for member protocolVersion
     */
    inline void protocolVersion(
            const fastrtps::rtps::ProtocolVersion_t& _protocolVersion)
    {
        m_protocolVersion = _protocolVersion;
    }

    /*!
     * @brief This function returns the value of member protocolVersion
     * @return Value of member protocolVersion
     */
    inline fastrtps::rtps::ProtocolVersion_t protocolVersion() const
    {
        return m_protocolVersion;
    }

    /*!
     * @brief This function returns a reference to member protocolVersion
     * @return Reference to member protocolVersion
     */
    inline fastrtps::rtps::ProtocolVersion_t& protocolVersion()
    {
        return m_protocolVersion;
    }

    /*!
     * @brief This function sets a value in member vendorId
     * @param _vendorId New value for member vendorId
     */
    inline void vendorId(
            const fastrtps::rtps::VendorId_t& _vendorId)
    {
        m_vendorId = _vendorId;
    }

    /*!
     * @brief This function returns the value of member vendorId
     * @return Value of member vendorId
     */
    inline fastrtps::rtps::VendorId_t vendorId() const
    {
        return m_vendorId;
    }

    /*!
     * @brief This function returns a reference to member vendorId
     * @return Reference to member vendorId
     */
    inline fastrtps::rtps::VendorId_t& vendorId()
    {
        return m_vendorId;
    }

    /*!
     * @brief This function sets a value in member transportLocator
     * @param _transportLocator New value for member transportLocator
     */
    inline void transportLocator(
            const Locator& _transportLocator)
    {
        m_transportLocator = _transportLocator;
    }

    /*!
     * @brief This function returns the value of member transportLocator
     * @return Value of member transportLocator
     */
    inline Locator transportLocator() const
    {
        return m_transportLocator;
    }

    /*!
     * @brief This function returns the maximum serialized size of an object
     * depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    static size_t getBufferCdrSerializedSize(
            const ConnectionRequest_t& data,
            size_t current_alignment = 0);

    /*!
     * @brief This function returns the serialized size of a data depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Serialized size.
     */
    static size_t getCdrSerializedSize(
            const ConnectionRequest_t& data,
            size_t current_alignment = 0);


    /*!
     * @brief This function serializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void serialize(
            eprosima::fastcdr::Cdr& cdr) const;

    /*!
     * @brief This function deserializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void deserialize(
            eprosima::fastcdr::Cdr& cdr);

    bool serialize(
            fastrtps::rtps::SerializedPayload_t* payload);
    bool deserialize(
            fastrtps::rtps::SerializedPayload_t* payload);

private:

    fastrtps::rtps::ProtocolVersion_t m_protocolVersion;
    fastrtps::rtps::VendorId_t m_vendorId;
    Locator m_transportLocator;
};
/*!
 * @brief This class represents the structure OpenLogicalPortRequest_t defined by the user in the IDL file.
 * @ingroup TCP_IDL.1
 */
class OpenLogicalPortRequest_t
{
public:

    /*!
     * @brief Default constructor.
     */
    OpenLogicalPortRequest_t();

    /*!
     * @brief Default destructor.
     */
    ~OpenLogicalPortRequest_t();

    /*!
     * @brief This function sets a value in member logicalPort
     * @param _logicalPort New value for member logicalPort
     */
    inline void logicalPort(
            uint16_t _logicalPort)
    {
        m_logicalPort = _logicalPort;
    }

    /*!
     * @brief This function returns the value of member logicalPort
     * @return Value of member logicalPort
     */
    inline uint16_t logicalPort() const
    {
        return m_logicalPort;
    }

    /*!
     * @brief This function returns the maximum serialized size of an object
     * depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    static size_t getBufferCdrSerializedSize(
            const OpenLogicalPortRequest_t& data,
            size_t current_alignment = 0);

    /*!
     * @brief This function returns the serialized size of a data depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Serialized size.
     */
    static size_t getCdrSerializedSize(
            const OpenLogicalPortRequest_t& data,
            size_t current_alignment = 0);


    /*!
     * @brief This function serializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void serialize(
            eprosima::fastcdr::Cdr& cdr) const;

    /*!
     * @brief This function deserializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void deserialize(
            eprosima::fastcdr::Cdr& cdr);

    bool serialize(
            fastrtps::rtps::SerializedPayload_t* payload);
    bool deserialize(
            fastrtps::rtps::SerializedPayload_t* payload);

private:

    uint16_t m_logicalPort;
};
/*!
 * @brief This class represents the structure CheckLogicalPortsRequest_t defined by the user in the IDL file.
 * @ingroup TCP_IDL.1
 */
class CheckLogicalPortsRequest_t
{
public:

    /*!
     * @brief Default constructor.
     */
    CheckLogicalPortsRequest_t();

    /*!
     * @brief Default destructor.
     */
    ~CheckLogicalPortsRequest_t();

    /*!
     * @brief This function copies the value in member logicalPortsRange
     * @param _logicalPortsRange New value to be copied in member logicalPortsRange
     */
    inline void logicalPortsRange(
            const std::vector<uint16_t>& _logicalPortsRange)
    {
        m_logicalPortsRange = _logicalPortsRange;
    }

    /*!
     * @brief This function moves the value in member logicalPortsRange
     * @param _logicalPortsRange New value to be moved in member logicalPortsRange
     */
    inline void logicalPortsRange(
            std::vector<uint16_t>&& _logicalPortsRange)
    {
        m_logicalPortsRange = std::move(_logicalPortsRange);
    }

    /*!
     * @brief This function returns a constant reference to member logicalPortsRange
     * @return Constant reference to member logicalPortsRange
     */
    inline const std::vector<uint16_t>& logicalPortsRange() const
    {
        return m_logicalPortsRange;
    }

    /*!
     * @brief This function returns a reference to member logicalPortsRange
     * @return Reference to member logicalPortsRange
     */
    inline std::vector<uint16_t>& logicalPortsRange()
    {
        return m_logicalPortsRange;
    }

    /*!
     * @brief This function returns the maximum serialized size of an object
     * depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    static size_t getBufferCdrSerializedSize(
            const CheckLogicalPortsRequest_t& data,
            size_t current_alignment = 0);

    /*!
     * @brief This function returns the serialized size of a data depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Serialized size.
     */
    static size_t getCdrSerializedSize(
            const CheckLogicalPortsRequest_t& data,
            size_t current_alignment = 0);


    /*!
     * @brief This function serializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void serialize(
            eprosima::fastcdr::Cdr& cdr) const;

    /*!
     * @brief This function deserializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void deserialize(
            eprosima::fastcdr::Cdr& cdr);

    bool serialize(
            fastrtps::rtps::SerializedPayload_t* payload);
    bool deserialize(
            fastrtps::rtps::SerializedPayload_t* payload);

private:

    std::vector<uint16_t> m_logicalPortsRange;
};
/*!
 * @brief This class represents the structure KeepAliveRequest_t defined by the user in the IDL file.
 * @ingroup TCP_IDL.1
 */
class KeepAliveRequest_t
{
public:

    /*!
     * @brief Default constructor.
     */
    KeepAliveRequest_t();

    /*!
     * @brief Default destructor.
     */
    ~KeepAliveRequest_t();

    /*!
     * @brief This function sets a value in member locator
     * @param _locator New value for member locator
     */
    inline void locator(
            const Locator& _locator)
    {
        m_locator = _locator;
    }

    /*!
     * @brief This function returns the value of member locator
     * @return Value of member locator
     */
    inline Locator locator() const
    {
        return m_locator;
    }

    /*!
     * @brief This function returns the maximum serialized size of an object
     * depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    static size_t getBufferCdrSerializedSize(
            const KeepAliveRequest_t& data,
            size_t current_alignment = 0);

    /*!
     * @brief This function returns the serialized size of a data depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Serialized size.
     */
    static size_t getCdrSerializedSize(
            const KeepAliveRequest_t& data,
            size_t current_alignment = 0);


    /*!
     * @brief This function serializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void serialize(
            eprosima::fastcdr::Cdr& cdr) const;

    /*!
     * @brief This function deserializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void deserialize(
            eprosima::fastcdr::Cdr& cdr);

    bool serialize(
            fastrtps::rtps::SerializedPayload_t* payload);
    bool deserialize(
            fastrtps::rtps::SerializedPayload_t* payload);

private:

    Locator m_locator;
};

/*!
 * @brief This class represents the structure LogicalPortIsClosedRequest_t defined by the user in the IDL file.
 * @ingroup TCP_IDL.1
 */
class LogicalPortIsClosedRequest_t
{
public:

    /*!
     * @brief Default constructor.
     */
    LogicalPortIsClosedRequest_t();

    /*!
     * @brief Default destructor.
     */
    ~LogicalPortIsClosedRequest_t();

    /*!
     * @brief This function sets a value in member logicalPort
     * @param _logicalPort New value for member logicalPort
     */
    inline void logicalPort(
            uint16_t _logicalPort)
    {
        m_logicalPort = _logicalPort;
    }

    /*!
     * @brief This function returns the value of member logicalPort
     * @return Value of member logicalPort
     */
    inline uint16_t logicalPort() const
    {
        return m_logicalPort;
    }

    /*!
     * @brief This function returns the maximum serialized size of an object
     * depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    static size_t getBufferCdrSerializedSize(
            const LogicalPortIsClosedRequest_t& data,
            size_t current_alignment = 0);

    /*!
     * @brief This function returns the serialized size of a data depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Serialized size.
     */
    static size_t getCdrSerializedSize(
            const LogicalPortIsClosedRequest_t& data,
            size_t current_alignment = 0);


    /*!
     * @brief This function serializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void serialize(
            eprosima::fastcdr::Cdr& cdr) const;

    /*!
     * @brief This function deserializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void deserialize(
            eprosima::fastcdr::Cdr& cdr);

    bool serialize(
            fastrtps::rtps::SerializedPayload_t* payload);
    bool deserialize(
            fastrtps::rtps::SerializedPayload_t* payload);

private:

    uint16_t m_logicalPort;
};

/*!
 * @brief This class represents the enumeration ResponseCode defined by the user in the IDL file.
 * @ingroup TCP_IDL.1
 */
enum ResponseCode : uint32_t
{
    RETCODE_VOID = (uint32_t)(-1),
    RETCODE_OK = 0,
    RETCODE_BAD_REQUEST,
    RETCODE_EXISTING_CONNECTION,
    RETCODE_INVALID_PORT,
    RETCODE_UNKNOWN_LOCATOR,
    RETCODE_INCOMPATIBLE_VERSION,
    RETCODE_SERVER_ERROR
};
/*!
 * @brief This class represents the structure BindConnectionResponse_t defined by the user in the IDL file.
 * @ingroup TCP_IDL.1
 */
class BindConnectionResponse_t
{
public:

    /*!
     * @brief Default constructor.
     */
    BindConnectionResponse_t();

    /*!
     * @brief Default destructor.
     */
    ~BindConnectionResponse_t();

    /*!
     * @brief This function sets a value in member locator
     * @param _locator New value for member locator
     */
    inline void locator(
            const Locator& _locator)
    {
        m_locator = _locator;
    }

    /*!
     * @brief This function returns the value of member locator
     * @return Value of member locator
     */
    inline Locator locator() const
    {
        return m_locator;
    }

    /*!
     * @brief This function returns the maximum serialized size of an object
     * depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    static size_t getBufferCdrSerializedSize(
            const BindConnectionResponse_t& data,
            size_t current_alignment = 0);

    /*!
     * @brief This function returns the serialized size of a data depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Serialized size.
     */
    static size_t getCdrSerializedSize(
            const BindConnectionResponse_t& data,
            size_t current_alignment = 0);


    /*!
     * @brief This function serializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void serialize(
            eprosima::fastcdr::Cdr& cdr) const;

    /*!
     * @brief This function deserializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void deserialize(
            eprosima::fastcdr::Cdr& cdr);

    bool serialize(
            fastrtps::rtps::SerializedPayload_t* payload);
    bool deserialize(
            fastrtps::rtps::SerializedPayload_t* payload);

private:

    Locator m_locator;
};
/*!
 * @brief This class represents the structure CheckLogicalPortsResponse_t defined by the user in the IDL file.
 * @ingroup TCP_IDL.1
 */
class CheckLogicalPortsResponse_t
{
public:

    /*!
     * @brief Default constructor.
     */
    CheckLogicalPortsResponse_t();

    /*!
     * @brief Default destructor.
     */
    ~CheckLogicalPortsResponse_t();

    /*!
     * @brief This function copies the value in member availableLogicalPorts
     * @param _availableLogicalPorts New value to be copied in member availableLogicalPorts
     */
    inline void availableLogicalPorts(
            const std::vector<uint16_t>& _availableLogicalPorts)
    {
        m_availableLogicalPorts = _availableLogicalPorts;
    }

    /*!
     * @brief This function moves the value in member availableLogicalPorts
     * @param _availableLogicalPorts New value to be moved in member availableLogicalPorts
     */
    inline void availableLogicalPorts(
            std::vector<uint16_t>&& _availableLogicalPorts)
    {
        m_availableLogicalPorts = std::move(_availableLogicalPorts);
    }

    /*!
     * @brief This function returns a constant reference to member availableLogicalPorts
     * @return Constant reference to member availableLogicalPorts
     */
    inline const std::vector<uint16_t>& availableLogicalPorts() const
    {
        return m_availableLogicalPorts;
    }

    /*!
     * @brief This function returns a reference to member availableLogicalPorts
     * @return Reference to member availableLogicalPorts
     */
    inline std::vector<uint16_t>& availableLogicalPorts()
    {
        return m_availableLogicalPorts;
    }

    /*!
     * @brief This function returns the maximum serialized size of an object
     * depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    static size_t getBufferCdrSerializedSize(
            const CheckLogicalPortsResponse_t& data,
            size_t current_alignment = 0);

    /*!
     * @brief This function returns the serialized size of a data depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Serialized size.
     */
    static size_t getCdrSerializedSize(
            const CheckLogicalPortsResponse_t& data,
            size_t current_alignment = 0);


    /*!
     * @brief This function serializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void serialize(
            eprosima::fastcdr::Cdr& cdr) const;

    /*!
     * @brief This function deserializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    void deserialize(
            eprosima::fastcdr::Cdr& cdr);

    bool serialize(
            fastrtps::rtps::SerializedPayload_t* payload);
    bool deserialize(
            fastrtps::rtps::SerializedPayload_t* payload);

private:

    std::vector<uint16_t> m_availableLogicalPorts;
};

} // namespace rtps
} // namespace fastdds
} // Namespaces

#endif // _FASTDDS_TCP_CONTROL_MESSAGE_H_
