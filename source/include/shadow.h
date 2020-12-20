/*
 * AWS IoT Device Shadow v1.0.2
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file shadow.h
 * @brief User-facing Shadow functions, and parameter structs.
 */

#ifndef SHADOW_H_
#define SHADOW_H_

/* Standard includes. */
#include <stdint.h>

/* SHADOW_DO_NOT_USE_CUSTOM_CONFIG allows building the Shadow library
 * without a custom config. If a custom config is provided, the
 * SHADOW_DO_NOT_USE_CUSTOM_CONFIG macro should not be defined. */
#ifndef SHADOW_DO_NOT_USE_CUSTOM_CONFIG
    /* Include custom config file before other headers. */
    #include "shadow_config.h"
#endif

/* Include config defaults header to get default values of configs not
 * defined in shadow_config_defaults.h file. */
#include "shadow_config_defaults.h"

/*--------------------------- Shadow types ---------------------------*/

/**
 * @ingroup shadow_enum_types
 * @brief Each of these values describes the type of a shadow message.
 *        https://docs.aws.amazon.com/iot/latest/developerguide/device-shadow-mqtt.html
 */
typedef enum ShadowMessageType
{
    ShadowMessageTypeGetAccepted = 0,
    ShadowMessageTypeGetRejected,
    ShadowMessageTypeDeleteAccepted,
    ShadowMessageTypeDeleteRejected,
    ShadowMessageTypeUpdateAccepted,
    ShadowMessageTypeUpdateRejected,
    ShadowMessageTypeUpdateDocuments,
    ShadowMessageTypeUpdateDelta,
    ShadowMessageTypeMaxNum
} ShadowMessageType_t;

/**
 * @ingroup shadow_enum_types
 * @brief Each of these values describes the type of a shadow topic string.
 *
 * These are used for topicType parameter of Shadow_GetTopicString() to tell it
 * what topic string to assemble.
 */
typedef enum ShadowTopicStringType
{
    ShadowTopicStringTypeGet = 0,
    ShadowTopicStringTypeGetAccepted,
    ShadowTopicStringTypeGetRejected,
    ShadowTopicStringTypeDelete,
    ShadowTopicStringTypeDeleteAccepted,
    ShadowTopicStringTypeDeleteRejected,
    ShadowTopicStringTypeUpdate,
    ShadowTopicStringTypeUpdateAccepted,
    ShadowTopicStringTypeUpdateRejected,
    ShadowTopicStringTypeUpdateDocuments,
    ShadowTopicStringTypeUpdateDelta,
    ShadowTopicStringTypeMaxNum
} ShadowTopicStringType_t;

/**
 * @ingroup shadow_enum_types
 * @brief Return codes from Shadow functions.
 */
typedef enum ShadowStatus
{
    SHADOW_SUCCESS = 0,               /**< @brief Shadow function success. */
    SHADOW_FAIL,                      /**< @brief Shadow function encountered error. */
    SHADOW_BAD_PARAMETER,             /**< @brief Input parameter is invalid. */
    SHADOW_BUFFER_TOO_SMALL,          /**< @brief The provided buffer is too small. */
    SHADOW_THINGNAME_PARSE_FAILED,    /**< @brief Could not parse the thing name. */
    SHADOW_MESSAGE_TYPE_PARSE_FAILED, /**< @brief Could not parse the shadow type. */
    SHADOW_ROOT_PARSE_FAILED,         /**< @brief Could not parse the classic or named shadow root. */
    SHADOW_SHADOWNAME_PARSE_FAILED    /**< @brief Could not parse the shadow name (in the case of a named shadow topic). */
} ShadowStatus_t;

/*------------------------ Shadow library functions -------------------------*/

/**
 * @ingroup shadow_constants
 * @brief The common prefix of all Shadow MQTT topics
 *        from here https://docs.aws.amazon.com/iot/latest/developerguide/device-shadow-mqtt.html.
 */
#define SHADOW_PREFIX                     "$aws/things/"

/**
 * @ingroup shadow_constants
 * @brief The length of #SHADOW_PREFIX.
 */
#define SHADOW_PREFIX_LENGTH              ( ( uint16_t ) ( sizeof( SHADOW_PREFIX ) - 1U ) )

