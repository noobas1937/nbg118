//
//  ServerInfo.cpp
//  IF
//
//  Created by wangzhenlei on 13-9-27.
//
//

#include "ServerInfo.h"

ServerInfo::ServerInfo(CCDictionary* dict)
{
    const CCString *pStr = dict->valueForKey("id");
    if(pStr->compare("")!=0){
        id=pStr->intValue();
    }else{
        id = -1;
    }
    pStr = dict->valueForKey("name");
    if(pStr->compare("")!=0){
        name=pStr->getCString();
    }else{
        name = "";
    }
    pStr = dict->valueForKey("bRecommend");
    if(pStr->compare("")!=0){
        bRecommend=pStr->boolValue();
    }else{
        bRecommend = false;
    }
    pStr = dict->valueForKey("bNew");
    if(pStr->compare("")!=0){
        bNew=pStr->boolValue();
    }else{
        bNew = false;
    }
    pStr = dict->valueForKey("zone");
    if(pStr->compare("")!=0){
        zone=pStr->getCString();
    }else{
        zone = "";
    }

    pStr = dict->valueForKey("ip");
    if(pStr->compare("")!=0){
        ip=pStr->getCString();
    }else{
        ip = "";
    }
    pStr = dict->valueForKey("port");
    if(pStr->compare("")!=0){
        port=pStr->intValue();
    }else{
        port = -1;
    }
    pStr = dict->valueForKey("ip");
    if(pStr->compare("")!=0){
        ip=pStr->getCString();
    }else{
        ip = "";
    }
    pStr = dict->valueForKey("platformId");
    if(pStr->compare("")!=0){
        platformId=pStr->getCString();
    }else{
        platformId = "";
    }
    
    pStr = dict->valueForKey("nickName");
    if(pStr->compare("")!=0){
        nickName=pStr->getCString();
    }else{
        nickName = "";
    }

    pStr = dict->valueForKey("level");
    if(pStr->compare("")!=0){
        level = dict->valueForKey("level")->intValue();
    }else{
        level = -1;
    }

    pStr = dict->valueForKey("gameUid");
    if(pStr->compare("")!=0){
        gameUid = pStr->getCString();
    }else{
        gameUid = "";
    }
}