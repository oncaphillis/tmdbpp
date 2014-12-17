#include <tmdbpp/win_wget.h>

#include <winsock2.h>
#include <Winhttp.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <memory>

namespace tmdbpp {
    static HINTERNET _hsession = NULL;
    WGet & WGet::instance() {
        static WGet g;
        return g;
    }
    
    std::string WGet::get(const std::string & url) {
        std::wstring ws;
        std::string  os;
        URL_COMPONENTS c;
        
        ZeroMemory(&c, sizeof(c));
        c.dwStructSize = sizeof(c);
        
        c.dwSchemeLength    = (DWORD)-1;
        c.dwHostNameLength  = (DWORD)-1;
        c.dwUrlPathLength   = (DWORD)-1;
        c.dwExtraInfoLength = (DWORD)-1;

        
        // No nifty transcoding just dumb copying
        std::copy(url.begin(),url.end(),std::back_inserter(ws));
        
        if(!WinHttpCrackUrl(ws.c_str(),ws.length(),0,&c)) {
            throw std::runtime_error("unable to crack URL");
        }
        
        std::wstring sh = std::wstring(c.lpszScheme,c.dwSchemeLength);
        std::wstring hs = std::wstring(c.lpszHostName,c.dwHostNameLength);
        std::wstring pa = std::wstring(c.lpszUrlPath,c.dwUrlPathLength);
        std::wstring ex = std::wstring(c.lpszExtraInfo,c.dwExtraInfoLength);
        
        
        HINTERNET connect = NULL;
        HINTERNET request = NULL;
        BOOL      results = FALSE;
        bool      ssl     = false;
        DWORD     size    = 0;
        
        
        try {
            if(sh==L"http") {
            } else if(sh.compare(L"https")==0) {
                ssl = true;
            } else {
                throw std::runtime_error("illegal sheme");
            }
            
            if( (connect = WinHttpConnect( _hsession, hs.c_str(), c.nPort, 0)) != NULL) {
                if( (request = WinHttpOpenRequest( connect, L"GET",
                                                   (std::wstring(L"/")+pa+ex).c_str(), NULL, WINHTTP_NO_REFERER, NULL, ssl ? WINHTTP_FLAG_SECURE : NULL))!=NULL) {
                    if((results = WinHttpSendRequest( request, WINHTTP_NO_ADDITIONAL_HEADERS,
                                                      0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))==TRUE) {
                        
                        // Keep checking for data until there is nothing left.
                        
                        if((results = WinHttpReceiveResponse( request, NULL )) == TRUE)    {
                            // Check for available data.
                            while(true) {
                                if(!WinHttpQueryDataAvailable( request, &size)) {
                                    throw std::runtime_error("http error");
                                }
                                
                                if(size==0)
                                    break;
                                // Allocate space for the buffer.
                                std::auto_ptr<byte> buff(new byte[size+1]);
                                
                                // Read the Data.
                                ZeroMemory(buff.get(), size+1);
                                DWORD asize;
                                if (!WinHttpReadData( request, (LPVOID)buff.get(), size, &asize)) {
                                    throw std::runtime_error("http read error");
                                }
                                os += std::string((char*)buff.get(),asize);
                            }
                        }
                    }
                }
            }
        }
        catch(...) {
            if (request)
                WinHttpCloseHandle(request);
            if (connect)
                WinHttpCloseHandle(connect);
            throw;
        }
        
        if (request)
            WinHttpCloseHandle(request);
        if (connect)
            WinHttpCloseHandle(connect);
        
        return os;
    }
    
    WGet::WGet()  {
        _hsession = NULL;
        // Use WinHttpOpen to obtain a session handle.
        _hsession = WinHttpOpen( L"WGet Session/1.0",
                                 WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                 WINHTTP_NO_PROXY_NAME,
                                 WINHTTP_NO_PROXY_BYPASS, 0);

        if(_hsession==NULL)
            throw std::runtime_error("unable to open http client session");
    }
    
    WGet::~WGet() {
        if(_hsession!=NULL)
            WinHttpCloseHandle(_hsession);
 
        _hsession = NULL;

    }
}

/*int main (void)
{
    std::cerr << tmdbpp::WGet::instance().get("https://api.themoviedb.org/3/tv/popular?api_key=b1f45d094cb0c142dba7442cb5dd7341&page=1000") << std::endl;
}
*/
