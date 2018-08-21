#pragma once

#include "File.h"
#include "Log.h"
#include <curl/curl.h>

size_t writeHttpData (void* ptr, size_t size, size_t nmemb, void* stream);

class Http {
    private:
        static void initCurl () {
            static bool needsInit = true;
            if (needsInit) {
                curl_global_init(CURL_GLOBAL_DEFAULT);
                needsInit = false;
            }
        }

    public:
        static PtrToBuffer get (const Text& url, const Text& filename) {
            initCurl ();

            CURL* curlHandle = curl_easy_init();;
            if(curlHandle) {
                curl_easy_setopt (curlHandle, CURLOPT_URL, url.get ());

                //curl_easy_setopt (curlHandle, CURLOPT_SSL_VERIFYPEER, 0L);

                //curl_easy_setopt (curlHandle, CURLOPT_SSL_VERIFYHOST, 0L);

                // Switch on full protocol/debug output while testing
                //curl_easy_setopt (curlHandle, CURLOPT_VERBOSE, 1L);

                // disable progress meter, set to 0L to enable and disable debug output
                curl_easy_setopt (curlHandle, CURLOPT_NOPROGRESS, 1L);

                // send all data to this function
                curl_easy_setopt (curlHandle, CURLOPT_WRITEFUNCTION, writeHttpData);

                /* open the file */
                FILE* httpDataFile = fopen (filename, "wb");
                if(httpDataFile) {
                    // write the page body to this file handle
                    curl_easy_setopt (curlHandle, CURLOPT_WRITEDATA, httpDataFile);

                    // perform the request, res will get the return code
                    CURLcode res = curl_easy_perform (curlHandle);
                    if(res != CURLE_OK) {
                        Log::error () << "curl_easy_perform() failed: " << curl_easy_strerror (res) << endl;
                    }

                    // close the file
                    fclose (httpDataFile);
                }

                curl_easy_cleanup (curlHandle);
            }

            // read the file result
            File file (filename);
            return file.read ();
        }

        static Text getText (const Text& url, const Text& filename) {
            return Text (get (url, filename));
        }
};
