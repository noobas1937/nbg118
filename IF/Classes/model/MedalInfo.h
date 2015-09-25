//
//  MedalInfo.h
//  IF
//
//  Created by chenfubi on 15/4/3.
//
//

#ifndef __IF__MedalInfo__
#define __IF__MedalInfo__

#include "CommonInclude.h"

enum medalColor
{
    MEDAL_RED,
    MEDAL_BULE,
    MEDAL_PURPLE,
};

class MedalInfo
{
public:
    MedalInfo():medalId(0),name(""),icon_main(""),icon_sub(""),icon_base(""),hasGet(false),level(0),icon_color(0){};
    ~MedalInfo(){};
    
    int medalId;
    string name;
    string icon_main;
    string icon_sub;
    string icon_base;
    string achievement;
    bool hasGet;
    int level;
    int icon_color;
    
    void initData(CCDictionary* dict);
    void initData(string medalId);
    void setXmlData(CCDictionary* dict);
    void setXmlData(string medalId);
};

#endif /* defined(__IF__MedalInfo__) */