/**
 * @ingroup shadow_constants
 * @brief The root of all unnamed "Classic" Shadow MQTT topics
 *        from here https://docs.aws.amazon.com/iot/latest/developerguide/device-shadow-mqtt.html.
 */
#define SHADOW_CLASSIC_ROOT               "/shadow"

/**
 * @ingroup shadow_constants
 * @brief The length of #SHADOW_CLASSIC_ROOT.
 */
#define SHADOW_CLASSIC_ROOT_LENGTH        ( ( uint16_t ) ( sizeof( SHADOW_CLASSIC_ROOT ) - 1U ) )

/**
 * @ingroup shadow_constants
 * @brief The common root of all named Shadow MQTT topics
 *        from here https://docs.aws.amazon.com/iot/latest/developerguide/device-shadow-mqtt.html.
 */
#define SHADOW_NAMED_ROOT                 "/shadow/name/"

/**
 * @ingroup shadow_constants
 * @brief The length of #SHADOW_NAMED_ROOT.
 */
#define SHADOW_NAMED_ROOT_LENGTH          ( ( uint16_t ) ( sizeof( SHADOW_NAMED_ROOT ) - 1U ) )

/**
 * @ingroup shadow_constants
 * @brief The string representing a Shadow "DELETE" operation in a Shadow MQTT topic.
 */
#define SHADOW_OP_DELETE                  "/delete"

/**
 * @ingroup shadow_constants
 * @brief The length of #SHADOW_OP_DELETE.
 */
#define SHADOW_OP_DELETE_LENGTH           ( ( uint16_t ) ( sizeof( SHADOW_OP_DELETE ) - 1U ) )

/**
 * @ingroup shadow_constants
 * @brief The string representing a Shadow "GET" operation in a Shadow MQTT topic.
 */
#define SHADOW_OP_GET                     "/get"

/**
 * @ingroup shadow_constants
 * @brief The length of #SHADOW_OP_GET.
 */
#define SHADOW_OP_GET_LENGTH              ( ( uint16_t ) ( sizeof( SHADOW_OP_GET ) - 1U ) )

/**
 * @ingroup shadow_constants
 * @brief The string representing a Shadow "UPDATE" operation in a Shadow MQTT topic.
 */
#define SHADOW_OP_UPDATE                  "/update"

/**
 * @ingroup shadow_constants
 * @brief The length of #SHADOW_OP_UPDATE.
 */
#define SHADOW_OP_UPDATE_LENGTH           ( ( uint16_t ) ( sizeof( SHADOW_OP_UPDATE ) - 1U ) )

/**
 * @ingroup shadow_constants
 * @brief The suffix for a Shadow operation "accepted" topic.
 */
#define SHADOW_SUFFIX_ACCEPTED            "/accepted"

/**
 * @ingroup shadow_constants
 * @brief The length of #SHADOW_SUFFIX_ACCEPTED.
 */
#define SHADOW_SUFFIX_ACCEPTED_LENGTH     ( ( uint16_t ) ( sizeof( SHADOW_SUFFIX_ACCEPTED ) - 1U ) )

/**
 * @ingroup shadow_constants
 * @brief The suffix for a Shadow operation "rejected" topic.
 */
#define SHADOW_SUFFIX_REJECTED            "/rejected"

/**
 * @ingroup shadow_constants
 * @brief The length of #SHADOW_SUFFIX_REJECTED.
 */
#define SHADOW_SUFFIX_REJECTED_LENGTH     ( ( uint16_t ) ( sizeof( SHADOW_SUFFIX_REJECTED ) - 1U ) )

/**
 * @ingroup shadow_constants
 * @brief The suffix for a Shadow "delta" topic.
 */
#define SHADOW_SUFFIX_DELTA               "/delta"

/**
 * @ingroup shadow_constants
 * @brief The length of #SHADOW_SUFFIX_DELTA.
 */
#define SHADOW_SUFFIX_DELTA_LENGTH        ( ( uint16_t ) ( sizeof( SHADOW_SUFFIX_DELTA ) - 1U ) )

/**
 * @ingroup shadow_constants
 * @brief The suffix for a Shadow "documents" topic.
 */
#define SHADOW_SUFFIX_DOCUMENTS           "/documents"

/**
 * @ingroup shadow_constants
 * @brief The length of #SHADOW_SUFFIX_DOCUMENTS.
 */
