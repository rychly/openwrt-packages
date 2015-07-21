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

#include <http_tools.h>

uint8_t getOperation(char *HttpRequest)
{
    uint8_t i = 0;
    while (HttpRequest[i++] != ' ');
    
    return parseMethod(HttpRequest) * 10 + parsePath(HttpRequest + i);
}

uint8_t parseMethod(char *Method)
{
    if (!strncmp(Method, "PO", 2) ) return POST;
    /*
    if (!strncmp(Method, "G", 1) ) return GET;
    
    if (!strncmp(Method, "PU", 2) ) return PUT;
    
    if (!strncmp(Method, "D", 1) ) return DELETE;
    
    if (!strncmp(Method, "PA", 2) ) return PATCH;
    
    if (!strncmp(Method, "O", 1) ) return OPTIONS;
    */
    return UNKNOWN_METHOD;
}

uint8_t parsePath(char *Path)
{
    if (!strncmp(Path, "/esmska/send", 12) && (Path[12] == ' ') ) return ESMSKA_SEND;
    
    return UNKNOWN_PATH;
}

void parseJson(char *Json, char *argv_fake[], InputParameters_t *Parameters)
{
    bool BackSlash = false;
    
    for (;;)
    {
        while ( (*(Json++) != '"') && (*(Json - 1) != '}') );
        if ( *(Json - 1) == '}') return;
        
        Attribute_t AttributeType = getAttribute(Json);
        
        while (*(Json++) != '"'); // v nazvu atributu '"' nebude; po konec nazvu
        while (*(Json++) != '"'); // zacatek hodnoty atributu
        
        if (AttributeType) setAttributeValue(Parameters, Json, AttributeType, argv_fake);
        
        // Nalezeni koncove '"' a nahrazeni za '\0'
        while ( (*Json != '"') && (!BackSlash) )
        {
            BackSlash = false;
            
            if (*(Json++) == '\\') BackSlash = true;
        }
        *(Json++) = '\0';
    }
}

void setAttributeValue(InputParameters_t *Parameters, char *Json, Attribute_t AttributeType, char *argv_fake[] )
{
    uint8_t i = 1;
    while(argv_fake[i] ) i++; // nalezeni volneho mista
    argv_fake[i] = Json;
    
    switch (AttributeType)
    {
        case NUMBER:
            Parameters->Number = i;
        return;
        
        case USER:
            Parameters->User = i;
        return;
        
        case PASSWORD:
            Parameters->Password = i;
        return;
        
        case GATEWAY:
            Parameters->Gateway = i;
        return;
        
        case TEXT:
            Parameters->Text = i;
        return;
        
        case SENDERNUMBER:
            Parameters->SenderNumber = i;
        return;
        
        default:
        return;
    }
}

uint8_t getAttribute(char *Attribute)
{
    if (!strncmp(Attribute, "NUMBER\"", 7) ) return NUMBER;
    
    if (!strncmp(Attribute, "USER\"", 5) ) return USER;
    
    if (!strncmp(Attribute, "PASSWORD\"", 9) ) return PASSWORD;
    
    if (!strncmp(Attribute, "GATEWAY\"", 8) ) return GATEWAY;
    
    if (!strncmp(Attribute, "TEXT\"", 5) ) return TEXT;
    
    if (!strncmp(Attribute, "SENDERNUMBER\"", 13) ) return SENDERNUMBER;
    
    return UNKNOWN_ATTRIBUTE;
}

