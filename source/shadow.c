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
 * @file shadow.c
 * @brief Implements the user-facing functions of the Shadow library.
 */

/* Standard includes. */
#include <string.h>

/* Shadow includes. */
#include "shadow.h"

/**
 * @brief The string representing "/shadow/update/accepted".
 */
#define SHADOW_OP_UPDATE_ACCEPTED            SHADOW_OP_UPDATE SHADOW_SUFFIX_ACCEPTED

/**
 * @brief The string representing "/shadow/update/rejected".
 */
#define SHADOW_OP_UPDATE_REJECTED            SHADOW_OP_UPDATE SHADOW_SUFFIX_REJECTED

/**
 * @brief The string representing "/shadow/update/delta".
 */
#define SHADOW_OP_UPDATE_DELTA               SHADOW_OP_UPDATE SHADOW_SUFFIX_DELTA

/**
 * @brief The string representing "/shadow/update/document".
 */
#define SHADOW_OP_UPDATE_DOCUMENTS           SHADOW_OP_UPDATE SHADOW_SUFFIX_DOCUMENTS

/**
 * @brief The string representing "/shadow/delete/accepted".
 */
#define SHADOW_OP_DELETE_ACCEPTED            SHADOW_OP_DELETE SHADOW_SUFFIX_ACCEPTED

/**
 * @brief The string representing "/shadow/delete/accepted".
 */
#define SHADOW_OP_DELETE_REJECTED            SHADOW_OP_DELETE SHADOW_SUFFIX_REJECTED

/**
 * @brief The string representing "/shadow/get/accepted".
 */
#define SHADOW_OP_GET_ACCEPTED               SHADOW_OP_GET SHADOW_SUFFIX_ACCEPTED

/**
 * @brief The string representing "/shadow/get/accepted".
 */
#define SHADOW_OP_GET_REJECTED               SHADOW_OP_GET SHADOW_SUFFIX_REJECTED

/**
 * @brief The length of "/shadow/update/accepted".
 */
#define SHADOW_OP_UPDATE_ACCEPTED_LENGTH     ( SHADOW_OP_UPDATE_LENGTH + SHADOW_SUFFIX_ACCEPTED_LENGTH )

/**
 * @brief The length of "/shadow/update/rejected".
 */
#define SHADOW_OP_UPDATE_REJECTED_LENGTH     ( SHADOW_OP_UPDATE_LENGTH + SHADOW_SUFFIX_REJECTED_LENGTH )

/**
 * @brief The length of "/shadow/update/document".
 */
#define SHADOW_OP_UPDATE_DOCUMENTS_LENGTH    ( SHADOW_OP_UPDATE_LENGTH + SHADOW_SUFFIX_DOCUMENTS_LENGTH )

/**
 * @brief The length of "/shadow/update/rejected".
 */
#define SHADOW_OP_UPDATE_DELTA_LENGTH        ( SHADOW_OP_UPDATE_LENGTH + SHADOW_SUFFIX_DELTA_LENGTH )

/**
 * @brief The length of "/shadow/get/accepted".
 */
#define SHADOW_OP_GET_ACCEPTED_LENGTH        ( SHADOW_OP_GET_LENGTH + SHADOW_SUFFIX_ACCEPTED_LENGTH )

/**
 * @brief The length of "/shadow/get/rejected".
 */
#define SHADOW_OP_GET_REJECTED_LENGTH        ( SHADOW_OP_GET_LENGTH + SHADOW_SUFFIX_REJECTED_LENGTH )

/**
 * @brief The length of "/shadow/get/accepted".
 */
#define SHADOW_OP_DELETE_ACCEPTED_LENGTH     ( SHADOW_OP_DELETE_LENGTH + SHADOW_SUFFIX_ACCEPTED_LENGTH )

/**
 * @brief The length of "/shadow/delete/rejected".
 */
#define SHADOW_OP_DELETE_REJECTED_LENGTH     ( SHADOW_OP_DELETE_LENGTH + SHADOW_SUFFIX_REJECTED_LENGTH )

