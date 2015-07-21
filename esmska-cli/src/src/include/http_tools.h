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

#ifndef HTTP_TOOLS_H
#define HTTP_TOOLS_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <common.h>
#include <duktape.h>
#include <main_function.h>
#include <unistd.h>

typedef enum {
    UNKNOWN_METHOD,
    POST,
    GET,
    PUT,
    DELETE,
    PATCH,
    OPTIONS
}HttpMethods_t;

typedef enum {
    UNKNOWN_PATH,
    ESMSKA_SEND
} Resources_t;

typedef enum {
    SEND_SMS = POST * 10 + ESMSKA_SEND
} Operation_t;

typedef enum {
    UNKNOWN_ATTRIBUTE,
    NUMBER,
    USER,
    PASSWORD,
    GATEWAY,
    TEXT,
    SENDERNUMBER
} Attribute_t;

int8_t handleRequest(int SocketDescriptor_Response);

uint8_t getOperation(char *HttpRequest);

uint8_t parseMethod(char *Method);

uint8_t parsePath(char *Path);

void parseJson(char *Json, char *argv_fake[], InputParameters_t *Parameters);

uint8_t getAttribute(char *Attribute);

void setAttributeValue(InputParameters_t *Parameters, char *Json, Attribute_t AttributeType, char *argv_fake[] );

uint16_t buildResponse(char *OutputBuffer, char *HttpCode, int8_t RetCode, bool HeaderOnly);

#endif
