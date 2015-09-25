//
//  AllianceEventInfo.cpp
//  IF
//
//  Created by ganxiaohua on 14-9-24.
//
//

#include "AllianceEventInfo.h"

void AllianceEventInfo::parse(CCDictionary *dict){
    isTime = false;
    createTime = dict->valueForKey("ct")->doubleValue() / 1000;
    //createTime = GlobalData::shared()->changeTime(createTime);
    srcTime = dict->valueForKey("ct")->getCString();
    type = dict->valueForKey("t")->intValue();
    m_params = dynamic_cast<CCArray*>(dict->objectForKey("p"));
    if(m_params==NULL){
        m_params = CCArray::create();
    }
}

std::string AllianceEventInfo::getContent(){
    if(m_params==NULL){
        m_params = CCArray::create();
    }
    string str = "";
    string srt1 = "";
    string srt2 = "";
    string srt3 = "";
    string srt4 = "";
    string abbr = "";
    switch (type) {////事件的类型占领为0，掠夺为1，科技为2，加入联盟为3，退出为4 公告为5，联盟建筑放置9 收回10
        case 0:
            if(m_params->count()==7){
                abbr = ((CCString*)m_params->objectAtIndex(0))->getCString();
//                if (abbr!="") {
//                    srt1.append("(");
//                    srt1.append(abbr);
//                    srt1.append(")");
//                }
                srt1 += ((CCString*)m_params->objectAtIndex(1))->getCString();
                int resType = ((CCString*)m_params->objectAtIndex(2))->intValue();
                srt2 = CCCommonUtils::getResourceProductByType(resType);
                srt3 = ((CCString*)m_params->objectAtIndex(3))->getCString();
                srt3 += ",";
                srt3 += ((CCString*)m_params->objectAtIndex(4))->getCString();
                
                posX = ((CCString*)m_params->objectAtIndex(3))->intValue();
                posY = ((CCString*)m_params->objectAtIndex(4))->intValue();
                
                abbr = ((CCString*)m_params->objectAtIndex(5))->getCString();
                if (abbr!="") {
                    srt4.append("(");
                    srt4.append(abbr);
                    srt4.append(")");
                }
                srt4 += ((CCString*)m_params->objectAtIndex(6))->getCString();
            }
            str = _lang_4("115225", srt1.c_str(), srt2.c_str(), srt3.c_str(), srt4.c_str());
            break;
        case 1:
            if(m_params->count()==6){
                abbr = ((CCString*)m_params->objectAtIndex(0))->getCString();
//                if (abbr!="") {
//                    srt1.append("(");
//                    srt1.append(abbr);
//                    srt1.append(")");
//                }
                srt1 += ((CCString*)m_params->objectAtIndex(1))->getCString();
                srt2 = ((CCString*)m_params->objectAtIndex(2))->getCString();
                srt2 += ",";
                srt2 += ((CCString*)m_params->objectAtIndex(3))->getCString();
                
                posX = ((CCString*)m_params->objectAtIndex(2))->intValue();
                posY = ((CCString*)m_params->objectAtIndex(3))->intValue();
                
                abbr = ((CCString*)m_params->objectAtIndex(4))->getCString();
                if (abbr!="") {
                    srt3.append("(");
                    srt3.append(abbr);
                    srt3.append(")");
                }
                srt3 += ((CCString*)m_params->objectAtIndex(5))->getCString();
            }
            str = _lang_3("115226", srt1.c_str(), srt2.c_str(), srt3.c_str());
            break;
        case 2:
            if(m_params->count()==3){
                abbr = ((CCString*)m_params->objectAtIndex(0))->getCString();
//                if (abbr!="") {
//                    srt1.append("(");
//                    srt1.append(abbr);
//                    srt1.append(")");
//                }
                srt1 += ((CCString*)m_params->objectAtIndex(1))->getCString();
                srt2 = ((CCString*)m_params->objectAtIndex(2))->getCString();
                srt2 = CCCommonUtils::getNameById(srt2);
            }
            str = _lang_2("115227", srt1.c_str(), srt2.c_str());
            break;
        case 3:
            if(m_params->count()==2){
                abbr = ((CCString*)m_params->objectAtIndex(0))->getCString();
//                if (abbr!="") {
//                    srt1.append("(");
//                    srt1.append(abbr);
//                    srt1.append(")");
//                }
                srt1 += ((CCString*)m_params->objectAtIndex(1))->getCString();
            }
            str = _lang_1("115228", srt1.c_str());
            break;
        case 4:
            srt1 = "";
            if(m_params->count()==2){
                abbr = ((CCString*)m_params->objectAtIndex(0))->getCString();
                srt1 += ((CCString*)m_params->objectAtIndex(1))->getCString();
            }
            str = _lang_1("115229", srt1.c_str());
            break;
        case 5:
            str = content;
            break;
        case 6:
            str = content;
            break;
        case 7:
            srt1 = "";
            srt2 = "";
            if(m_params->count()==2){
                srt1 = ((CCString*)m_params->objectAtIndex(0))->getCString();
                srt2 = ((CCString*)m_params->objectAtIndex(1))->getCString();
                srt2 = CCCommonUtils::getNameById(srt2);
            }
            //str = _lang_2("133080", srt1.c_str(), srt2.c_str());
            break;
        case 8:
            srt1 = "";
            srt2 = "";
            if(m_params->count()==6){
                srt1 = ((CCString*)m_params->objectAtIndex(1))->getCString();
                srt2 = ((CCString*)m_params->objectAtIndex(5))->getCString();
                srt2 = CCCommonUtils::getNameById(srt2);
            }
            str = _lang_2("133080", srt1.c_str(), srt2.c_str());
            break;
        case 9: {
            std::string playerName = "";
            std::string territoryName = "";
            if (m_params->count()==3) {
                playerName = ((CCString*)m_params->objectAtIndex(0))->getCString();
                std::string territoryType = ((CCString*)m_params->objectAtIndex(1))->getCString();
                if (territoryType == "14") {
                    territoryName = ((CCString*)m_params->objectAtIndex(2))->getCString();
                    if (territoryName.length() < 3) {//如果长度小于3则为默认名字，传的是默认名字的标识
                        if (territoryName == "1" || territoryName == "2" || territoryName == "3" || territoryName == "4" || territoryName == "5") {//默认名字标识从1到5，如扩展手动添加
                            std::string countStr = ((CCString*)m_params->objectAtIndex(2))->getCString();
                            territoryName = _lang_1("115312", countStr.c_str());
                        }
                    }
                }
                else if (territoryType == "18") {
                    std::string resType = ((CCString*)m_params->objectAtIndex(2))->getCString();
                    if (resType == "3") {
                        territoryName = _lang("115376");
                    }
                    else if (resType == "0")  {
                        territoryName = _lang("115377");
                    }
                    else if (resType == "2") {
                        territoryName = _lang("115378");
                    }
                    else if (resType == "1") {
                        territoryName = _lang("115379");
                    }
                }
                else if (territoryType == "20") {
                    territoryName = ((CCString*)m_params->objectAtIndex(2))->getCString();
                    if (territoryName.length() < 3) {//如果长度小于3则为默认名字，传的是默认名字的标识
                        if (territoryName == "1" || territoryName == "2" || territoryName == "3" || territoryName == "4" || territoryName == "5") {//默认名字标识从1到5，如扩展手动添加
                            std::string countStr = ((CCString*)m_params->objectAtIndex(2))->getCString();
                            territoryName = _lang_1("115406", countStr.c_str());
                        }
                    }
                }
                else if (territoryType == "23" ) {
                    territoryName = _lang("115534");
                }
            }
            else if (m_params->count()==2) {
                playerName = ((CCString*)m_params->objectAtIndex(0))->getCString();
                std::string territoryType = ((CCString*)m_params->objectAtIndex(1))->getCString();
                if (territoryType == "21") {
                    territoryName = _lang("115364");
                }
            }
            str = _lang_2("115402", playerName.c_str(), territoryName.c_str());
        }
            break;
        case 10: {
            std::string playerName = "";
            std::string territoryName = "";
            if (m_params->count()==3) {
                playerName = ((CCString*)m_params->objectAtIndex(0))->getCString();
                std::string territoryType = ((CCString*)m_params->objectAtIndex(1))->getCString();
                if (territoryType == "14") {
                    territoryName = ((CCString*)m_params->objectAtIndex(2))->getCString();
                    if (territoryName.length() < 3) {//如果长度小于3则为默认名字，传的是默认名字的标识
                        if (territoryName == "1" || territoryName == "2" || territoryName == "3" || territoryName == "4" || territoryName == "5") {
                            std::string countStr = ((CCString*)m_params->objectAtIndex(2))->getCString();
                            territoryName = _lang_1("115312", countStr.c_str());
                        }
                    }
                }
                else if (territoryType == "18") {
                    std::string resType = ((CCString*)m_params->objectAtIndex(2))->getCString();
                    if (resType == "3") {
                        territoryName = _lang("115376");
                    }
                    else if (resType == "0")  {
                        territoryName = _lang("115377");
                    }
                    else if (resType == "2") {
                        territoryName = _lang("115378");
                    }
                    else if (resType == "1") {
                        territoryName = _lang("115379");
                    }
                }
                else if (territoryType == "20") {
                    territoryName = ((CCString*)m_params->objectAtIndex(2))->getCString();
                    if (territoryName.length() < 3) {//如果长度小于3则为默认名字，传的是默认名字的标识
                        if (territoryName == "1" || territoryName == "2" || territoryName == "3" || territoryName == "4" || territoryName == "5") {//默认名字标识从1到5，如扩展手动添加
                            std::string countStr = ((CCString*)m_params->objectAtIndex(2))->getCString();
                            territoryName = _lang_1("115406", countStr.c_str());
                        }
                    }
                }
                else if (territoryType == "23" ) {
                    territoryName = _lang("115534");
                }
            }
            else if (m_params->count()==2) {
                playerName = ((CCString*)m_params->objectAtIndex(0))->getCString();
                std::string territoryType = ((CCString*)m_params->objectAtIndex(1))->getCString();
                if (territoryType == "21") {
                    territoryName = _lang("115364");
                }
            }
            str = _lang_2("115401", playerName.c_str(), territoryName.c_str());
        }
            break;
    }
    return str;
}