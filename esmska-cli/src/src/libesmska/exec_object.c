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

#include <exec_object.h>

static char *appendParameters(duk_context *JsContext, bool ArePostData, uint8_t AddressIndex, uint8_t ArrayIndex, uint8_t ArrayLength)
{
    char *FinalAddress = NULL;
    char *Tmp;
    const char *OriginalAddress;
    duk_size_t AddressLength = 0;
    
    if (!ArePostData)
    {
        OriginalAddress = duk_get_lstring(JsContext, AddressIndex, &AddressLength);
        
        if ( !(FinalAddress = malloc( (AddressLength += 1) * sizeof(char) ) ) ) return NULL;
        
        memcpy(FinalAddress, OriginalAddress, AddressLength);
        FinalAddress[AddressLength - 1] = '?';
    }
    
    for (uint8_t i = 0; i < ArrayLength; i += 2)
    {
        duk_size_t KeyLength, ValueLength;
        const char *KeyString, *ValueString;
        
        duk_get_prop_index(JsContext, ArrayIndex, i);
        KeyString = duk_to_lstring(JsContext, -1, &KeyLength);
                
        duk_get_prop_index(JsContext, ArrayIndex, i + 1);
        ValueString = duk_to_lstring(JsContext, -1, &ValueLength);
                
        if ( !(Tmp = realloc(FinalAddress, (AddressLength + KeyLength + ValueLength + 2) * sizeof(char) ) ) )
        {
            free(FinalAddress);
            return NULL;
        }
        
        FinalAddress = Tmp;
        
        memcpy(FinalAddress + AddressLength, KeyString, KeyLength);
        FinalAddress[ (AddressLength += (KeyLength + 1) ) - 1] = '=';
        
        memcpy(FinalAddress + AddressLength, ValueString, ValueLength);
        FinalAddress[ (AddressLength += (ValueLength + 1) ) - 1] = '&';
        
        duk_pop_2(JsContext);
    }
    
    FinalAddress[AddressLength - 1] = '\0';
    
    return FinalAddress;
}

duk_ret_t post(duk_context *JsContext)
{
    extern HttpHandle_t *Handle;
    
    uint8_t ArrayLength;
    char *FinalAddress = NULL;
    char *PostData = NULL;
    Buffer_t WebContent = {NULL, 0};
    CURLcode ErrCode;
    
    
    if (duk_is_array(JsContext, 2) && ( (ArrayLength = duk_get_length(JsContext, 2) ) ) )
    {
        if ( (ArrayLength % 2) )
        {
            duk_push_int(JsContext, ODD_POST_PARAMETERS);
            duk_throw(JsContext);
        }
        
       if ( !(PostData = appendParameters(JsContext, true, 0, 2, ArrayLength) ) )
       {
           duk_push_int(JsContext, NO_MEMORY);
           duk_throw(JsContext);
       }
    }
    else
    {
        duk_push_int(JsContext, NO_POST_DATA);
        duk_throw(JsContext);
    }
    
    if (duk_is_array(JsContext, 1) && ( (ArrayLength = duk_get_length(JsContext, 1) ) ) )
    {
        if ( (ArrayLength % 2) )
        {
            duk_push_int(JsContext, ODD_URL_PARAMETERS);
            duk_throw(JsContext);
        }
        
        if ( !(FinalAddress = appendParameters(JsContext, false, 0, 1, ArrayLength) ) )
        {
           duk_push_int(JsContext, NO_MEMORY);
           duk_throw(JsContext);
        }
        
        ErrCode = httpPost(Handle, FinalAddress, PostData, &WebContent);
        free(FinalAddress);
    }
    else ErrCode = httpPost(Handle, duk_get_string(JsContext, 0), PostData, &WebContent);
    
    free(PostData);
    
    if (ErrCode)
    {
        free(WebContent.Buffer);
        
        if (ErrCode == CURLE_WRITE_ERROR) duk_push_int(JsContext, NO_MEMORY);
        else duk_push_int(JsContext, ErrCode);
        
        duk_throw(JsContext);
    }
    
    duk_push_lstring(JsContext, WebContent.Buffer, WebContent.ActualSize);
    free(WebContent.Buffer);
    
    return 1;
}

