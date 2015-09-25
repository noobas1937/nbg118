//
//  Utf8Utils.cpp
//  aoe
//
//  Created by cc on 13-5-6.
//
//

#include "Utf8Utils.h"
#include <unistd.h>
#include <string.h>

char Utf8Utils::buf[] = {0};

int Utf8Utils::fetch_utf_length( const char *str )
{
    int x = 0xf0 & (*str);
    int ret;
    x >>= 4;
    ret = 4;
    if( x <= 14 ) ret = 3;
    if( x <= 12 ) ret = 2;
    if( x <= 7 ) ret = 1;
    return ret;
}

char *Utf8Utils::utf8sub( const char *str, int s, int len )
{
    int l = 0;
    const char *start;
    //char *ret;
    while(s&&*str)
    {
        str += fetch_utf_length(str);
        s--;
    }
    start = str;
    while(len&&*str)
    {
        str += fetch_utf_length(str);
        len--;
    }
    l = str - start;
    //ret = (char*)valloc(l+1);
    strncpy(buf, start, l+1);
    buf[l] = 0;
    return buf;
}

int Utf8Utils::strlen( const char *str ) {
    int ret = 0;
    while( str && *str ) {
        str += fetch_utf_length( str );
        ret ++;
    }
    return ret;
}


std::string* Utf8Utils::byteToHexStr(unsigned char byte_arr[], int arr_len)
{
	std::string* hexstr=new std::string();
	
	for (int i=0;i < arr_len;i++)
	{
		char hex1;
		char hex2;
		
		/*借助C++支持的unsigned和int的强制转换，把unsigned char赋值给int的值，那么系统就会自动完成强制转换*/
		int value=byte_arr[i];
		int S=value/16;
		int Y=value % 16;
		
		//将C++中unsigned char和int的强制转换得到的商转成字母
		if (S>=0&&S<=9)
			hex1=(char)(48+S);
		else
			hex1=(char)(55+S);
		
		//将C++中unsigned char和int的强制转换得到的余数转成字母
		if (Y>=0&&Y<=9)
			hex2=(char)(48+Y);
		else
			hex2=(char)(55+Y);
		
		//最后一步的代码实现，将所得到的两个字母连接成字符串达到目的
		*hexstr=*hexstr+" "+hex1+hex2;
	}
	
	return hexstr;
}

std::string* Utf8Utils::byteToHexStr(const char *text)
{
	std::string* hexstr=new std::string();
	int i = 0;
	
	while(*text){
		char hex1;
		char hex2;
		
		/*借助C++支持的unsigned和int的强制转换，把unsigned char赋值给int的值，那么系统就会自动完成强制转换*/
		int value=(unsigned char)*text;
		int S=value/16;
		int Y=value % 16;
		
		//将C++中unsigned char和int的强制转换得到的商转成字母
		if (S>=0&&S<=9)
			hex1=(char)(48+S);
		else
			hex1=(char)(55+S);
		
		//将C++中unsigned char和int的强制转换得到的余数转成字母
		if (Y>=0&&Y<=9)
			hex2=(char)(48+Y);
		else
			hex2=(char)(55+Y);
		
		//最后一步的代码实现，将所得到的两个字母连接成字符串达到目的
		*hexstr=*hexstr+" "+hex1+hex2;
		
		i++;
		text++;
	}
	
	return hexstr;
}