/**
 * @brief Check if Shadow_MatchTopicString has valid parameters.
 *
 * @param[in] pTopic Pointer to the topic string.
 * @param[in] topicLength Length of pTopic.
 * @param[in] pMessageType Pointer to caller-supplied memory for returning the type of the shadow message.
 *
 * @return Return SHADOW_SUCCESS if the parameters are valid;
 *         return SHADOW_BAD_PARAMETER if not.
 */
static ShadowStatus_t validateMatchTopicParameters( const char * pTopic,
                                                    uint16_t topicLength,
                                                    const ShadowMessageType_t * pMessageType );

/**
 * @brief Determine if the string contains the substring.
 *
 * @param[in] pString Pointer to the string.
 * @param[in] stringLength Length of pString.
 * @param[in] pSubString Pointer to the substring.
 * @param[in] subStringLength Length of pSubString.
 *
 * @return Return SHADOW_SUCCESS if it contains;
 *         return SHADOW_FAIL if not.
 */
static ShadowStatus_t containsSubString( const char * pString,
                                         uint16_t stringLength,
                                         const char * pSubString,
                                         uint16_t subStringLength );

/**
 * @brief Check if the Thing or Shadow Name is valid.
 *
 * @param[in] pString Pointer to the starting of a name.
 * @param[in] stringLength Length of pString.
 * @param[out] pNameLength Pointer to caller-supplied memory for returning the length of the Thing or Shadow Name.
 *
 * @return Return SHADOW_SUCCESS if it is valid;
 *         return SHADOW_FAIL if it is not.
 */
static ShadowStatus_t validateName( const char * pString,
                                    uint16_t stringLength,
                                    uint16_t * pNameLength );

/**
 * @brief Extract the Shadow message type from a string.
 *
 * @param[in] pString Pointer to the string.
 * @param[in] stringLength Length of pString.
 * @param[out] pMessageType Pointer to caller-supplied memory for returning the type of the shadow message.
 *
 * @return Return SHADOW_SUCCESS if successfully extracted;
 *         return SHADOW_MESSAGE_TYPE_PARSE_FAILED if failed.
 */
static ShadowStatus_t extractShadowMessageType( const char * pString,
                                                uint16_t stringLength,
                                                ShadowMessageType_t * pMessageType );

/**
 * @brief Extract the classic shadow root OR the named shadow root and shadow name from a topic string.
 *
 * @param[in] pTopic Pointer to the topic string.
 * @param[in] topicLength Length of pTopic.
 * @param[out] pConsumedTopicLength Pointer to caller-supplied memory for returning the consumed topic length.
 * @param[out] pShadowNameLength Pointer to caller-supplied memory for returning the shadow name length.
 *
 * @return Return SHADOW_SUCCESS if successfully extracted;
 *         return SHADOW_ROOT_PARSE_FAILED shadow root parsing fails.
 *         return SHADOW_SHADOWNAME_PARSE_FAILED shadow name parsing fails.
 */
static ShadowStatus_t extractShadowRootAndName( const char * pTopic,
                                                uint16_t topicLength,
                                                uint16_t * pConsumedTopicLength,
                                                uint16_t * pShadowNameLength );

/**
 * @brief Get the shadow operation string for a given shadow topic type.
 *
 * @param[in] topicType The given shadow topic type.
 *
 * @return The shadow operation string for the given shadow type.
 */
static const char * getShadowOperationString( ShadowTopicStringType_t topicType );

/**
 * @brief Get the shadow operation string length for a given shadow topic type.
 *
 * @param[in] topicType The given shadow topic type.
 *
 * @return The shadow operation string length for the given shadow type.
 */
static uint16_t getShadowOperationLength( ShadowTopicStringType_t topicType );

/**
 * @brief Creates a shadow topic string
 *
 * @param[in] topicType The type of shadow topic to be constructed.
 * @param[in] pThingName Pointer to the Thing name.
 * @param[in] thingNameLength The length of the Thing name.
 * @param[in] pShadowName Pointer to the Shadow name.
 * @param[in] shadowNameLength The length of the Shadow name.
 * @param[out] pTopicBuffer Pointer to caller-supplied memory for returning the constructed shadow topic string.
 */
static void createShadowTopicString( ShadowTopicStringType_t topicType,
                                     const char * pThingName,
                                     uint8_t thingNameLength,
                                     const char * pShadowName,
                                     uint8_t shadowNameLength,
                                     char * pTopicBuffer );