#define SHADOW_SUFFIX_DOCUMENTS_LENGTH    ( ( uint16_t ) ( sizeof( SHADOW_SUFFIX_DOCUMENTS ) - 1U ) )

/**
 * @ingroup shadow_constants
 * @brief The suffix for a "null" suffix.
 */
#define SHADOW_SUFFIX_NULL

/**
 * @ingroup shadow_constants
 * @brief The length of null suffix.
 */
#define SHADOW_SUFFIX_NULL_LENGTH      ( 0U )

/**
 * @ingroup shadow_constants
 * @brief The maximum length of Thing Name.
 */
#define SHADOW_THINGNAME_LENGTH_MAX    ( 128U )

/**
 * @ingroup shadow_constants
 * @brief The maximum length of Shadow Name.
 */
#define SHADOW_NAME_LENGTH_MAX         ( 64U )

/**
 * @ingroup shadow_constants
 * @brief The name string for the unnamed "Classic" shadow.
 */
#define SHADOW_NAME_CLASSIC            ""

/**
 * @ingroup shadow_constants
 * @brief Compute shadow topic length.
 *
 * The format of shadow topic strings is defined at https://docs.aws.amazon.com/iot/latest/developerguide/device-shadow-mqtt.html
 *
 * A shadow topic string takes one of the two forms, in the case of an unnamed ("Classic") shadow:
 *     $aws/things/\<thingName\>/shadow/\<operation\>
 *     $aws/things/\<thingName\>/shadow/\<operation\>/\<suffix\>
 *
 * Or as follows, in the case of a named shadow:
 *     $aws/things/\<thingName\>/shadow/name/<shadowName\>/<operation\>
 *     $aws/things/\<thingName\>/shadow/name/<shadowName\>/<operation\>/\<suffix\>
 *
 * The \<thingName\>, \<shadowName\>, \<operation\> and \<suffix\> segments correspond to the
 * four input parameters of this macro. The \<suffix\> part can be null.
 *
 * When thingName and shadow name are known to be "myThing"  and "myShadow" at compile time,
 * invoke the macro like this:
 * (In this case, the length is a constant at compile time.)
 *
 *     SHADOW_TOPIC_LENGTH( SHADOW_OP_UPDATE_LENGTH, SHADOW_SUFFIX_DELTA_LENGTH, 7, 8 )
 *
 * When thingName and shadowName are only known at run time, and held in variables myThingName
 * and myShadowName, invoke the macro like this:
 *
 *     SHADOW_TOPIC_LENGTH( SHADOW_OP_UPDATE_LENGTH, SHADOW_SUFFIX_DELTA_LENGTH,
 *                       strlen( ( const char * ) myThingName ),
 *                       strlen( ( const char * ) myShadowName ) )
 *
 * To use an unnamed ("Classic") shadow, the shadowName length passed must be zero.
 *
 * @param[in] operationLength   Can be one of:
 *                                  - #SHADOW_OP_UPDATE_LENGTH
 *                                  - #SHADOW_OP_DELETE_LENGTH
 *                                  - #SHADOW_OP_GET_LENGTH
 * @param[in] suffixLength      Can be one of:
 *                                  - #SHADOW_SUFFIX_NULL_LENGTH
 *                                  - #SHADOW_SUFFIX_ACCEPTED_LENGTH
 *                                  - #SHADOW_SUFFIX_REJECTED_LENGTH
 *                                  - #SHADOW_SUFFIX_DELTA_LENGTH
 *                                  - #SHADOW_SUFFIX_DOCUMENTS_LENGTH
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH( operationLength, suffixLength, thingNameLength, shadowNameLength ) \
    ( operationLength + suffixLength + thingNameLength + shadowNameLength +                     \
      SHADOW_PREFIX_LENGTH +                                                                    \
      ( ( shadowNameLength > 0 ) ? SHADOW_NAMED_ROOT_LENGTH : SHADOW_CLASSIC_ROOT_LENGTH ) )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/update" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/update".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_UPDATE( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_UPDATE_LENGTH, SHADOW_SUFFIX_NULL_LENGTH, thingNameLength, shadowNameLength )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/update/accepted" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/update/accepted".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_UPDATE_ACCEPTED( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_UPDATE_LENGTH, SHADOW_SUFFIX_ACCEPTED_LENGTH, thingNameLength, shadowNameLength )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/update/rejected" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/update/rejected".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_UPDATE_REJECTED( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_UPDATE_LENGTH, SHADOW_SUFFIX_REJECTED_LENGTH, thingNameLength, shadowNameLength )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/update/documents" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/update/documents".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_UPDATE_DOCUMENTS( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_UPDATE_LENGTH, SHADOW_SUFFIX_DOCUMENTS_LENGTH, thingNameLength, shadowNameLength )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/update/delta" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/update/delta".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_UPDATE_DELTA( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_UPDATE_LENGTH, SHADOW_SUFFIX_DELTA_LENGTH, thingNameLength, shadowNameLength )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/get" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/get".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_GET( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_GET_LENGTH, SHADOW_SUFFIX_NULL_LENGTH, thingNameLength, shadowNameLength )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/get/accepted" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/get/accepted".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_GET_ACCEPTED( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_GET_LENGTH, SHADOW_SUFFIX_ACCEPTED_LENGTH, thingNameLength, shadowNameLength )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/get/rejected" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/get/rejected".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_GET_REJECTED( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_GET_LENGTH, SHADOW_SUFFIX_REJECTED_LENGTH, thingNameLength, shadowNameLength )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/delete" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/delete".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_DELETE( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_DELETE_LENGTH, SHADOW_SUFFIX_NULL_LENGTH, thingNameLength, shadowNameLength )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/delete/accepted" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/delete".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_DELETE_ACCEPTED( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_DELETE_LENGTH, SHADOW_SUFFIX_ACCEPTED_LENGTH, thingNameLength, shadowNameLength )

/**
 * @brief Compute the length of shadow topic "$aws/things/<thingName>/shadow/delete/rejected" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/delete".
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_DELETE_REJECTED( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_DELETE_LENGTH, SHADOW_SUFFIX_REJECTED_LENGTH, thingNameLength, shadowNameLength )

/**
 * @ingroup shadow_constants
 * @brief Compute the length of the longest shadow topic.
 *
 * @param[in] thingNameLength   Length of the thingName excluding the ending NULL.
 * @param[in] shadowNameLength  Length of the shadowName excluding the ending NULL. Zero for "Classic" shadow.
 *
 * @return Length of the shadow topic in bytes.
 */
