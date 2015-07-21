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

#include <main_function.h>

int8_t sendSms(duk_context *JsContext, InputParameters_t *Parameters, char *Input[])
{
    extern const char const *ErrorMessages[];
    extern HttpHandle_t *Handle;
    extern Output_t OutputMessages;
    int8_t RetCode = EXIT_SUCCESS;
    
    char *GatewayLocation;
    
    if (Parameters->GatewayLocation)
    {
        uint8_t GatewayLocationLength = strlen(Input[Parameters->GatewayLocation] );
        
        if ( Input[Parameters->GatewayLocation][GatewayLocationLength - 1] == '/')
        {
            Input[Parameters->GatewayLocation][GatewayLocationLength - 1] = '\0';
            GatewayLocationLength--;
        }
        
        // 10 = '/' + ".gateway" + '\0'
        GatewayLocation = malloc(GatewayLocationLength + strlen(Input[Parameters->Gateway] ) + 10);
        
        if (!GatewayLocation)
        {
            setErrorMsg(ErrorMessages[MEMORY], true);
            return NO_MEMORY;
        }
        
        sprintf(GatewayLocation, "%s/%s.gateway",Input[Parameters->GatewayLocation], Input[Parameters->Gateway] );
    }
    else
    {
        // 27 = "/usr/share/esmska/" + ".gateway" + '\0'
        GatewayLocation = malloc(strlen(Input[Parameters->Gateway] ) + 27);
        
        if (!GatewayLocation)
        {
            setErrorMsg(ErrorMessages[MEMORY], true);
            return NO_MEMORY;
        }
        
        sprintf(GatewayLocation, "/usr/share/esmska/%s.gateway", Input[Parameters->Gateway] );
    }
    
    if (duk_peval_file(JsContext, GatewayLocation) )
    {
        duk_get_prop_string(JsContext, -1, "message");
        
        setErrorMsg(ErrorMessages[MODULE_PROCESSING], false);
        setErrorMsg(Input[Parameters->Gateway], false);
        setErrorMsg(duk_get_string(JsContext, -1), true);
        
        free(GatewayLocation);
        return MODULE_ERROR;
    }
    
    free(GatewayLocation);
    duk_pop(JsContext); // Zahozeni vysledku (undefined)
    
    // Zpristupneni potrebnych udaju jako username, passwd, cislo, text zpravy
    duk_push_global_object(JsContext);
    
    if (Parameters->User) duk_push_string(JsContext, Input[Parameters->User] );
    else duk_push_string(JsContext, "");
    duk_put_prop_string(JsContext, -2, "LOGIN");
    
    if (Parameters->Password) duk_push_string(JsContext, Input[Parameters->Password] );
    else duk_push_string(JsContext, "");
    duk_put_prop_string(JsContext, -2, "PASSWORD");
    
    if (Parameters->SenderNumber) duk_push_string(JsContext, Input[Parameters->SenderNumber] );
    else duk_push_string(JsContext, "");
    duk_put_prop_string(JsContext, -2, "SENDERNUMBER");
    
    duk_push_string(JsContext, Input[Parameters->Number] );
    duk_put_prop_string(JsContext, -2, "NUMBER");
    
    duk_push_string(JsContext, Input[Parameters->Text] );
    duk_put_prop_string(JsContext, -2, "MESSAGE");
    
    if (duk_peval_string(JsContext, "RECEIPT = (getFeatures().indexOf('RECEIPT') > -1 ? true : false)") )
    {
        duk_get_prop_string(JsContext, -1, "message");
        
        setErrorMsg(ErrorMessages[RECEIPT_SETTING], false);
        setErrorMsg(duk_get_string(JsContext, -1), true);
        
        return MODULE_ERROR;
    }
    
    duk_pop(JsContext);
    
    // Zpristupneni potrebnych metod, promennych a samotny objekt EXEC
    duk_push_object(JsContext);
    duk_push_c_function(JsContext, get, 2);
    duk_put_prop_string(JsContext, -2, "getURL");
    duk_push_c_function(JsContext, post, 3);
    duk_put_prop_string(JsContext, -2, "postURL");
    duk_push_c_function(JsContext, setProblem, 2);
    duk_put_prop_string(JsContext, -2, "setProblem");
    duk_push_c_function(JsContext, setSupplementalMsg, 1);
    duk_put_prop_string(JsContext, -2, "setSupplementalMessage");
    duk_push_c_function(JsContext, stall, 1);
    duk_put_prop_string(JsContext, -2, "sleep");
    duk_push_c_function(JsContext, setReferer, 1);
    duk_put_prop_string(JsContext, -2, "setReferer");
    
    duk_push_string(JsContext, "Free SMS remaining: ");
    duk_put_prop_string(JsContext, -2, "INFO_FREE_SMS_REMAINING");
    duk_push_string(JsContext, "The gateway doesn't provide any information about successful sending. The message might be and might not be delivered.");
    duk_put_prop_string(JsContext, -2, "INFO_STATUS_NOT_PROVIDED");
    duk_push_string(JsContext, "Remaining credit: ");
    duk_put_prop_string(JsContext, -2, "INFO_CREDIT_REMAINING");
    
    duk_put_prop_string(JsContext, -2, "EXEC");
    
    // Vytvoreni CURL contextu
    if ( !(Handle = httpInit() ) )
    {
        setErrorMsg(ErrorMessages[NO_CURL_CONTEXT], true);
        
        return CURL_ERROR;
    }
    
    // Volani funkce send()
    duk_get_prop_string(JsContext, -1, "send");
    if (duk_pcall(JsContext, 0) )
    {
        if (duk_is_number(JsContext, -1) )
        {
            uint8_t ErrCode = duk_get_number(JsContext, -1);
            
            if (ErrCode < 90)
            {
                setErrorMsg(ErrorMessages[GENERIC], false);
                setErrorMsg(curl_easy_strerror(ErrCode), true);
                
                RetCode = CURL_ERROR;
            }
            else
            {
                OutputMessages.MessagesArray[OutputMessages.NextFree].MsgType = ERR;
                OutputMessages.MessagesArray[OutputMessages.NextFree].IsEndOfMsg = 1;
                
                switch (ErrCode)
                {
                    case NO_MEMORY:
                        OutputMessages.MessagesArray[OutputMessages.NextFree++].Msg = ErrorMessages[MEMORY];
                        RetCode = INTERNAL_ERROR;
                    break;
                    
                    case NO_POST_DATA:
                        OutputMessages.MessagesArray[OutputMessages.NextFree++].Msg = ErrorMessages[POST_DATA];
                        RetCode = MODULE_ERROR;
                    break;
                    
                    case SET_PROBLEM_NO_MSG:
                        OutputMessages.MessagesArray[OutputMessages.NextFree++].Msg = ErrorMessages[NO_EXPLAINING];
                        RetCode = MODULE_ERROR;
                    break;
                    
                    case SLEEP_NOT_NUMBER:
                        OutputMessages.MessagesArray[OutputMessages.NextFree++].Msg = ErrorMessages[SLEEP_NUMBER];
                        RetCode = MODULE_ERROR;
                    break;
                    
                    case ODD_URL_PARAMETERS:
                        OutputMessages.MessagesArray[OutputMessages.NextFree++].Msg = ErrorMessages[URL_PARAMETERS];
                        RetCode = MODULE_ERROR;
                    break;
                    
                    case ODD_POST_PARAMETERS:
                        OutputMessages.MessagesArray[OutputMessages.NextFree++].Msg = ErrorMessages[POST_PARAMETERS];
                        RetCode = MODULE_ERROR;
                    break;
                }
            }
        }
        else
        {
            setErrorMsg(duk_safe_to_string(JsContext, -1), true);
            
            RetCode = DUKTAPE_ERROR;
        }
    }
    else
    {
        bool SendRetVal;
        
        if (duk_is_boolean(JsContext, -1) ) SendRetVal = duk_get_boolean(JsContext, -1);
        else
        {
            setErrorMsg(ErrorMessages[NOT_BOOL], true);
            
            SendRetVal = (bool) ( (int8_t) duk_to_number(JsContext, -1) );
        }
            
        if (!SendRetVal) RetCode = GATEWAY_ERROR;
    }
    
    httpCleanUp(Handle);
    
    return RetCode;
}

void setErrorMsg(const char *Msg, bool IsEndOfMsg)
{
    extern Output_t OutputMessages;
    
    OutputMessages.MessagesArray[OutputMessages.NextFree].MsgType = ERR;
    OutputMessages.MessagesArray[OutputMessages.NextFree].IsEndOfMsg = IsEndOfMsg;
    OutputMessages.MessagesArray[OutputMessages.NextFree++].Msg = Msg;
}
