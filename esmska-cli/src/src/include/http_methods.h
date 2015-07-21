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

#ifndef HTTP_METHODS_H
#define HTTP_METHODS_H

#include <curl/curl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <common.h>

// V pripade chyby vrati NULL
HttpHandle_t *httpInit();

void httpCleanUp(HttpHandle_t *Handle);

uint8_t httpGet(HttpHandle_t *Handle, const char *HttpAddress, Buffer_t *DestinationStruct);

uint8_t httpPost(HttpHandle_t *Handle, const char *HttpAddress, const char *PostData, Buffer_t *DestinationStruct);

#endif
