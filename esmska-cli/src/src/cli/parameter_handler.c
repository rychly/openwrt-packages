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

#include <parameter_handler.h>

uint8_t parameterHandler(int argc, char *argv[], InputParameters_t *Parameters)
{
    uint8_t i = 1;
    
    if ( (argc == 1) || (!(argc % 2) && !(argc == 2) ) ) return NOT_ENOUGH_PARAMETERS;
    
    while (i < argc)
    {
        if (!strncmp("-h", argv[i], 2) )
        {
            printHelp();
            Parameters->Help = true;
            return ALRIGHT;
        }
        else if (!strcmp("-u", argv[i]) )
        {
            if (Parameters->User)
            {
                Parameters->Fault = i;
                return DUPLICATE_PARAMETER;
            }
            
            if ( (i + 1) < argc)
            {
                Parameters->User = i + 1;
                i += 2;
            }
            else return NOT_ENOUGH_PARAMETERS;
        }
        else if (!strcmp("-p", argv[i]) )
        {
            if (Parameters->Password)
            {
                Parameters->Fault = i;
                return DUPLICATE_PARAMETER;
            }
            
            if ( (i + 1) < argc)
            {
                Parameters->Password = i + 1;
                i += 2;
            }
            else return NOT_ENOUGH_PARAMETERS;
        }
        else if (!strcmp("-n", argv[i]) )
        {
            if (Parameters->Number)
            {
                Parameters->Fault = i;
                return DUPLICATE_PARAMETER;
            }
            
            if ( (i + 1) < argc)
            {
                Parameters->Number = i + 1;
                i += 2;
            }
            else return NOT_ENOUGH_PARAMETERS;
        }
        else if (!strcmp("-t", argv[i]) )
        {
            if (Parameters->Text)
            {
                Parameters->Fault = i;
                return DUPLICATE_PARAMETER;
            }
            
            if ( (i + 1) < argc)
            {
                Parameters->Text = i + 1;
                i += 2;
            }
            else return NOT_ENOUGH_PARAMETERS;
        }
        else if (!strcmp("-g", argv[i]) )
        {
            if (Parameters->Gateway)
            {
                Parameters->Fault = i;
                return DUPLICATE_PARAMETER;
            }
            
            if ( (i + 1) < argc)
            {
                Parameters->Gateway = i + 1;
                i += 2;
            }
            else return NOT_ENOUGH_PARAMETERS;
        }
        else if (!strcmp("-s", argv[i]) )
        {
            if (Parameters->SenderNumber)
            {
                Parameters->Fault = i;
                return DUPLICATE_PARAMETER;
            }
            
            if ( (i + 1) < argc)
            {
                Parameters->SenderNumber = i + 1;
                i += 2;
            }
            else return NOT_ENOUGH_PARAMETERS;
        }
        else if (!strcmp("-l", argv[i]) )
        {
            if (Parameters->GatewayLocation)
            {
                Parameters->Fault = i;
                return DUPLICATE_PARAMETER;
            }
            
            if ( (i + 1) < argc)
            {
                Parameters->GatewayLocation = i + 1;
                i += 2;
            }
            else return NOT_ENOUGH_PARAMETERS;
        }
        else
        {
            Parameters->Fault = i;
            return UNKNOWN_PARAMETER;
        }
    }
    
    return ALRIGHT;
}

void printHelp()
{
    puts("An application for sending SMS messages via web gateways (CLI version)\nusage: esmska-cli options\nRequired options:");
    puts("    -g gateway_name\n\tuses the gateway_name.gateway module for sending the message");
    puts("    -n cellnumber\n\tcellphone number of the recipient of the message\n    -t message\n\tactual text of the SMS message");
    puts("Options not required by all gateways:\n    -u user\n\tgateway username");
    puts("    -p password\n\tgateway password\n    -s sendernumber\n\tcellphone number of the sender of the message");
    puts("Other options:\n    -l location\n\tspcifies path to a directory where the chosen module is located,\n\toverriding the default path /usr/share/esmska");
    puts("    -h\n\tprints this help");
}