duk_ret_t get(duk_context *JsContext)
{
    extern HttpHandle_t *Handle;
    
    uint8_t ArrayLength;
    char *FinalAddress = NULL;
    Buffer_t WebContent = {NULL, 0};
    CURLcode ErrCode;
    
    if (duk_is_array(JsContext, 1) && ( (ArrayLength = duk_get_length(JsContext, 1) ) ) )
    {
        if ( (ArrayLength % 2) )
        {
            duk_push_int(JsContext, ODD_URL_PARAMETERS);
            duk_throw(JsContext);
        }
        
        if ( !(FinalAddress = appendParameters(JsContext, false, 0, 1, ArrayLength) ) )
        {
           duk_push_int(JsContext, NO_MEMORY);
           duk_throw(JsContext);
        }
        
        ErrCode = httpGet(Handle, FinalAddress, &WebContent);
        free(FinalAddress);
    }
    else ErrCode = httpGet(Handle, duk_get_string(JsContext, 0), &WebContent);
    
    if (ErrCode)
    {
        free(WebContent.Buffer);
        
        if (ErrCode == CURLE_WRITE_ERROR) duk_push_int(JsContext, NO_MEMORY);
        else duk_push_int(JsContext, ErrCode);
        
        duk_throw(JsContext);
    }
    
    duk_push_lstring(JsContext, WebContent.Buffer, WebContent.ActualSize);
    free(WebContent.Buffer);
    
    return 1;
}

ProblemType_t getProblemType(const char *ErrType)
{
    if (!strncmp(ErrType, "C", 1) ) return CUSTOM_MESSAGE;
    
    if (!strncmp(ErrType, "F", 1) ) return FIX_IN_PROGRESS;
    
    if (!strncmp(ErrType, "G", 1) ) return GATEWAY_MESSAGE;
    
    //if (!strncmp(ErrType, "I", 1) ) return INTERNAL_MESSAGE; - nepouziva se?
    
    if (!strncmp(ErrType, "S", 1) ) return SIGNATURE_NEEDED;
    
    if (!strncmp(ErrType, "LI", 2) ) return LIMIT_REACHED;
    
    if (!strncmp(ErrType, "LO", 2) ) return LONG_TEXT;
    
    if (!strncmp(ErrType, "UNK", 3) ) return UNKNOWN;
    
    if (!strncmp(ErrType, "UNU", 3) ) return UNUSABLE;
    
    if (!strncmp(ErrType, "NO_C", 4) ) return NO_CREDIT;
    
    if (!strncmp(ErrType, "NO_R", 4) ) return NO_REASON;
    
    if (!strncmp(ErrType, "WRONG_A", 7) ) return WRONG_AUTH;
    
    //if (!strncmp(ErrType, "WRONG_C", 7) ) return WRONG_CODE; - neni treba, jen v pripade bran s captcha ochranou
    
    if (!strncmp(ErrType, "WRONG_N", 7) ) return WRONG_NUMBER;
    
    if (!strncmp(ErrType, "WRONG_S", 7) ) return WRONG_SIGNATURE;
    
    return UNKNOWN;
}

duk_ret_t setProblem(duk_context *JsContext)
{
    extern SmsProblem_t OccuredProblem;
    
    if (duk_is_string(JsContext, 0) ) OccuredProblem.ProblemType = getProblemType(duk_get_string(JsContext, 0) );
    else OccuredProblem.ProblemType = duk_to_int(JsContext, 0);
    
    switch (OccuredProblem.ProblemType)
    {
        case CUSTOM_MESSAGE:
        case FIX_IN_PROGRESS:
        case GATEWAY_MESSAGE:
        //case INTERNAL_MESSAGE: - nepouziva se?
        case UNUSABLE:
            if (duk_is_undefined(JsContext, 1) )
            {
                duk_push_int(JsContext, SET_PROBLEM_NO_MSG);
                duk_throw(JsContext);
            }
            else OccuredProblem.Msg = duk_to_string(JsContext, 1);
        break;
        
        default:
        break;
    }
    
    return 0;
}

duk_ret_t setSupplementalMsg(duk_context *JsContext)
{
    extern Output_t OutputMessages;
    
    OutputMessages.MessagesArray[OutputMessages.NextFree].MsgType = OUT;
    OutputMessages.MessagesArray[OutputMessages.NextFree].IsEndOfMsg = true;
    OutputMessages.MessagesArray[OutputMessages.NextFree++].Msg = duk_to_string(JsContext, 0);
    
    return 0;
}

duk_ret_t stall(duk_context *JsContext)
{
    if (!duk_is_number(JsContext, 0) )
    {
        duk_push_int(JsContext, SLEEP_NOT_NUMBER);
        duk_throw(JsContext);
    }
    
    usleep( (int) duk_get_number(JsContext, 0) * 1000);
    
    return 0;
}

duk_ret_t setReferer(duk_context *JsContext)
{
    extern HttpHandle_t *Handle;
    
    CURLcode ErrCode = curl_easy_setopt(Handle, CURLOPT_REFERER, duk_to_string(JsContext, 0) ); // staci duk_get_string rekl bych
    
    if (ErrCode)
    {
        duk_push_int(JsContext, ErrCode);
        duk_throw(JsContext);
    }
    
    return 0;
}