/*-----------------------------------------------------------*/

static ShadowStatus_t validateMatchTopicParameters( const char * pTopic,
                                                    uint16_t topicLength,
                                                    const ShadowMessageType_t * pMessageType )
{
    ShadowStatus_t shadowStatus = SHADOW_SUCCESS;

    if( ( pTopic == NULL ) ||
        ( topicLength == 0U ) ||
        ( pMessageType == NULL ) )
    {
        shadowStatus = SHADOW_BAD_PARAMETER;
        LogError( ( "Invalid input parameters pTopic: %p, topicLength: %u, pMessageType: %p.",
                    ( void * ) pTopic,
                    ( unsigned int ) topicLength,
                    ( void * ) pMessageType ) );
    }

    return shadowStatus;
}

/*-----------------------------------------------------------*/

static ShadowStatus_t containsSubString( const char * pString,
                                         uint16_t stringLength,
                                         const char * pSubString,
                                         uint16_t subStringLength )
{
    ShadowStatus_t returnStatus = SHADOW_FAIL;

    /* The string must be at least as long as the substring to contain it
     * completely. */
    if( stringLength >= subStringLength )
    {
        /* We are only checking up to subStringLength characters in the original
        * string. The string may be longer and contain additional characters. */
        if( strncmp( pString, pSubString, ( size_t ) subStringLength ) == 0 )
        {
            returnStatus = SHADOW_SUCCESS;
        }
    }

    return returnStatus;
}

/*-----------------------------------------------------------*/

static ShadowStatus_t validateName( const char * pString,
                                    uint16_t stringLength,
                                    uint16_t * pNameLength )
{
    uint16_t index = 0U;
    ShadowStatus_t returnStatus = SHADOW_FAIL;

    for( ; index < stringLength; index++ )
    {
        /* The name should always be terminated by a forward slash */
        if( pString[ index ] == ( char ) '/' )
        {
            if( index > 0U )
            {
                /* Only accept names of greater than zero length */
                *pNameLength = index;
                returnStatus = SHADOW_SUCCESS;
            }

            break;
        }
    }

    return returnStatus;
}

/*-----------------------------------------------------------*/

static ShadowStatus_t extractShadowRootAndName( const char * pTopic,
                                                uint16_t topicLength,
                                                uint16_t * pConsumedTopicLength,
                                                uint16_t * pShadowNameLength )
{
    ShadowStatus_t shadowStatus;

    /* Look for the named shadow root */
    shadowStatus = containsSubString( &( pTopic[ *pConsumedTopicLength ] ),
                                      topicLength - *pConsumedTopicLength,
                                      SHADOW_NAMED_ROOT,
                                      SHADOW_NAMED_ROOT_LENGTH );

    if( shadowStatus == SHADOW_SUCCESS )
    {
        /* Topic is a named shadow */
        *pConsumedTopicLength += SHADOW_NAMED_ROOT_LENGTH;

        /* Extract shadow name. */
        shadowStatus = validateName( &( pTopic[ *pConsumedTopicLength ] ),
                                     topicLength - *pConsumedTopicLength,
                                     pShadowNameLength );

        if( shadowStatus == SHADOW_SUCCESS )
        {
            *pConsumedTopicLength += *pShadowNameLength;
        }
        else
        {
            shadowStatus = SHADOW_SHADOWNAME_PARSE_FAILED;
            LogDebug( ( "Not related to Shadow, failed to parse shadow name in pTopic %s", pTopic ) );
        }
    }
    else
    {
        /* Not a named shadow. Try to match the classic shadow root. */
        shadowStatus = containsSubString( &( pTopic[ *pConsumedTopicLength ] ),
                                          topicLength - *pConsumedTopicLength,
                                          SHADOW_CLASSIC_ROOT,
                                          SHADOW_CLASSIC_ROOT_LENGTH );

        if( shadowStatus == SHADOW_SUCCESS )
        {
            *pConsumedTopicLength += SHADOW_CLASSIC_ROOT_LENGTH;
        }
        else
        {
            shadowStatus = SHADOW_ROOT_PARSE_FAILED;
            LogDebug( ( "Not related to Shadow, failed to parse shadow root in pTopic %s", pTopic ) );
        }
    }

    return shadowStatus;
}

