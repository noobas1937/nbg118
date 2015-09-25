//
//  Utf8Utils.h
//  aoe
//
//  Created by cc on 13-5-6.
//
//

#ifndef __aoe__Utf8Utils__
#define __aoe__Utf8Utils__

#include <string>

class Utf8Utils
{
public:
    static int fetch_utf_length( const char *str );    
    static char *utf8sub( const char *str, int s, int e );
    static int strlen( const char *str );
	static char buf[8192];
	static std::string* byteToHexStr(unsigned char byte_arr[], int arr_len);
	static std::string* byteToHexStr(const char *text);
};

#endif /* defined(__aoe__Utf8Utils__) */