#define SHADOW_TOPIC_LENGTH_MAX( thingNameLength, shadowNameLength ) \
    SHADOW_TOPIC_LENGTH( SHADOW_OP_UPDATE_LENGTH, SHADOW_SUFFIX_DOCUMENTS_LENGTH, thingNameLength, shadowNameLength )

/**
 * @ingroup shadow_constants
 * @brief Assemble constant shadow topic strings when Thing Name is known at compile time.
 *
 * When thingName is known to be "myThing" at compile time, invoke the macro like this:
 *
 *     SHADOW_TOPIC_STRING( SHADOW_OP_UPDATE, SHADOW_SUFFIX_DELTA, "myThing" )
 *
 * When thingName is only known at run time, do not use this macro. Use the
 * Shadow_GetTopicString() function instead.
 *
 * @param[in] operation     Can be one of:
 *                              - #SHADOW_OP_UPDATE
 *                              - #SHADOW_OP_DELETE
 *                              - #SHADOW_OP_GET
 * @param[in] suffix        Can be one of:
 *                              - #SHADOW_SUFFIX_NULL
 *                              - #SHADOW_SUFFIX_ACCEPTED
 *                              - #SHADOW_SUFFIX_REJECTED
 *                              - #SHADOW_SUFFIX_DELTA
 *                              - #SHADOW_SUFFIX_DOCUMENTS
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING( thingName, shadowName, operation, suffix )           \
    ( ( sizeof( shadowName ) > 1 ) ?                                              \
      ( SHADOW_PREFIX thingName SHADOW_NAMED_ROOT shadowName operation suffix ) : \
      ( SHADOW_PREFIX thingName SHADOW_CLASSIC_ROOT operation suffix ) )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/update" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/update".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_UPDATE( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_UPDATE, SHADOW_SUFFIX_NULL )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/update/accepted" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/update/accepted".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_UPDATE_ACCEPTED( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_UPDATE, SHADOW_SUFFIX_ACCEPTED )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/update/rejected" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/update/rejected".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_UPDATE_REJECTED( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_UPDATE, SHADOW_SUFFIX_REJECTED )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/update/documents" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/update/documents".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_UPDATE_DOCUMENTS( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_UPDATE, SHADOW_SUFFIX_DOCUMENTS )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/update/delta" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/update/delta".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_UPDATE_DELTA( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_UPDATE, SHADOW_SUFFIX_DELTA )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/get" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/get".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_GET( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_GET, SHADOW_SUFFIX_NULL )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/get/accepted" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/get/accepted".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_GET_ACCEPTED( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_GET, SHADOW_SUFFIX_ACCEPTED )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/get/rejected" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/get/rejected".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_GET_REJECTED( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_GET, SHADOW_SUFFIX_REJECTED )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/delete" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/delete".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_DELETE( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_DELETE, SHADOW_SUFFIX_NULL )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/delete/accepted" or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/delete/accepted".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_DELETE_ACCEPTED( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_DELETE, SHADOW_SUFFIX_ACCEPTED )

/**
 * @brief Assemble shadow topic string "$aws/things/<thingName>/shadow/delete/rejected". or
 *        "$aws/things/<thingName>/shadow/name/<shadowName>/delete/rejected".
 *
 * @param[in] thingName     Thing Name.
 * @param[in] shadowName    Shadow Name. Empty string for an unnamed ("Classic") shadow.
 *
 * @return Topic string.
 */
