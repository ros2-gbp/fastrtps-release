// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/*************************************************************************
 * @file SimpleRTPSPubSubType.cpp
 * This header file contains the implementation of the serialization functions.
 *
 * This file was generated by the tool fastrtpsgen.
 */


#include "fastcdr/FastBuffer.h"
#include "fastcdr/Cdr.h"

#include "SimplePubSubType.h"

SimplePubSubType::SimplePubSubType()
{
	m_topicDataTypeName = std::string("SimpleStructPubSubType");
	m_typeSize = (uint32_t)SimpleStruct::getMaxCdrSerializedSize();
	m_isGetKeyDefined = SimpleStruct::isKeyDefined();
	m_keyBuffer = (unsigned char*)malloc(SimpleStruct::getKeyMaxCdrSerializedSize()>16 ? SimpleStruct::getKeyMaxCdrSerializedSize() : 16);
}

SimplePubSubType::~SimplePubSubType()
{
	if(m_keyBuffer!=NULL)
		delete(m_keyBuffer);
}

bool SimplePubSubType::serialize(void *data, SerializedPayload_t *payload)
{
	SimpleStruct *p_type = (SimpleStruct*) data;

	// Object that manages the raw buffer.
	eprosima::fastcdr::FastBuffer fastbuffer((char*) payload->data, payload->max_size);
	// Object that serializes the data.
	eprosima::fastcdr::Cdr ser(fastbuffer,eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS);
	//Select the correct endianess
	payload->encapsulation = CDR_LE;
	// Serialize the object:
	p_type->serialize(ser);
	//Get the serialized length
    payload->length = (uint16_t)ser.getSerializedDataLength();

	return true;
}

bool SimplePubSubType::deserialize(SerializedPayload_t* payload, void* data)
{
	//CONVERT DATA to pointer of your type
	SimpleStruct* p_type = (SimpleStruct*) data;

	// Object that manages the raw buffer.
	eprosima::fastcdr::FastBuffer fastbuffer((char*)payload->data, payload->length);
	//select the correct endianess
	eprosima::fastcdr::Cdr::Endianness endian = payload->encapsulation == CDR_LE ? eprosima::fastcdr::Cdr::LITTLE_ENDIANNESS : eprosima::fastcdr::Cdr::BIG_ENDIANNESS;
	// Object that deserializes the data.
	eprosima::fastcdr::Cdr deser(fastbuffer,endian);
	//deserialize the object:
	p_type->deserialize(deser);
	return true;
}

bool SimplePubSubType::getKey(void *data, InstanceHandle_t* handle, bool force_md5)
{
	if(!m_isGetKeyDefined)
		return false;

	SimpleStruct* p_type = (SimpleStruct*) data;

	// Object that manages the raw buffer.
	eprosima::fastcdr::FastBuffer fastbuffer((char*)m_keyBuffer,SimpleStruct::getKeyMaxCdrSerializedSize());
	// Object that serializes the data.
	eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS);

	p_type->serializeKey(ser);
	if(force_md5 || SimpleStruct::getKeyMaxCdrSerializedSize()>16)
	{
		m_md5.init();
		m_md5.update(m_keyBuffer,(unsigned int)ser.getSerializedDataLength());
		m_md5.finalize();
		for(uint8_t i = 0;i<16;++i)
    	{
        	handle->value[i] = m_md5.digest[i];
    	}
    	}
    else
    {
    	for(uint8_t i = 0;i<16;++i)
    	{
        	handle->value[i] = m_keyBuffer[i];
    	}
    }

	return true;
}
