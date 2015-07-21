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

#include <stdint.h>
#include <stdio.h>
#include <common.h>
#include <duktape.h>
#include <main_function.h>
#include <parameter_handler.h>

HttpHandle_t *Handle;
SmsProblem_t OccuredProblem = {NULL, NO_ERROR};
Output_t OutputMessages = {0};

void printOccuredProblem()
{
    extern SmsProblem_t OccuredProblem;
    extern const char const *ErrorMessages[];
    
    if ( (OccuredProblem.ProblemType < CUSTOM_MESSAGE) || (OccuredProblem.ProblemType > WRONG_SIGNATURE) )
    {
        fprintf(stderr, "%s\n", ErrorMessages[UNKNOWN] );
        return;
    }
    
    if (OccuredProblem.Msg) fprintf(stderr, "%s %s\n", ErrorMessages[OccuredProblem.ProblemType], OccuredProblem.Msg);
    else fprintf(stderr, "%s\n", ErrorMessages[OccuredProblem.ProblemType] );
}

int main(int argc, char *argv[])
{
    extern const char const *ErrorMessages[];
    InputParameters_t Parameters = {0};
    duk_context *JsContext;
    
    switch (parameterHandler(argc, argv, &Parameters) )
    {
        case DUPLICATE_PARAMETER:
            fprintf(stderr, "Error: duplicate parameter '%s'\n", argv[Parameters.Fault]);
            return PARAMETER_ERROR;
        break;
        
        case NOT_ENOUGH_PARAMETERS:
            fprintf(stderr, "%s\n", ErrorMessages[TOO_FEW_PARAMETERS] ); // muzu nechat jen v mainu?
            return PARAMETER_ERROR;
        break;
        
        case UNKNOWN_PARAMETER:
            fprintf(stderr, "Error: unknown parameter '%s'\n", argv[Parameters.Fault]);
            return PARAMETER_ERROR;
        break;
        
        case ALRIGHT:
            if (Parameters.Help) return EXIT_SUCCESS;
        break;
    }
    
    // Zkontrolovani pritomnosti parametru
    if (!Parameters.Gateway)
    {
        fprintf(stderr, "%s\n", ErrorMessages[NO_GATEWAY] );
        return PARAMETER_ERROR;
    }
    
    if (!Parameters.Number)
    {
        fprintf(stderr, "%s\n", ErrorMessages[NO_NUMBER] );
        return PARAMETER_ERROR;
    }
    
    if (!Parameters.Text)
    {
        fprintf(stderr, "%s\n", ErrorMessages[NO_SMS] );
        return PARAMETER_ERROR;
    }
    
    if ( !(JsContext = duk_create_heap_default() ) )
    {
        fprintf(stderr, "%s\n", ErrorMessages[NO_JS_CONTEXT] );
        return DUKTAPE_ERROR;
    }
    
    int8_t RetCode = sendSms(JsContext, &Parameters, argv);
    
    if (RetCode == GATEWAY_ERROR) printOccuredProblem();
    
    uint8_t i = 0;
    while (i < OutputMessages.NextFree)
    {
        if (OutputMessages.MessagesArray[i].MsgType) // ERR
        {
            do {
                fprintf(stderr, "%s ", OutputMessages.MessagesArray[i].Msg);
            } while (!OutputMessages.MessagesArray[i++].IsEndOfMsg);
            fprintf(stderr, "\n");
        }
        else printf("Info:%s\n", OutputMessages.MessagesArray[i++].Msg);
    }
    
    duk_destroy_heap(JsContext);
    
    return RetCode;
}
