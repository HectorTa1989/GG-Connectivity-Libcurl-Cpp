# Google connectivity test using Libcurl written in C++

In this repository, I created a C++ library based on libcurl. It includes HTTP get request function and read the responses back from Google website. Then, extract statistics from the responses.


## Program operation and Libcurl API explanation
The requirement is that we must send HTTP get request to Google and read the responses. So, I use the following functions for those tasks:

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback );
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, LibcurlHeaders);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
    

Where,

CURLOPT_WRITEFUNCTION - callback for writing received data


CURLOPT_WRITEDATA - pointer passed to the write callback. 
**Note**: If you are using libcurl as a win32 DLL, you MUST use a CURLOPT_WRITEFUNCTION if you set this option. Otherwise, you will experience crashes.


## Environment settings
I built the program in MacOS. So I installed curl, libcurl using brew install in cmd. After finished installation, we have the path: **/user/local/Cellar**

Our cpp program and Make file can be stored there. 

In case we want to test connectivity of various websites, we should write main function like this:

## Main function description

    int main(int argc, char *argv[]) 
    {
      if( argc != 2 ) 
      {
      printf("usage: try './curl [url]' to make a get request.\n");
      return 1;
      }
      CURL *curl_handle;
      CURLcode res;

      curl_handle = curl_easy_init();
      if(curl_handle) 
      {
        curl_easy_setopt(curl_handle, CURLOPT_URL, argv[1]); // argv[1] is the website we want to test 
        ...
      }
    }
    
But according to the test requirement is Google website, I just put it directly in curl_easy_setopt(curl_handle, CURLOPT_URL, "https://www.google.com");

## Build program instruction

On MacOS cmd:

    cd /
    cd usr/local/Cellar/
    make
    
Then it will generate output file. We execute it by running: 
   
    ./output


## Program output

    SKTEST;<IP address : 172.217.160.68>;<HTTP response code: 200>;,Name lookup time: 0.0011000000>;<Connect time: 0.0139810000>;<Start tranfer time: 0.1204410000>;<Total time: 0.1417340000>;<50013 bytes retrieved>

Output screen capture

![](https://github.com/HectorTa1989/GG-Connectivity-Libcurl-Cpp/blob/master/SK%20test%20result.jpg)



## Ongoing work

I'm going to develop the code for 2 modes: One is to use Libcurl, another implements only based on TCP Linux socket.