uint16_t buildResponse(char *OutputBuffer, char *HttpCode, int8_t RetCode, bool HeaderOnly)
{
    extern const char const *ErrorMessages[];
    extern SmsProblem_t OccuredProblem;
    extern Output_t OutputMessages;
    uint8_t HeaderEnd;
    uint16_t Index = HeaderEnd = sprintf(OutputBuffer, "HTTP/1.1 %s\r\nContent-Type: application/json\r\nContent-Length:     \r\n\r\n", HttpCode);
    uint8_t ContentLenghtIndex = Index - 8;
    uint16_t CharsRemaining = 1017;
    uint16_t Tmp;
    
    
    if (HeaderOnly)
    {
        OutputBuffer[ContentLenghtIndex] = '0';
        return Index;
    }
    
    Tmp = snprintf(OutputBuffer + Index, CharsRemaining, "{\"RETCODE\":%d,", RetCode);
    Index += Tmp;
    CharsRemaining -= Tmp;
    
    if (RetCode == GATEWAY_ERROR)
    {
        Tmp = snprintf(OutputBuffer + Index, CharsRemaining, "\"ERROR\":\"");
        Index += Tmp;
        CharsRemaining -= Tmp;
        
        if ( (OccuredProblem.ProblemType < CUSTOM_MESSAGE) || (OccuredProblem.ProblemType > WRONG_SIGNATURE) )
        {
            Tmp = snprintf(OutputBuffer + Index, CharsRemaining, "%s\",", ErrorMessages[UNKNOWN] );
            Index += Tmp;
            CharsRemaining -= Tmp;
        }
        else
        {
            if (OccuredProblem.Msg) Tmp = snprintf(OutputBuffer + Index, CharsRemaining, "%s %s\",", ErrorMessages[OccuredProblem.ProblemType], OccuredProblem.Msg);
            else Tmp = snprintf(OutputBuffer + Index, CharsRemaining, "%s\",", ErrorMessages[OccuredProblem.ProblemType] );
            Index += Tmp;
            CharsRemaining -= Tmp;
        }
    }
    
    uint8_t i = 0;
    while (i < OutputMessages.NextFree)
    {
        if (OutputMessages.MessagesArray[i].MsgType) // ERR
        {
            Tmp = snprintf(OutputBuffer + Index, CharsRemaining, "\"ERROR\":\"");
            Index += Tmp;
            CharsRemaining -= Tmp;
            do {
                Tmp = snprintf(OutputBuffer + Index, CharsRemaining, "%s ", OutputMessages.MessagesArray[i].Msg);
                Index += Tmp;
                CharsRemaining -= Tmp;
                fprintf(stderr, "%s ", OutputMessages.MessagesArray[i].Msg);
            } while (!OutputMessages.MessagesArray[i++].IsEndOfMsg);
            fprintf(stderr, "\n");
            OutputBuffer[Index - 1] = '"';
            OutputBuffer[Index++] = ',';
        }
        else
        {
            Tmp = snprintf(OutputBuffer + Index, CharsRemaining, "\"INFO\":\"%s\",", OutputMessages.MessagesArray[i++].Msg);
            Index += Tmp;
            CharsRemaining -= Tmp;
        }
    }
    
    OutputBuffer[Index - 1] = '}';
    // misto pro Content-Length bude vzdy
    OutputBuffer[ContentLenghtIndex + sprintf(OutputBuffer + ContentLenghtIndex, "%d", Index - HeaderEnd)] = ' ';
    
    return Index;
}

#ifndef _SUPER_SERVER_DAEMON
    int8_t handleRequest(int SocketDescriptor_Response)
#else
    int8_t handleRequest()
