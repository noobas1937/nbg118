//
//  ResourcesPush.cpp
//  if
//
//  Created by zhaohf on 13-9-6.
//
//

#include "CommonInclude.h"
#include "ResourcesPush.h"

void ResourcesPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params)
        return;
    
    if (params) {
        auto &resourceInfo = GlobalData::shared()->resourceInfo;
        resourceInfo.setResourceData(params);
        
        CCDictionary *train=_dict(dict->objectForKey("train"));
        if(train){
            CCArray *generalArr = dynamic_cast<CCArray*>(train->objectForKey("gens"));
            map<string,GeneralInfo> *generals = &(GlobalData::shared()->generals);
            map<string, GeneralInfo>::iterator it;
            CCObject* obj;
            CCARRAY_FOREACH(generalArr, obj){
                string generalId = (_dict(obj))->valueForKey("generalId")->getCString();
                if(generalId!=""){
                    it = generals->find(generalId);
                    it->second.updateGeneralData(_dict(obj));
                }
                    
            }
        }
    }
}
