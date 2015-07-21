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

#include <http_methods.h>

static size_t writeCallback(signed char *Source, size_t ElementSize, size_t NumberOfElements, void *Destination)
{
    char *Tmp = NULL;
    uint32_t ChunkSize = ElementSize * NumberOfElements;
    Buffer_t *DestinationStruct = (Buffer_t *) Destination;
    
    if (!ChunkSize) return 0; // 0B bylo prijmuto
    
    // V pripade pocatecni varky prenosu je potreba pridat '\0', proto + 1
    if ( (Tmp = realloc(DestinationStruct->Buffer,
    (DestinationStruct->ActualSize ? DestinationStruct->ActualSize + ChunkSize : DestinationStruct->ActualSize + ChunkSize + 1) * sizeof(char) ) ) )
    {
        DestinationStruct->Buffer = Tmp;
    }
    else return 0;
    
    if (DestinationStruct->ActualSize)
    {
        memcpy(DestinationStruct->Buffer + (DestinationStruct->ActualSize - 1), Source, ChunkSize);
        DestinationStruct->ActualSize += ChunkSize;
    }
    else
    {
        memcpy(DestinationStruct->Buffer, Source, ChunkSize);
        DestinationStruct->ActualSize += (ChunkSize + 1);
    }
    
    DestinationStruct->Buffer[DestinationStruct->ActualSize - 1] = '\0';
    
    return ChunkSize;
}

HttpHandle_t *httpInit()
{
    if (curl_global_init(CURL_GLOBAL_SSL) ) return NULL;
    
    HttpHandle_t *Handle = curl_easy_init();
    
    if (Handle)
    {
        curl_easy_setopt(Handle, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(Handle, CURLOPT_COOKIEFILE, "");
        curl_easy_setopt(Handle, CURLOPT_COOKIESESSION, 1);
        curl_easy_setopt(Handle, CURLOPT_WRITEFUNCTION, writeCallback);
    }
    
    return Handle;
}

inline void httpCleanUp(HttpHandle_t *Handle)
{
    curl_easy_cleanup(Handle);
}


uint8_t httpGet(HttpHandle_t *Handle, const char *HttpAddress, Buffer_t *DestinationStruct)
{
    CURLcode ErrCode;
    
    if ( (ErrCode = curl_easy_setopt(Handle, CURLOPT_URL, HttpAddress) ) ) return ErrCode; //error
    curl_easy_setopt(Handle, CURLOPT_WRITEDATA, DestinationStruct);
    curl_easy_setopt(Handle, CURLOPT_HTTPGET, 1);
    
    if ( (ErrCode = curl_easy_perform(Handle) ) ) return ErrCode;
    
    return CURLE_OK;
}

uint8_t httpPost(HttpHandle_t *Handle, const char *HttpAddress, const char *PostData, Buffer_t *DestinationStruct)
{
    CURLcode ErrCode;
    
    if ( (ErrCode = curl_easy_setopt(Handle, CURLOPT_URL, HttpAddress) ) ) return ErrCode;
    curl_easy_setopt(Handle, CURLOPT_WRITEDATA, DestinationStruct);
    curl_easy_setopt(Handle, CURLOPT_POSTFIELDS, PostData);
    
    if ( (ErrCode = curl_easy_perform(Handle) ) ) return ErrCode;
    
    return CURLE_OK;
}
