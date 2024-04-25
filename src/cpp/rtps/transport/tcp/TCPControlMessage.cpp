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
 * @file tcp_idl.1.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace {
char dummy;
} // namespace
#endif // ifdef _WIN32

#include <fastdds/rtps/common/Types.h>
#include <rtps/transport/tcp/TCPControlMessage.h>

#include <fastcdr/FastCdr.h>
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

namespace eprosima {
namespace fastdds {
namespace rtps {

using ProtocolVersion_t = fastrtps::rtps::ProtocolVersion_t;
using VendorId_t = fastrtps::rtps::VendorId_t;
using SerializedPayload_t = fastrtps::rtps::SerializedPayload_t;

static void operator <<(
        eprosima::fastcdr::Cdr& scdr,
        const ProtocolVersion_t& protocolVersion)
{
    scdr << protocolVersion.m_major;
    scdr << protocolVersion.m_minor;
}

static void operator <<(
        eprosima::fastcdr::Cdr& scdr,
        const VendorId_t& vendorId)
{
    scdr << vendorId[0];
    scdr << vendorId[1];
}

static void operator <<(
        eprosima::fastcdr::Cdr& scdr,
        const Locator& locator)
{
    scdr << locator.kind;
    scdr << locator.port;
    for (int i = 0; i < 16; ++i)
    {
        scdr << locator.address[i];
    }
}

static void operator >>(
        eprosima::fastcdr::Cdr& scdr,
        ProtocolVersion_t& protocolVersion)
{
    scdr >> protocolVersion.m_major;
    scdr >> protocolVersion.m_minor;
}

static void operator >>(
        eprosima::fastcdr::Cdr& scdr,
        VendorId_t& vendorId)
{
    scdr >> vendorId[0];
    scdr >> vendorId[1];
}

static void operator >>(
        eprosima::fastcdr::Cdr& scdr,
        Locator& locator)
{
    scdr >> locator.kind;
    scdr >> locator.port;
    for (int i = 0; i < 16; ++i)
    {
        scdr >> locator.address[i];
    }
}

ConnectionRequest_t::ConnectionRequest_t()
    : m_vendorId(fastrtps::rtps::c_VendorId_eProsima)
{
}

ConnectionRequest_t::~ConnectionRequest_t()
{
}

size_t ConnectionRequest_t::getBufferCdrSerializedSize(
        const ConnectionRequest_t& data,
        size_t current_alignment)
{
    return getCdrSerializedSize(data, current_alignment) + 4;
}

size_t ConnectionRequest_t::getCdrSerializedSize(
        const ConnectionRequest_t& /*data*/,
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += 2 + eprosima::fastcdr::Cdr::alignment(current_alignment, 2);

    current_alignment += 2 + eprosima::fastcdr::Cdr::alignment(current_alignment, 2);

    current_alignment += 24 + eprosima::fastcdr::Cdr::alignment(current_alignment, 24);


    return current_alignment - initial_alignment;
}

void ConnectionRequest_t::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    scdr << m_protocolVersion;
    scdr << m_vendorId;
    scdr << m_transportLocator;
}

void ConnectionRequest_t::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    dcdr >> m_protocolVersion;
    dcdr >> m_vendorId;
    dcdr >> m_transportLocator;
}

bool ConnectionRequest_t::serialize(
        SerializedPayload_t* payload)
{
    ConnectionRequest_t* p_type = this;
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->max_size); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

    try
    {
        // Serialize encapsulation
        ser.serialize_encapsulation();
        p_type->serialize(ser); // Serialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    payload->length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    return true;
}

bool ConnectionRequest_t::deserialize(
        SerializedPayload_t* payload)
{
    ConnectionRequest_t* p_type = this;     //Convert DATA to pointer of your type
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->length + 4); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.

    try
    {
        // Deserialize encapsulation.
        deser.read_encapsulation();
        payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
        p_type->deserialize(deser); //Deserialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    return true;
}

OpenLogicalPortRequest_t::OpenLogicalPortRequest_t()
{
    m_logicalPort = 0;
}

OpenLogicalPortRequest_t::~OpenLogicalPortRequest_t()
{
}

size_t OpenLogicalPortRequest_t::getBufferCdrSerializedSize(
        const OpenLogicalPortRequest_t& data,
        size_t current_alignment)
{
    return getCdrSerializedSize(data, current_alignment) + 4;
}

size_t OpenLogicalPortRequest_t::getCdrSerializedSize(
        const OpenLogicalPortRequest_t& /*data*/,
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += 2 + eprosima::fastcdr::Cdr::alignment(current_alignment, 2);


    return current_alignment - initial_alignment;
}

void OpenLogicalPortRequest_t::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    scdr << m_logicalPort;
}