/*-----------------------------------------------------------*/

static ShadowStatus_t extractShadowMessageType( const char * pString,
                                                uint16_t stringLength,
                                                ShadowMessageType_t * pMessageType )
{
    uint32_t index = 0U;
    ShadowStatus_t returnStatus = SHADOW_FAIL;

    /* Lookup table for Shadow message string. */
    static const char * const pMessageStrings[ ShadowMessageTypeMaxNum ] =
    {
        SHADOW_OP_GET_ACCEPTED,
        SHADOW_OP_GET_REJECTED,
        SHADOW_OP_DELETE_ACCEPTED,
        SHADOW_OP_DELETE_REJECTED,
        SHADOW_OP_UPDATE_ACCEPTED,
        SHADOW_OP_UPDATE_REJECTED,
        SHADOW_OP_UPDATE_DOCUMENTS,
        SHADOW_OP_UPDATE_DELTA
    };

    /* Lookup table for Shadow message string length. */
    static const uint16_t pMessageStringsLength[ ShadowMessageTypeMaxNum ] =
    {
        SHADOW_OP_GET_ACCEPTED_LENGTH,
        SHADOW_OP_GET_REJECTED_LENGTH,
        SHADOW_OP_DELETE_ACCEPTED_LENGTH,
        SHADOW_OP_DELETE_REJECTED_LENGTH,
        SHADOW_OP_UPDATE_ACCEPTED_LENGTH,
        SHADOW_OP_UPDATE_REJECTED_LENGTH,
        SHADOW_OP_UPDATE_DOCUMENTS_LENGTH,
        SHADOW_OP_UPDATE_DELTA_LENGTH
    };

    /* Lookup table for Shadow message types. */
    static const ShadowMessageType_t pMessageTypes[ ShadowMessageTypeMaxNum ] =
    {
        ShadowMessageTypeGetAccepted,
        ShadowMessageTypeGetRejected,
        ShadowMessageTypeDeleteAccepted,
        ShadowMessageTypeDeleteRejected,
        ShadowMessageTypeUpdateAccepted,
        ShadowMessageTypeUpdateRejected,
        ShadowMessageTypeUpdateDocuments,
        ShadowMessageTypeUpdateDelta
    };

    for( ; index < ( uint32_t ) ( sizeof( pMessageStrings ) / sizeof( pMessageStrings[ 0 ] ) ); index++ )
    {
        returnStatus = containsSubString( pString,
                                          stringLength,
                                          pMessageStrings[ index ],
                                          pMessageStringsLength[ index ] );

        /* If the operation string matches, there must not be any other extra
         * character remaining in the string. */
        if( returnStatus == SHADOW_SUCCESS )
        {
            if( stringLength != pMessageStringsLength[ index ] )
            {
                returnStatus = SHADOW_FAIL;
            }
            else
            {
                *pMessageType = pMessageTypes[ index ];
                break;
            }
        }
    }

    if( returnStatus != SHADOW_SUCCESS )
    {
        LogDebug( ( "Not related to Shadow, failed to match shadow message type in pString %s.", pString ) );
    }

    return returnStatus;
}

/*-----------------------------------------------------------*/

static const char * getShadowOperationString( ShadowTopicStringType_t topicType )
{
    const char * shadowOperationString = NULL;

    switch( topicType )
    {
        case ShadowTopicStringTypeGet:
            shadowOperationString = SHADOW_OP_GET;
            break;

        case ShadowTopicStringTypeGetAccepted:
            shadowOperationString = SHADOW_OP_GET_ACCEPTED;
            break;

        case ShadowTopicStringTypeGetRejected:
            shadowOperationString = SHADOW_OP_GET_REJECTED;
            break;

        case ShadowTopicStringTypeDelete:
            shadowOperationString = SHADOW_OP_DELETE;
            break;

        case ShadowTopicStringTypeDeleteAccepted:
            shadowOperationString = SHADOW_OP_DELETE_ACCEPTED;
            break;

        case ShadowTopicStringTypeDeleteRejected:
            shadowOperationString = SHADOW_OP_DELETE_REJECTED;
            break;

        case ShadowTopicStringTypeUpdate:
            shadowOperationString = SHADOW_OP_UPDATE;
            break;

        case ShadowTopicStringTypeUpdateAccepted:
            shadowOperationString = SHADOW_OP_UPDATE_ACCEPTED;
            break;

        case ShadowTopicStringTypeUpdateRejected:
            shadowOperationString = SHADOW_OP_UPDATE_REJECTED;
            break;

        case ShadowTopicStringTypeUpdateDocuments:
            shadowOperationString = SHADOW_OP_UPDATE_DOCUMENTS;
            break;

        case ShadowTopicStringTypeUpdateDelta:
        /* topicType >= ShadowTopicStringTypeMaxNum check is covered at entry of Shadow_AssembleTopicString. */
        default:
            shadowOperationString = SHADOW_OP_UPDATE_DELTA;
            break;
    }

    return shadowOperationString;
}