#endif
{
    extern const char const *ErrorMessages[];
    extern Output_t OutputMessages;
    char *Json;
    
    char *Buffer = calloc(1024, 1);
    if (!Buffer)
    {
        fprintf(stderr, "%s\n", ErrorMessages[MEMORY] );
        
        #ifndef _SUPER_SERVER_DAEMON
            close(SocketDescriptor_Response);
        #endif
        
        return INTERNAL_ERROR;
    }
    
    char *OutputBuffer = calloc(1024, 1);
    if (!OutputBuffer)
    {
        fprintf(stderr, "%s\n", ErrorMessages[MEMORY] );
        
        #ifndef _SUPER_SERVER_DAEMON
            close(SocketDescriptor_Response);
        #endif
        
        free(Buffer);
        return INTERNAL_ERROR;
    }
    
    // zkontrolovat, ze uz neni nic ke cteni a tudiz se to vsechno vlezlo
    int16_t MsgSize;
    #ifndef _SUPER_SERVER_DAEMON
        if ( (MsgSize = recv(SocketDescriptor_Response, Buffer, 1023, 0) ) < 1)
        {
            perror("Error: couldn't receive request");
            close (SocketDescriptor_Response);
            free(Buffer);
            free(OutputBuffer);
            
            return INTERNAL_ERROR;
        }
    #else
        MsgSize = read(stdin, Buffer, 1023);
    #endif
    
    if (Buffer[MsgSize - 1] != '}')
    {
        #ifndef _SUPER_SERVER_DAEMON
            send(SocketDescriptor_Response, OutputBuffer, buildResponse(OutputBuffer, "200 OK", REQUEST_TOO_LONG, false), 0);
            close(SocketDescriptor_Response);
        #else
            write(stdout, OutputBuffer, buildResponse(OutputBuffer, "200 OK", REQUEST_TOO_LONG, false) );
        #endif
        
        fprintf(stderr, "Error: incoming request too long, closing connection\n");
        
        free(Buffer);
        free(OutputBuffer);
        
        return REQUEST_TOO_LONG;
    }
    
    uint8_t Operation = getOperation(Buffer);
    char *argv_fake[7] = {0};
    InputParameters_t Parameters = {0};
    
    switch (Operation)
    {
        case SEND_SMS:
            Json = strstr(Buffer, "\r\n\r\n") + 4;
            
            #ifdef _CHECK_REQUEST_SOURCE
                if (!Json)
                {
                    fprintf(stderr, "Error: received a non-HTTP request, closing connection\n");
                    
                    free(Buffer);
                    free(OutputBuffer);
                    
                    #ifndef _SUPER_SERVER_DAEMON
                        close(SocketDescriptor_Response);
                    #endif
                    
                    return INTERNAL_ERROR;
                }
            #endif
            
            parseJson(Json, argv_fake, &Parameters);
            
            #ifdef _CHECK_JSON
                if ( (!Parameters.Number) || (!Parameters.Gateway) || (!Parameters.Text) )
                {
                    setErrorMsg(ErrorMessages[TOO_FEW_PARAMETERS], true);
                    
                    #ifndef _SUPER_SERVER_DAEMON
                        send(SocketDescriptor_Response, OutputBuffer, buildResponse(OutputBuffer, "200 OK", PARAMETER_ERROR, false), 0);
                        close(SocketDescriptor_Response);
                    #else
                        write(stdout, OutputBuffer, buildResponse(OutputBuffer, "200 OK", PARAMETER_ERROR, false) );
                    #endif
                    
                    free(Buffer);
                    free(OutputBuffer);
                    
                    return EXIT_SUCCESS;
                }
            #endif
            
            duk_context *JsContext = duk_create_heap_default();
            if (!JsContext)
            {
                setErrorMsg(ErrorMessages[NO_JS_CONTEXT], true);
                
                #ifndef _SUPER_SERVER_DAEMON
                    send(SocketDescriptor_Response, OutputBuffer, buildResponse(OutputBuffer, "200 OK", DUKTAPE_ERROR, false), 0);
                    close(SocketDescriptor_Response);
                #else
                    write(stdout, OutputBuffer, buildResponse(OutputBuffer, "200 OK", DUKTAPE_ERROR, false) );
                #endif
                
                free(Buffer);
                free(OutputBuffer);
                
                return DUKTAPE_ERROR;
            }
            
            int8_t RetCode = sendSms(JsContext, &Parameters, argv_fake);
            
            // poslani RetCode a odpovedi
            #ifndef _SUPER_SERVER_DAEMON
                send(SocketDescriptor_Response, OutputBuffer, buildResponse(OutputBuffer, "200 OK", RetCode, false), 0);
            #else
                write(stdout, OutputBuffer, buildResponse(OutputBuffer, "200 OK", RetCode, false) );
            #endif

            duk_destroy_heap(JsContext);
        break;
        
        default:
            #ifndef _SUPER_SERVER_DAEMON
                send(SocketDescriptor_Response, OutputBuffer, buildResponse(OutputBuffer, "400 Bad Request", 0, true), 0);
            #else
                write(stdout, OutputBuffer, buildResponse(OutputBuffer, "400 Bad Request", 0, true) );
            #endif
        break;
    }
    
    free(Buffer);
    free(OutputBuffer);
    
    #ifndef _SUPER_SERVER_DAEMON
        close(SocketDescriptor_Response);
    #endif
    
    return EXIT_SUCCESS;
}