void OpenLogicalPortRequest_t::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    dcdr >> m_logicalPort;
}

bool OpenLogicalPortRequest_t::serialize(
        SerializedPayload_t* payload)
{
    OpenLogicalPortRequest_t* p_type = this;
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->max_size); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

    try
    {
        // Serialize encapsulation
        ser.serialize_encapsulation();
        p_type->serialize(ser); // Serialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    payload->length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    return true;
}

bool OpenLogicalPortRequest_t::deserialize(
        SerializedPayload_t* payload)
{
    OpenLogicalPortRequest_t* p_type = this;     //Convert DATA to pointer of your type
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->length + 4); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.

    try
    {
        // Deserialize encapsulation.
        deser.read_encapsulation();
        payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
        p_type->deserialize(deser); //Deserialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    return true;
}

CheckLogicalPortsRequest_t::CheckLogicalPortsRequest_t()
{
}

CheckLogicalPortsRequest_t::~CheckLogicalPortsRequest_t()
{
}

size_t CheckLogicalPortsRequest_t::getBufferCdrSerializedSize(
        const CheckLogicalPortsRequest_t& data,
        size_t current_alignment)
{
    return getCdrSerializedSize(data, current_alignment) + 4;
}

size_t CheckLogicalPortsRequest_t::getCdrSerializedSize(
        const CheckLogicalPortsRequest_t& data,
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);
    current_alignment += (data.logicalPortsRange().size() * 2) +
            eprosima::fastcdr::Cdr::alignment(current_alignment, 2);


    return current_alignment - initial_alignment;
}

void CheckLogicalPortsRequest_t::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    scdr << m_logicalPortsRange;
}

void CheckLogicalPortsRequest_t::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    dcdr >> m_logicalPortsRange;
}

bool CheckLogicalPortsRequest_t::serialize(
        SerializedPayload_t* payload)
{
    CheckLogicalPortsRequest_t* p_type = this;
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->max_size); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

    try
    {
        // Serialize encapsulation
        ser.serialize_encapsulation();
        p_type->serialize(ser); // Serialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    payload->length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    return true;
}

bool CheckLogicalPortsRequest_t::deserialize(
        SerializedPayload_t* payload)
{
    CheckLogicalPortsRequest_t* p_type = this;     //Convert DATA to pointer of your type
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->length + 4); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.

    try
    {
        // Deserialize encapsulation.
        deser.read_encapsulation();
        payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
        p_type->deserialize(deser); //Deserialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    return true;
}

KeepAliveRequest_t::KeepAliveRequest_t()
{
    m_locator = 0;
}

KeepAliveRequest_t::~KeepAliveRequest_t()
{
}

size_t KeepAliveRequest_t::getBufferCdrSerializedSize(
        const KeepAliveRequest_t& data,
        size_t current_alignment)
{
    return getCdrSerializedSize(data, current_alignment) + 4;
}

size_t KeepAliveRequest_t::getCdrSerializedSize(
        const KeepAliveRequest_t& /*data*/,
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += 24 + eprosima::fastcdr::Cdr::alignment(current_alignment, 24);


    return current_alignment - initial_alignment;
}

void KeepAliveRequest_t::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    scdr << m_locator;
}

void KeepAliveRequest_t::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    dcdr >> m_locator;
}

bool KeepAliveRequest_t::serialize(
        SerializedPayload_t* payload)
{
    KeepAliveRequest_t* p_type = this;
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->max_size); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

    try
    {
        // Serialize encapsulation
        ser.serialize_encapsulation();
        p_type->serialize(ser); // Serialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    payload->length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    return true;
}

bool KeepAliveRequest_t::deserialize(
        SerializedPayload_t* payload)
{
    KeepAliveRequest_t* p_type = this;     //Convert DATA to pointer of your type
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->length + 4); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.

    try
    {
        // Deserialize encapsulation.
        deser.read_encapsulation();
        payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
        p_type->deserialize(deser); //Deserialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    return true;
}

LogicalPortIsClosedRequest_t::LogicalPortIsClosedRequest_t()
{
    m_logicalPort = 0;
}

LogicalPortIsClosedRequest_t::~LogicalPortIsClosedRequest_t()
{
}

size_t LogicalPortIsClosedRequest_t::getBufferCdrSerializedSize(
        const LogicalPortIsClosedRequest_t& data,
        size_t current_alignment)
{
    return getCdrSerializedSize(data, current_alignment) + 4;
}

size_t LogicalPortIsClosedRequest_t::getCdrSerializedSize(
        const LogicalPortIsClosedRequest_t& /*data*/,
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += 2 + eprosima::fastcdr::Cdr::alignment(current_alignment, 2);


    return current_alignment - initial_alignment;
}

