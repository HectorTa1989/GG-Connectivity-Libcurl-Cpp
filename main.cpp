#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <curl/curl.h>

struct Mem {
  char *memory;
  size_t size;
};

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct Mem *mem = (struct Mem *)userp;

    char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
      /* out of memory! */
      printf("not enough memory (realloc returned NULL)\n");
      return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main(int argc, char * argv[])
{
    CURL* curl;
    CURLcode res;
    
    struct Mem chunk;
    chunk.memory = NULL;  /* will be grown as needed by the realloc above */
    chunk.size = 0;       /* no data at this point */

    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    struct curl_slist *LibcurlHeaders = NULL;
    if(curl)
    {
        
        double TotalTime, StartTransferTime, connectTime;
        char *ip;
        long response_code;
        double NamelookupTime;
        
        std::cout << "SKTEST;";

        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L); //The number of seconds to wait while trying to connect.
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); //The maximum number of seconds to allow cURL functions to execute.
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        LibcurlHeaders = curl_slist_append(LibcurlHeaders, "Accept:");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, LibcurlHeaders);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

        /* get it! */
        res = curl_easy_perform(curl);

        /* check for errors */
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
        }
        else
        {
            /* get IP address of last connection */
            if(!curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &ip) && ip)
                std::cout << "<IP address : " << ip << ">;";

            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE,&response_code);
            if(res == CURLE_OK)
                std::cout << "<HTTP response code: " << response_code << ">;";

            res = curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &NamelookupTime);
            if(res == CURLE_OK)
                std::cout << std::fixed << std::setprecision(10) << "Name lookup time: " << NamelookupTime << ">;";
            
            res = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connectTime);
            if(CURLE_OK == res)
                std::cout << "<Connect time: " << connectTime << ">;";
            

            res = curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &StartTransferTime);
            if(CURLE_OK == res)
                std::cout << "<Start tranfer time: " << StartTransferTime << ">;";
            
            res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &TotalTime);
            if(CURLE_OK == res)
                std::cout << "<Total time: " << TotalTime << ">;";
            
            
          /*
           * Now, our chunk.memory points to a memory block that is chunk.size
           * bytes big and contains the remote file.
           *
           * Do something with it!
           */

            std::cout << "<" << (unsigned long)chunk.size << " bytes retrieved>\n";
        }
        curl_easy_cleanup(curl);
        delete chunk.memory;
        curl_slist_free_all(LibcurlHeaders);
        curl_global_cleanup();

    }
    return 0;
}
