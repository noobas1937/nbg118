//
//  EquipmentInfo.cpp
//  IF
//
//  Created by fubin on 14-12-2.
//
//

#include "EquipmentInfo.h"

EquipmentInfo::EquipmentInfo(CCDictionary* dict)
{//<ItemSpec id="260100" level="1" site="0" color="0" name="104301" description="104302" icon="item701" material1="201010;1" material2="201060;1"/>
    this->itemId = dict->valueForKey("id")->intValue();
    this->level = dict->valueForKey("level")->intValue();
    this->site = dict->valueForKey("site")->intValue();
    this->color = dict->valueForKey("color")->intValue();
    this->name = dict->valueForKey("name")->getCString();
    this->des = dict->valueForKey("description")->getCString();
    this->icon = dict->valueForKey("icon")->getCString();
    
    this->cost = dict->valueForKey("cost")->getCString();
    this->time = dict->valueForKey("time")->intValue();
    
    this->break_down = dict->valueForKey("break_down")->intValue();
    this->break_up = dict->valueForKey("break_up")->intValue();
    this->showType = dict->valueForKey("show")->getCString();
    if (dict->objectForKey("power")) {
        this->power = dict->valueForKey("power")->intValue();
    }
    
    string mate = dict->valueForKey("material1")->getCString();
    vector<string> tmpMateVec;
    if (mate!="") {
        mateVec.push_back(mate);
        CCCommonUtils::splitString(mate, ";", tmpMateVec);
        if (tmpMateVec.size()>=2) {
            mateMap[atoi(tmpMateVec[0].c_str())] += atoi(tmpMateVec[1].c_str());
        }
    }
    tmpMateVec.clear();
    mate = dict->valueForKey("material2")->getCString();
    if (mate!="") {
        mateVec.push_back(mate);
        CCCommonUtils::splitString(mate, ";", tmpMateVec);
        if (tmpMateVec.size()>=2) {
            mateMap[atoi(tmpMateVec[0].c_str())] += atoi(tmpMateVec[1].c_str());
        }
    }
    tmpMateVec.clear();
    mate = dict->valueForKey("material3")->getCString();
    if (mate!="") {
        mateVec.push_back(mate);
        CCCommonUtils::splitString(mate, ";", tmpMateVec);
        if (tmpMateVec.size()>=2) {
            mateMap[atoi(tmpMateVec[0].c_str())] += atoi(tmpMateVec[1].c_str());
        }
    }
    tmpMateVec.clear();
    mate = dict->valueForKey("material4")->getCString();
    if (mate!="") {
        mateVec.push_back(mate);
        CCCommonUtils::splitString(mate, ";", tmpMateVec);
        if (tmpMateVec.size()>=2) {
            mateMap[atoi(tmpMateVec[0].c_str())] += atoi(tmpMateVec[1].c_str());
        }
    }
    
    costLv = 0;
    costLv = dict->valueForKey("son_equipment")->intValue();
//    string strEquip = dict->valueForKey("son_equipment")->getCString();
//    if (strEquip!="") {
//        vector<string> tmpVec;
//        CCCommonUtils::splitString(strEquip, "|", tmpVec);
//        for (int ii=0; ii<tmpVec.size(); ii++) {
//            vector<string> eVec;
//            CCCommonUtils::splitString(tmpVec[ii], ";", eVec);
//            if (eVec.size()==2) {
//                equipMap[ atoi(eVec[0].c_str()) ] = atoi(eVec[1].c_str());
//            }
//        }
//    }
    
    for (int i=1; i<=4; i++) {
        string key = "show";
        key = key+CC_ITOA(i);
        if (dict->objectForKey(key)) {
            this->showDias[i] = dict->valueForKey(key)->getCString();
        }else{
            break;
        }
    }
    
    for (int j=1; j<=4; j++) {
        string key = "num";
        key = key+CC_ITOA(j);
        if (dict->objectForKey(key)) {
            this->values[j] = dict->valueForKey(key)->floatValue();
        }else{
            break;
        }
    }
    
    for (int m=1; m<=4; m++) {
        string key = "para";
        key = key+CC_ITOA(m);
        if (dict->objectForKey(key)) {
            string tmpStr = dict->valueForKey(key)->getCString();
            vector<string> tmpVec;
            CCCommonUtils::splitString(tmpStr, "|", tmpVec);
            for (int n=0; n<tmpVec.size(); n++) {
                this->parasMap[m].push_back(atoi(tmpVec[n].c_str()));
            }
        }else{
            break;
        }
    }
    if (dict->valueForKey("para1")->getCString()) {
        this->para1 = dict->valueForKey("para1")->getCString();
    }
    if (dict->valueForKey("num1")->getCString()) {
        this->num1 = dict->valueForKey("num1")->getCString();
    }
    if (dict->valueForKey("show1")->getCString()) {
        this->show1 = dict->valueForKey("show1")->getCString();
    }
}

