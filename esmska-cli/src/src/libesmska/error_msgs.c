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

const char const *ErrorMessages[] = {
    // Chyby brany
    "Error while operating with the gateway:",
    "An error occured, fix for this gateway is being worked on. More info:",
    "Gateway returned an error message:",
    "Error: SMS quota reached or cooldown period didn't expire yet",
    "Error: The message is too long",
    "Error: you don't have sufficient credit to send this message",
    "Error: sending failed, but no reason was given",
    "Error: sender name is missing",
    "An uknown error occured",
    "Error: gateway currently unsuable. More info:",
    "Error: wrong login or password",
    "Error: the recipient number isn't correct",
    "Error: sender name or number isn't correct",
    // Chyby programu
    "Error: not enough parameters given",
    "Error: no gateway specified",
    "Error: phone number not specified",
    "Error: sms text not specified",
    "Error: couldn't create JavaScript context",
    "Error: not enough memory",
    "Error while proccesing gateway module",
    "Error while setting RECEIPT variable:",
    "Error: couldn't create CURL context",
    "Error: no post data given when calling postURL() method",
    "Error: no explaining message when setting problem given",
    "Error: argument passed to sleep() method is not a number",
    "Warning: send() function didn't return a boolean value; assuming number",
    "Error: ",
    "Error: odd number of URL parameters to postURL() or getUrl() given",
    "Error: odd number of POST parameters in postURL() function"
};