/*-----------------------------------------------------------*/

static uint16_t getShadowOperationLength( ShadowTopicStringType_t topicType )
{
    uint16_t shadowOperationLength = 0U;

    switch( topicType )
    {
        case ShadowTopicStringTypeGet:
            shadowOperationLength = SHADOW_OP_GET_LENGTH;
            break;

        case ShadowTopicStringTypeGetAccepted:
            shadowOperationLength = SHADOW_OP_GET_ACCEPTED_LENGTH;
            break;

        case ShadowTopicStringTypeGetRejected:
            shadowOperationLength = SHADOW_OP_GET_REJECTED_LENGTH;
            break;

        case ShadowTopicStringTypeDelete:
            shadowOperationLength = SHADOW_OP_DELETE_LENGTH;
            break;

        case ShadowTopicStringTypeDeleteAccepted:
            shadowOperationLength = SHADOW_OP_DELETE_ACCEPTED_LENGTH;
            break;

        case ShadowTopicStringTypeDeleteRejected:
            shadowOperationLength = SHADOW_OP_DELETE_REJECTED_LENGTH;
            break;

        case ShadowTopicStringTypeUpdate:
            shadowOperationLength = SHADOW_OP_UPDATE_LENGTH;
            break;

        case ShadowTopicStringTypeUpdateAccepted:
            shadowOperationLength = SHADOW_OP_UPDATE_ACCEPTED_LENGTH;
            break;

        case ShadowTopicStringTypeUpdateRejected:
            shadowOperationLength = SHADOW_OP_UPDATE_REJECTED_LENGTH;
            break;

        case ShadowTopicStringTypeUpdateDocuments:
            shadowOperationLength = SHADOW_OP_UPDATE_DOCUMENTS_LENGTH;
            break;

        case ShadowTopicStringTypeUpdateDelta:
        /* topicType >= ShadowTopicStringTypeMaxNum check is covered at entry of Shadow_AssembleTopicString. */
        default:
            shadowOperationLength = SHADOW_OP_UPDATE_DELTA_LENGTH;
            break;
    }

    return shadowOperationLength;
}

/*-----------------------------------------------------------*/

