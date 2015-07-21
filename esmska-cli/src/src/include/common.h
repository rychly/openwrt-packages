/*
 * This file is part of esmska-cli.
 * Copyright (c) 2015 by Dominik Marton <sino.include@gmail.com>.
 * 
 * esmska-cli is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * esmska-cli is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with esmska-cli.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef COMMON_H
#define COMMON_H

#include <curl/curl.h>
#include <stdbool.h>
#include <stdint.h>

/* ERROR_MSGS */

typedef enum {
    TOO_FEW_PARAMETERS = 13,
    NO_GATEWAY,
    NO_NUMBER,
    NO_SMS,
    NO_JS_CONTEXT,
    MEMORY,
    MODULE_PROCESSING,
    RECEIPT_SETTING,
    NO_CURL_CONTEXT,
    POST_DATA,
    NO_EXPLAINING,
    SLEEP_NUMBER,
    NOT_BOOL,
    GENERIC,
    URL_PARAMETERS,
    POST_PARAMETERS
} ErrorMessagesIndex_t;

/* PARAMETER_HANDLER */

typedef struct {
    bool Help;
    uint8_t User;
    uint8_t Password;
    uint8_t Number;
    uint8_t Text;
    uint8_t Gateway;
    uint8_t SenderNumber;
    uint8_t Fault;
    uint8_t GatewayLocation;
} InputParameters_t;

typedef enum {
    ALRIGHT,
    DUPLICATE_PARAMETER,
    NOT_ENOUGH_PARAMETERS,
    UNKNOWN_PARAMETER
} ParameterHandelerRetValues_t;

/* EXEC_OBJECT */

typedef enum {
    NO_ERROR = -1,
    /** Gateway script author provided his own message.
     * Requires the message as a parameter (you can use HTML 3.2). */
    CUSTOM_MESSAGE,
    /** A fix for this gateway is being worked on.
     * Requires URL for a webpage with more details as a parameter. */
    FIX_IN_PROGRESS,
    /** Gateway provided its own error message. 
     * Requires gateway message as a parameter. */
    GATEWAY_MESSAGE,
    /** This is used for internal Esmska purposes. Don't use it from
     inside gateway scripts.
     Requires the message as a parameter. */
    //INTERNAL_MESSAGE, - not used?
    /** The user has not waited long enough to send another message
     * or message quota has been reached. */
    LIMIT_REACHED,
    /** The message text was too long. */
    LONG_TEXT,
    /** The user does not have sufficient credit. */
    NO_CREDIT,
    /** The sending failed but gateway hasn't provided any reason for it. */
    NO_REASON,
    /** The sender signature was missing. */
    SIGNATURE_NEEDED,
    /** Message that unknown error happened, maybe error in the script.
    * Make sure you set this problem before making any other HTTP requests */
    UNKNOWN,
    /** This gateway is for some reason currently unusable.
     * Requires URL for a webpage with more details as a parameter. */
    UNUSABLE,
    /** The login or password was wrong. */
    WRONG_AUTH,
    /** The security code was wrong. */
    //WRONG_CODE, - neni treba, captcha
    /** The recepient number was wrong. */
    WRONG_NUMBER,
    /** The sender signature was wrong. */
    WRONG_SIGNATURE
} ProblemType_t;

#define NO_MEMORY 90
#define NO_POST_DATA 91
#define SET_PROBLEM_NO_MSG 92
#define SLEEP_NOT_NUMBER 93
#define ODD_URL_PARAMETERS 94
#define ODD_POST_PARAMETERS 95

typedef struct {
    const char *Msg;
    ProblemType_t ProblemType;
} SmsProblem_t;

/* HTTP_METHODS */

typedef CURL HttpHandle_t;

typedef struct {
    char *Buffer;
    uint32_t ActualSize;
} Buffer_t;

/* MAIN_FUNCTION*/

typedef enum {
    PARAMETER_ERROR = 1,
    MODULE_ERROR,
    CURL_ERROR,
    DUKTAPE_ERROR,
    INTERNAL_ERROR,
    GATEWAY_ERROR
} MainRetValues_t;

typedef enum {
    OUT, ERR
} MessageType_t;

typedef struct {
    MessageType_t MsgType;
    char const *Msg;
    bool IsEndOfMsg;
} Message_t;

typedef struct {
    uint8_t NextFree;
    Message_t MessagesArray[4];
} Output_t;

/* SERVICE */

typedef enum {
    NO_PORT = 1,
    SOCKET_NOT_CREATED,
    BIND_ERROR,
    LISTEN_ERROR,
    REQUEST_TOO_LONG = 7
} ServiceRetValues_t;

#endif