#define SHADOW_TOPIC_STRING_DELETE_REJECTED( thingName, shadowName ) \
    SHADOW_TOPIC_STRING( thingName, shadowName, SHADOW_OP_DELETE, SHADOW_SUFFIX_REJECTED )

/**
 * @brief Assemble shadow topic string when Thing Name or Shadow Name is only known at run time.
 *        If both the Thing Name and Shadow Name are known at compile time, use
 *        @link #SHADOW_TOPIC_STRING_UPDATE SHADOW_TOPIC_STRING_* @endlink macros instead.
 *
 * @param[in]  topicType Indicates what topic will be written into the buffer pointed to by pTopicBuffer.
 *             can be one of:
 *                 - ShadowTopicStringTypeGet
 *                 - ShadowTopicStringTypeGetAccepted
 *                 - ShadowTopicStringTypeGetRejected
 *                 - ShadowTopicStringTypeDelete
 *                 - ShadowTopicStringTypeDeleteAccepted
 *                 - ShadowTopicStringTypeDeleteRejected
 *                 - ShadowTopicStringTypeUpdate
 *                 - ShadowTopicStringTypeUpdateAccepted
 *                 - ShadowTopicStringTypeUpdateRejected
 *                 - ShadowTopicStringTypeUpdateDocuments
 *                 - ShadowTopicStringTypeUpdateDelta
 * @param[in]  pThingName Thing Name string. No need to be null terminated. Must not be NULL.
 * @param[in]  thingNameLength Length of Thing Name string pointed to by pThingName. Must not be zero.
 * @param[in]  pShadowName Shadow Name string. No need to be null terminated. Must not be NULL. Empty string for classic shadow.
 * @param[in]  shadowNameLength Length of Shadow Name string pointed to by pShadowName. Zero for classic shadow.
 * @param[out] pTopicBuffer Pointer to buffer for returning the topic string.
 *             Caller is responsible for supplying memory pointed to by pTopicBuffer.
 *             This function does not fill in the terminating null character. The app
 *             can supply a buffer that does not have space for holding the null character.
 * @param[in]  bufferSize Length of pTopicBuffer. This function will return error if
 *             bufferSize is less than the length of the assembled topic string.
 * @param[out] pOutLength Pointer to caller-supplied memory for returning the length of the topic string.
 * @return     One of the following:
 *             - SHADOW_SUCCESS if successful.
 *             - An error code if failed to assemble.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 *
 * #define SHADOW_TOPIC_MAX_LENGTH  ( 256U )
 *
 * ShadowStatus_t shadowStatus = SHADOW_SUCCESS;
 * char topicBuffer[ SHADOW_TOPIC_MAX_LENGTH ] = { 0 };
 * uint16_t bufferSize = SHADOW_TOPIC_MAX_LENGTH;
 * uint16_t outLength = 0;
 * const char thingName[] = "TestThingName";
 * uint16_t thingNameLength  = ( sizeof( thingName ) - 1U );
 * const char shadowName[] = "TestShadowName";
 * uint16_t shadowNameLength  = ( sizeof( shadowName ) - 1U );
 *
 * shadowStatus = Shadow_GetTopicString( ShadowTopicStringTypeUpdateDelta,
 *                                       thingName,
 *                                       thingNameLength,
 *                                       shadowName,
 *                                       shadowNameLength,
 *                                       & ( topicBuffer[ 0 ] ),
 *                                       bufferSize,
 *                                       & outLength );
 *
 * if( shadowStatus == SHADOW_SUCCESS )
 * {
 *      // The assembled topic string is put in pTopicBuffer with the length outLength.
 * }
 *
 * @endcode
 */