void EquipmentInfo::SetInfoFromServer(CCDictionary* dict)
{
    
}

void EquipmentInfo::setLongProperty(string extraProperty){
    if (this->site==6) {
        vector<string> pVec1;
        CCCommonUtils::splitString(this->para1, "|", pVec1);
        vector<string> nVec1;
        CCCommonUtils::splitString(this->num1, "|", nVec1);
        vector<string> sVec1;
        CCCommonUtils::splitString(this->show1, "|", sVec1);
        
        vector<string> pVec2;
        vector<string> nVec2;
        vector<string> sVec2;
        
        vector<string> vec;
        CCCommonUtils::splitString(extraProperty, "|", vec);
        for (int i=0; i<vec.size(); i++) {
            string vStr = vec[i];
            vector<string> vec1;
            CCCommonUtils::splitString(vStr, ",", vec1);
            if (vec1.size()==3) {
                pVec2.push_back(vec1[0]);
                nVec2.push_back(vec1[1]);
                sVec2.push_back(vec1[2]);
            }
        }
        
        this->parasMap.clear();
        this->values.clear();
        this->showDias.clear();
        
        int count = 0;
        if (pVec1.size()>count && nVec1.size()>count && sVec1.size()>count) {
            string tmpStr = pVec1[count];
            vector<string> tmpVec;
            CCCommonUtils::splitString(tmpStr, "@", tmpVec);
            for (int n=0; n<tmpVec.size(); n++) {
                this->parasMap[1].push_back(atoi(tmpVec[n].c_str()));
            }
//            this->parasMap[1].push_back(atoi(pVec1[count].c_str()));
            this->values[1] = atof(nVec1[count].c_str());
            this->showDias[1] = sVec1[count];
        }
        if (pVec2.size()>count && nVec2.size()>count && sVec2.size()>count) {
            string tmpStr = pVec2[count];
            vector<string> tmpVec;
            CCCommonUtils::splitString(tmpStr, "@", tmpVec);
            for (int n=0; n<tmpVec.size(); n++) {
                this->parasMap[2].push_back(atoi(tmpVec[n].c_str()));
            }
//            this->parasMap[2].push_back(atoi(pVec2[count].c_str()));
            this->values[2] = atof(nVec2[count].c_str());
            this->showDias[2] = sVec2[count];
        }
        count++;
        if (pVec1.size()>count && nVec1.size()>count && sVec1.size()>count) {
            string tmpStr = pVec1[count];
            vector<string> tmpVec;
            CCCommonUtils::splitString(tmpStr, "@", tmpVec);
            for (int n=0; n<tmpVec.size(); n++) {
                this->parasMap[3].push_back(atoi(tmpVec[n].c_str()));
            }
//            this->parasMap[3].push_back(atoi(pVec1[count].c_str()));
            this->values[3] = atof(nVec1[count].c_str());
            this->showDias[3] = sVec1[count];
        }
        if (pVec2.size()>count && nVec2.size()>count && sVec2.size()>count) {
            string tmpStr = pVec2[count];
            vector<string> tmpVec;
            CCCommonUtils::splitString(tmpStr, "@", tmpVec);
            for (int n=0; n<tmpVec.size(); n++) {
                this->parasMap[4].push_back(atoi(tmpVec[n].c_str()));
            }
//            this->parasMap[4].push_back(atoi(pVec2[count].c_str()));
            this->values[4] = atof(nVec2[count].c_str());
            this->showDias[4] = sVec2[count];
        }
        count++;
        if (pVec1.size()>count && nVec1.size()>count && sVec1.size()>count) {
            string tmpStr = pVec1[count];
            vector<string> tmpVec;
            CCCommonUtils::splitString(tmpStr, "@", tmpVec);
            for (int n=0; n<tmpVec.size(); n++) {
                this->parasMap[5].push_back(atoi(tmpVec[n].c_str()));
            }
//            this->parasMap[5].push_back(atoi(pVec1[count].c_str()));
            this->values[5] = atof(nVec1[count].c_str());
            this->showDias[5] = sVec1[count];
        }
        
    }
}