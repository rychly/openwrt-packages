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

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <common.h>
#include <http_tools.h>

SmsProblem_t OccuredProblem = {NULL, NO_ERROR};
HttpHandle_t *Handle;
Output_t OutputMessages = {0};

int main(int argc, char *argv[])
{
    #ifndef _SUPER_SERVER_DAEMON
        signal(SIGCHLD, SIG_IGN); // Ignorovani navratove hodnoty potomku
    
        if (argc < 2)
        {
            fprintf(stderr, "Error: no port specified\n");
            return NO_PORT;
        }

        uint16_t Port = atoi(argv[1] );

        int SocketDescriptor_Reception = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (SocketDescriptor_Reception < 0)
        {
            perror("Error: couldn't create local socket");
            return SOCKET_NOT_CREATED;
        }

        struct sockaddr_in ServiceStruct;
        ServiceStruct.sin_family = AF_INET;
        ServiceStruct.sin_addr.s_addr = INADDR_ANY;
        ServiceStruct.sin_port = htons(Port);

        if (bind(SocketDescriptor_Reception, (struct sockaddr *)&ServiceStruct, sizeof(ServiceStruct) ) < 0)
        {
            fprintf(stderr, "Error: couldn't bind socket to port %d\n", Port);
            close(SocketDescriptor_Reception);
            return BIND_ERROR;
        }

        if (listen(SocketDescriptor_Reception, SOMAXCONN) < 0)
        {
            fprintf(stderr, "Error: couldn't start listening on port %d\n", Port);
            close(SocketDescriptor_Reception);
            return LISTEN_ERROR;
        }
    
        int SocketDescriptor_Response;
        
        for (;;)
        {
            if ( (SocketDescriptor_Response = accept(SocketDescriptor_Reception, NULL, NULL) ) < 0)
            {
                perror("Error: couldn't accept an incoming connection");
                continue;
            }
        
            // Pred zavrenim socketu budou vsechna data ve fronte poslana; timeout 30s
            struct linger SendWait;
            SendWait.l_onoff = 1;
            SendWait.l_linger = 30;
            setsockopt(SocketDescriptor_Response, SOL_SOCKET, SO_LINGER, &SendWait, sizeof(SendWait) );
        
            pid_t Pid = fork();
            if (Pid < 0)
            {
                perror("Error: fork() failed, closing connection");
                close(SocketDescriptor_Response);
                
                continue;
            }
            
            if (Pid) close(SocketDescriptor_Response); // puvodni proces
            else // potomek
            {
                close(SocketDescriptor_Reception);
                
                return handleRequest(SocketDescriptor_Response);
            }
        }
    #else
        return handleRequest();
    #endif
}