void LogicalPortIsClosedRequest_t::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    scdr << m_logicalPort;
}

void LogicalPortIsClosedRequest_t::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    dcdr >> m_logicalPort;
}

bool LogicalPortIsClosedRequest_t::serialize(
        SerializedPayload_t* payload)
{
    LogicalPortIsClosedRequest_t* p_type = this;
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->max_size); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

    try
    {
        // Serialize encapsulation
        ser.serialize_encapsulation();
        p_type->serialize(ser); // Serialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    payload->length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    return true;
}

bool LogicalPortIsClosedRequest_t::deserialize(
        SerializedPayload_t* payload)
{
    LogicalPortIsClosedRequest_t* p_type = this;     //Convert DATA to pointer of your type
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->length + 4); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.

    try
    {
        // Deserialize encapsulation.
        deser.read_encapsulation();
        payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
        p_type->deserialize(deser); //Deserialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    return true;
}

BindConnectionResponse_t::BindConnectionResponse_t()
{
    m_locator = 0;
}

BindConnectionResponse_t::~BindConnectionResponse_t()
{
}

size_t BindConnectionResponse_t::getBufferCdrSerializedSize(
        const BindConnectionResponse_t& data,
        size_t current_alignment)
{
    return getCdrSerializedSize(data, current_alignment) + 4;
}

size_t BindConnectionResponse_t::getCdrSerializedSize(
        const BindConnectionResponse_t& /*data*/,
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += 24 + eprosima::fastcdr::Cdr::alignment(current_alignment, 24);


    return current_alignment - initial_alignment;
}

void BindConnectionResponse_t::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    scdr << m_locator;
}

void BindConnectionResponse_t::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    dcdr >> m_locator;
}

bool BindConnectionResponse_t::serialize(
        SerializedPayload_t* payload)
{
    BindConnectionResponse_t* p_type = this;
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->max_size); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

    try
    {
        // Serialize encapsulation
        ser.serialize_encapsulation();
        p_type->serialize(ser); // Serialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    payload->length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    return true;
}

bool BindConnectionResponse_t::deserialize(
        SerializedPayload_t* payload)
{
    BindConnectionResponse_t* p_type = this;     //Convert DATA to pointer of your type
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->length + 4); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.

    try
    {
        // Deserialize encapsulation.
        deser.read_encapsulation();
        payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
        p_type->deserialize(deser); //Deserialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    return true;
}

CheckLogicalPortsResponse_t::CheckLogicalPortsResponse_t()
{
}

CheckLogicalPortsResponse_t::~CheckLogicalPortsResponse_t()
{
}

size_t CheckLogicalPortsResponse_t::getBufferCdrSerializedSize(
        const CheckLogicalPortsResponse_t& data,
        size_t current_alignment)
{
    return getCdrSerializedSize(data, current_alignment) + 4;
}

size_t CheckLogicalPortsResponse_t::getCdrSerializedSize(
        const CheckLogicalPortsResponse_t& data,
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);
    current_alignment += (data.availableLogicalPorts().size() * 2) + eprosima::fastcdr::Cdr::alignment(
        current_alignment, 2);


    return current_alignment - initial_alignment;
}

void CheckLogicalPortsResponse_t::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{
    scdr << m_availableLogicalPorts;
}

void CheckLogicalPortsResponse_t::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{
    dcdr >> m_availableLogicalPorts;
}

bool CheckLogicalPortsResponse_t::serialize(
        SerializedPayload_t* payload)
{
    CheckLogicalPortsResponse_t* p_type = this;
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->max_size); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that serializes the data.
    payload->encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

    try
    {
        // Serialize encapsulation
        ser.serialize_encapsulation();
        p_type->serialize(ser); // Serialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    payload->length = (uint32_t)ser.getSerializedDataLength(); //Get the serialized length
    return true;
}

bool CheckLogicalPortsResponse_t::deserialize(
        SerializedPayload_t* payload)
{
    CheckLogicalPortsResponse_t* p_type = this;     //Convert DATA to pointer of your type
    eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->length + 4); // Object that manages the raw buffer.
    eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
            eprosima::fastcdr::Cdr::DDS_CDR); // Object that deserializes the data.

    try
    {
        // Deserialize encapsulation.
        deser.read_encapsulation();
        payload->encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
        p_type->deserialize(deser); //Deserialize the object:
    }
    catch (eprosima::fastcdr::exception::Exception& /*exception*/)
    {
        return false;
    }

    return true;
}

} // namespace rtps
} // namespace fastdds
} // Namespaces
