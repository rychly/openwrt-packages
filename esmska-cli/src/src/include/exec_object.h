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

#ifndef EXEC_OBJECT_H
#define EXEC_OBJECT_H

#include <curl/curl.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <common.h>
#include <duktape.h>
#include <http_methods.h>

duk_ret_t post(duk_context *JsContext);

duk_ret_t get(duk_context *JsContext);

duk_ret_t setProblem(duk_context *JsContext);

duk_ret_t setSupplementalMsg(duk_context *JsContext);

duk_ret_t stall(duk_context *JsContext);

duk_ret_t setReferer(duk_context *JsContext);

#endif