/* @[declare_shadow_gettopicstring] */
ShadowStatus_t Shadow_GetTopicString( ShadowTopicStringType_t topicType,
                                      const char * pThingName,
                                      uint8_t thingNameLength,
                                      const char * pShadowName,
                                      uint8_t shadowNameLength,
                                      char * pTopicBuffer,
                                      uint16_t bufferSize,
                                      uint16_t * pOutLength );
/* @[declare_shadow_gettopicstring] */

/**
 * @brief Given the topic string of an incoming message, determine whether it is
 *        related to a device shadow; if it is, return information about the type of
 *        device shadow message, and a pointer to the Thing Name inside of the topic string.
 *        See #ShadowMessageType_t for the list of message types.  Those types correspond to
 *        Device Shadow Topics.
 *
 * @note When this function returns, the pointer pThingName points at the first character
 *       of the \<thingName\> segment inside of the topic string.
 *       Caller is responsible for keeping the memory holding the topic string around while
 *       accessing the Thing Name through pThingName.
 *
 * @param[in]  pTopic Pointer to the MQTT topic string. Does not have to be null-terminated.
 * @param[in]  topicLength Length of the MQTT topic string.
 * @param[out] pMessageType Pointer to caller-supplied memory for returning the type of the shadow message.
 * @param[out] pThingName Points to the 1st character of Thing Name inside of the topic string, and can be
 *             null if caller does not need to know the Thing Name contained in the topic string.
 * @param[out] pThingNameLength Pointer to caller-supplied memory for returning the length of the Thing Name,
 *             and can be null if caller does not need to know the Thing Name contained in the topic string.
 * @param[out] pShadowName Points to the 1st character of Shadow Name inside of the topic string, and can be
 *             null if caller does not need to know the Shadow Name contained in the topic string. Null is
 *             returned if the shadow is Classic.
 * @param[out] pShadowNameLength Pointer to caller-supplied memory for returning the length of the Shadow Name,
 *             and can be null if caller does not need to know the Shadow Name contained in the topic string.
 *             A value of 0 is returned if the shadow is Classic.
 * @return     One of the following:
 *             - #SHADOW_SUCCESS if the message is related to a device shadow;
 *             - An error code defined in #ShadowStatus_t if the message is not related to a device shadow,
 *               if any input parameter is invalid, or if the function fails to
 *               parse the topic string.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 *
 * #define SHADOW_TOPIC_MAX_LENGTH  ( 256U )
 *
 * ShadowStatus_te shadowStatus = SHADOW_SUCCESS;
 * char * pTopicName; //usually supplied by MQTT stack
 * uint16_t topicNameLength; //usually supplied by MQTT stack
 * ShadowMessageType_t messageType;
 *
 * shadowStatus = Shadow_MatchTopic( pTopicName,
 *                                   topicNameLength,
 *                                   &messageType,
 *                                   NULL,
 *                                   NULL,
 *                                   NULL,
 *                                   NULL );
 *
 * if( shadowStatus == SHADOW_SUCCESS )
 * {
 *      // It is a device shadow message. And the type of the message has been returned in messageType.
 *      // Now we can act on the message.
 * }
 *
 * @endcode
 */
/* @[declare_shadow_matchtopic] */
ShadowStatus_t Shadow_MatchTopic( const char * pTopic,
                                  uint16_t topicLength,
                                  ShadowMessageType_t * pMessageType,
                                  const char ** pThingName,
                                  uint16_t * pThingNameLength,
                                  const char ** pShadowName,
                                  uint16_t * pShadowNameLength );
/* @[declare_shadow_matchtopic] */

#endif /* ifndef SHADOW_H_ */
