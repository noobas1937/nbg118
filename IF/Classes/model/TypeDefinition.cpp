//
//  TypeDefinition.cpp
//  IF
//
//  Created by gj on 9/29/15.
//
//

#include "TypeDefinition.h"

//#define CHANGE_SERVER COCOS2D_DEBUG
//测试使用 不要提交
#define CHANGE_SERVER 1
//#define CHANGE_SERVER 2

#if CHANGE_SERVER == 1 // 本地服
    #define __SERVER__IP     "10.1.4.106"
    #define __SERVER__PORT   80
    #define __SERVER__ZONE   "COK1"
    #define __SERVERLIST__IP "10.1.4.106"

    #define __SERVER__IP_2     __SERVER__IP
    #define __SERVER__PORT_2   __SERVER__PORT
    #define __SERVER__ZONE_2   __SERVER__ZONE
    #define __SERVERLIST__IP_2 __SERVERLIST__IP
#elif CHANGE_SERVER == 2 // 测试服
    #define __SERVER__IP     "10.1.4.121"
    #define __SERVER__PORT   80
    #define __SERVER__ZONE   "COK2"
    #define __SERVERLIST__IP "10.1.4.121"

    #define __SERVER__IP_2     __SERVER__IP
    #define __SERVER__PORT_2   __SERVER__PORT
    #define __SERVER__ZONE_2   __SERVER__ZONE
    #define __SERVERLIST__IP_2 __SERVERLIST__IP
#else // 正式服
    #define __SERVER__IP     "p3nbg.elexapp.com"
    #define __SERVER__PORT   80
    #define __SERVER__ZONE   "COK2"
    #define __SERVERLIST__IP "p3nbg.elexapp.com"

    #define __SERVER__IP_2     "p1.nbg.elexapp.com"
    #define __SERVER__PORT_2   80
    #define __SERVER__ZONE_2   "COK2"
    #define __SERVERLIST__IP_2 "p1.nbg.elexapp.com"
#endif

const char * getServerIP_TypeDefinition() {
    return __SERVER__IP;
}

int getServerPort_TypeDefinition() {
    return __SERVER__PORT;
}

const char * getServerZone_TypeDefinition() {
    return __SERVER__ZONE;
}

const char * getServerListIP_TypeDefinition(bool b) {
    if (b) return __SERVERLIST__IP;
    else  return __SERVERLIST__IP_2;
}