static void createShadowTopicString( ShadowTopicStringType_t topicType,
                                     const char * pThingName,
                                     uint8_t thingNameLength,
                                     const char * pShadowName,
                                     uint8_t shadowNameLength,
                                     char * pTopicBuffer )
{
    uint16_t offset = 0U, operationStringLength = 0U;
    const char * pShadowPrefix = SHADOW_PREFIX;
    const char * pOperationString = NULL;
    const char * pClassicShadowRoot = SHADOW_CLASSIC_ROOT;
    const char * pNamedShadowRoot = SHADOW_NAMED_ROOT;

    /* Copy the Shadow topic prefix into the topic buffer. */
    ( void ) memcpy( ( void * ) pTopicBuffer,
                     ( const void * ) pShadowPrefix,
                     ( size_t ) SHADOW_PREFIX_LENGTH );
    offset = ( uint16_t ) ( offset + SHADOW_PREFIX_LENGTH );

    /* Copy the Thing Name into the topic buffer. */
    ( void ) memcpy( ( void * ) &( pTopicBuffer[ offset ] ),
                     ( const void * ) pThingName,
                     ( size_t ) thingNameLength );
    offset = ( uint16_t ) ( offset + thingNameLength );

    /* Are we assembling a named shadow? */
    if( shadowNameLength > 0U )
    {
        /* Copy the named Shadow topic root into the topic buffer. */
        ( void ) memcpy( ( void * ) &( pTopicBuffer[ offset ] ),
                         ( const void * ) pNamedShadowRoot,
                         ( size_t ) SHADOW_NAMED_ROOT_LENGTH );
        offset = ( uint16_t ) ( offset + SHADOW_NAMED_ROOT_LENGTH );

        /* Copy the Shadow Name into the topic buffer. */
        ( void ) memcpy( ( void * ) &( pTopicBuffer[ offset ] ),
                         ( const void * ) pShadowName,
                         ( size_t ) shadowNameLength );
        offset = ( uint16_t ) ( offset + shadowNameLength );
    }
    else
    {
        /* Copy the Classic Shadow topic root into the topic buffer. */
        ( void ) memcpy( ( void * ) &( pTopicBuffer[ offset ] ),
                         ( const void * ) pClassicShadowRoot,
                         ( size_t ) SHADOW_CLASSIC_ROOT_LENGTH );
        offset = ( uint16_t ) ( offset + SHADOW_CLASSIC_ROOT_LENGTH );
    }

    pOperationString = getShadowOperationString( topicType );
    operationStringLength = getShadowOperationLength( topicType );
    /* Copy the Shadow operation string into the topic buffer. */
    ( void ) memcpy( ( void * ) &( pTopicBuffer[ offset ] ),
                     ( const void * ) pOperationString,
                     ( size_t ) operationStringLength );
}

/*-----------------------------------------------------------*/

