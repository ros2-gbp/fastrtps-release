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

/*!
 * @file TestTypeObject.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _FAST_DDS_GENERATED_TEST_TYPE_OBJECT_H_
#define _FAST_DDS_GENERATED_TEST_TYPE_OBJECT_H_


#include <fastrtps/types/TypeObject.h>
#include <map>

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif
#else
#define eProsima_user_DllExport
#endif

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(Test_SOURCE)
#define Test_DllAPI __declspec( dllexport )
#else
#define Test_DllAPI __declspec( dllimport )
#endif // Test_SOURCE
#else
#define Test_DllAPI
#endif
#else
#define Test_DllAPI
#endif // _WIN32

using namespace eprosima::fastrtps::types;

eProsima_user_DllExport void registerTestTypes();

eProsima_user_DllExport const TypeIdentifier* GetMyEnumIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMyEnumObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalMyEnumObject();
eProsima_user_DllExport const TypeObject* GetCompleteMyEnumObject();

eProsima_user_DllExport const TypeIdentifier* GetMyAliasEnumIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMyAliasEnumObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalMyAliasEnumObject();
eProsima_user_DllExport const TypeObject* GetCompleteMyAliasEnumObject();

eProsima_user_DllExport const TypeIdentifier* GetMyAliasEnum2Identifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMyAliasEnum2Object(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalMyAliasEnum2Object();
eProsima_user_DllExport const TypeObject* GetCompleteMyAliasEnum2Object();

eProsima_user_DllExport const TypeIdentifier* GetMyAliasEnum3Identifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMyAliasEnum3Object(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalMyAliasEnum3Object();
eProsima_user_DllExport const TypeObject* GetCompleteMyAliasEnum3Object();

eProsima_user_DllExport const TypeIdentifier* GetBasicStructIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetBasicStructObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalBasicStructObject();
eProsima_user_DllExport const TypeObject* GetCompleteBasicStructObject();

eProsima_user_DllExport const TypeIdentifier* GetMyOctetArray500Identifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMyOctetArray500Object(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalMyOctetArray500Object();
eProsima_user_DllExport const TypeObject* GetCompleteMyOctetArray500Object();

eProsima_user_DllExport const TypeIdentifier* GetBSAlias5Identifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetBSAlias5Object(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalBSAlias5Object();
eProsima_user_DllExport const TypeObject* GetCompleteBSAlias5Object();

eProsima_user_DllExport const TypeIdentifier* GetMA3Identifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMA3Object(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalMA3Object();
eProsima_user_DllExport const TypeObject* GetCompleteMA3Object();

eProsima_user_DllExport const TypeIdentifier* GetMyMiniArrayIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMyMiniArrayObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalMyMiniArrayObject();
eProsima_user_DllExport const TypeObject* GetCompleteMyMiniArrayObject();

eProsima_user_DllExport const TypeIdentifier* GetMySequenceLongIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMySequenceLongObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalMySequenceLongObject();
eProsima_user_DllExport const TypeObject* GetCompleteMySequenceLongObject();

eProsima_user_DllExport const TypeIdentifier* GetComplexStructIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetComplexStructObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalComplexStructObject();
eProsima_user_DllExport const TypeObject* GetCompleteComplexStructObject();

eProsima_user_DllExport const TypeIdentifier* GetMyUnionIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMyUnionObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalMyUnionObject();
eProsima_user_DllExport const TypeObject* GetCompleteMyUnionObject();

eProsima_user_DllExport const TypeIdentifier* GetMyUnion2Identifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMyUnion2Object(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalMyUnion2Object();
eProsima_user_DllExport const TypeObject* GetCompleteMyUnion2Object();

eProsima_user_DllExport const TypeIdentifier* GetCompleteStructIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetCompleteStructObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalCompleteStructObject();
eProsima_user_DllExport const TypeObject* GetCompleteCompleteStructObject();

eProsima_user_DllExport const TypeIdentifier* GetKeyedStructIdentifier(bool complete = false);
eProsima_user_DllExport const TypeObject* GetKeyedStructObject(bool complete = false);
eProsima_user_DllExport const TypeObject* GetMinimalKeyedStructObject();
eProsima_user_DllExport const TypeObject* GetCompleteKeyedStructObject();


#endif // _FAST_DDS_GENERATED_TEST_TYPE_OBJECT_H_