ShadowStatus_t Shadow_MatchTopicString( const char * pTopic,
                                        uint16_t topicLength,
                                        ShadowMessageType_t * pMessageType,
                                        const char ** pThingName,
                                        uint16_t * pThingNameLength,
                                        const char ** pShadowName,
                                        uint16_t * pShadowNameLength )
{
    uint16_t consumedTopicLength = 0U;
    ShadowStatus_t shadowStatus = SHADOW_SUCCESS;
    uint16_t thingNameLength = 0;
    uint16_t shadowNameLength = 0;

    shadowStatus = validateMatchTopicParameters( pTopic, topicLength, pMessageType );

    /* A shadow topic string takes one of the two forms.
     * Classic shadow:
     *   $aws/things/<thingName>/shadow/<operation>
     *   $aws/things/<thingName>/shadow/<operation>/<suffix>
     * Named shadow:
     *   $aws/things/<thingName>/shadow/name/<shadowName>/<operation>
     *   $aws/things/<thingName>/shadow/name/<shadowName>/<operation>/<suffix>
     *
     * We need to match the following things:
     * 1. Prefix ($aws/things).
     * 2. Thing Name.
     * 3. Classic shadow root (/shadow) OR Named shadow root (/shadow/name) and shadow name
     * 4. Shadow operation and suffix.
     */
    if( shadowStatus == SHADOW_SUCCESS )
    {
        /* First match the prefix. */
        shadowStatus = containsSubString( &( pTopic[ consumedTopicLength ] ),
                                          topicLength - consumedTopicLength,
                                          SHADOW_PREFIX,
                                          SHADOW_PREFIX_LENGTH );

        if( shadowStatus == SHADOW_SUCCESS )
        {
            consumedTopicLength += SHADOW_PREFIX_LENGTH;
        }
        else
        {
            LogDebug( ( "Not related to Shadow, failed to parse shadow topic prefix in pTopic %s.", pTopic ) );
        }
    }

    if( shadowStatus == SHADOW_SUCCESS )
    {
        /* Extract thing name. */
        shadowStatus = validateName( &( pTopic[ consumedTopicLength ] ),
                                     topicLength - consumedTopicLength,
                                     &thingNameLength );

        if( shadowStatus == SHADOW_SUCCESS )
        {
            consumedTopicLength += thingNameLength;
        }
        else
        {
            shadowStatus = SHADOW_THINGNAME_PARSE_FAILED;
            LogDebug( ( "Not related to Shadow, failed to parse thing name in pTopic %s.", pTopic ) );
        }
    }

    if( shadowStatus == SHADOW_SUCCESS )
    {
        shadowStatus = extractShadowRootAndName( pTopic,
                                                 topicLength,
                                                 &consumedTopicLength,
                                                 &shadowNameLength );
    }

    if( shadowStatus == SHADOW_SUCCESS )
    {
        /* Extract shadow message type. */
        shadowStatus = extractShadowMessageType( &( pTopic[ consumedTopicLength ] ),
                                                 topicLength - consumedTopicLength,
                                                 pMessageType );

        if( shadowStatus != SHADOW_SUCCESS )
        {
            shadowStatus = SHADOW_MESSAGE_TYPE_PARSE_FAILED;
            LogDebug( ( "Not related to Shadow, shadow message type is not in pTopic %s, failed to parse shadow message type.", pTopic ) );
        }
    }

    if( shadowStatus == SHADOW_SUCCESS )
    {
        /* Update the out parameters if we successfully matched the topic. */
        if( pThingName != NULL )
        {
            /* Thing name comes after shadow prefix. */
            *pThingName = &( pTopic[ SHADOW_PREFIX_LENGTH ] );
        }

        if( pThingNameLength != NULL )
        {
            *pThingNameLength = thingNameLength;
        }

        if( pShadowName != NULL )
        {
            *pShadowName = &( pTopic[ SHADOW_PREFIX_LENGTH + thingNameLength +
                                      SHADOW_NAMED_ROOT_LENGTH ] );
        }

        if( pShadowNameLength != NULL )
        {
            *pShadowNameLength = shadowNameLength;
        }
    }

    return shadowStatus;
}
/*-----------------------------------------------------------*/
ShadowStatus_t Shadow_AssembleTopicString( ShadowTopicStringType_t topicType,
                                           const char * pThingName,
                                           uint8_t thingNameLength,
                                           const char * pShadowName,
                                           uint8_t shadowNameLength,
                                           char * pTopicBuffer,
                                           uint16_t bufferSize,
                                           uint16_t * pOutLength )
{
    uint16_t generatedTopicStringLength = 0U;
    ShadowStatus_t shadowStatus = SHADOW_SUCCESS;

    if( ( pTopicBuffer == NULL ) ||
        ( pThingName == NULL ) ||
        ( thingNameLength == 0U ) ||
        ( ( pShadowName == NULL ) && ( shadowNameLength > 0U ) ) ||
        ( topicType >= ShadowTopicStringTypeMaxNum ) ||
        ( pOutLength == NULL ) )
    {
        shadowStatus = SHADOW_BAD_PARAMETER;
        LogError( ( "Invalid input parameters pTopicBuffer: %p, pThingName: %p, thingNameLength: %u,\
                    pShadowName: %p, shadowNameLength: %u, topicType: %d, pOutLength: %p.",
                    ( void * ) pTopicBuffer,
                    ( void * ) pThingName,
                    ( unsigned int ) thingNameLength,
                    ( void * ) pShadowName,
                    ( unsigned int ) shadowNameLength,
                    topicType,
                    ( void * ) pOutLength ) );
    }
    else
    {
        generatedTopicStringLength = SHADOW_PREFIX_LENGTH +        /* Prefix ("$aws/things/"). */
                                     thingNameLength +             /* Thing name. */
                                     ( ( shadowNameLength > 0U ) ? /* Handle named or classic shadow */
                                       ( SHADOW_NAMED_ROOT_LENGTH + shadowNameLength ) :
                                       SHADOW_CLASSIC_ROOT_LENGTH ) +
                                     getShadowOperationLength( topicType ); /* Shadow operation. */

        if( bufferSize < generatedTopicStringLength )
        {
            shadowStatus = SHADOW_BUFFER_TOO_SMALL;
            LogError( ( "Input bufferSize too small, bufferSize %u, required %u.",
                        ( unsigned int ) bufferSize,
                        ( unsigned int ) generatedTopicStringLength ) );
        }
        else
        {
            /* With everything validated, now create the topic string */
            createShadowTopicString( topicType,
                                     pThingName,
                                     thingNameLength,
                                     pShadowName,
                                     shadowNameLength,
                                     pTopicBuffer );

            /* Return the generated topic string length to the caller. */
            *pOutLength = generatedTopicStringLength;
        }
    }

    return shadowStatus;
}
/*-----------------------------------------------------------*/
