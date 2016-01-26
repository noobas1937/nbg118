//
//  GetNewSystemMailMsg_iOS_Command.cpp
//  IF
//
//  Created by 张彦涛 on 15/9/27.
//
//
#include "ChatServiceCocos2dx.h"

#include "GetNewSystemMailMsg_iOS_Command.h"
bool GetNewSystemMailMsg_iOS_Command::handleRecieve(cocos2d::CCDictionary *dict){
    

    
    if (dict->valueForKey("cmd")->compare(Chat_Get_NewSystemMailMsg_iOS) != 0){
        return false;
    }
    
//    CCDictElement * pElement;
//    CCDICT_FOREACH(dict, pElement)
//    
//    {
//        
//        const char * key = pElement->getStrKey();
//        
//        CCString * value = (CCString *)pElement->getObject();
//        
//        CCLog(key);
//        
//        CCLog(value ->getCString());
//        
//    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        CCLOGFUNC("params==NULL");
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        return false;
    }
//    CCDictElement * pElement1;
//    CCDICT_FOREACH(params, pElement1)
//    
//    {
//        
//        const char * key = pElement1->getStrKey();
//        
//        CCString * value = (CCString *)pElement1->getObject();
//        
//        CCLog(key);
//        
//        CCLog(value ->getCString());
//        
//    }
//    
//     CCArray* mailInfoArray=CCArray::create();
    CCArray* mailInfoArray= CCArray::create();//new __Array;
    //mailInfoArray->initWithCapacity(7);
    CCArray* roomArray = dynamic_cast<CCArray*>(params->objectForKey("msg")) ;
    int  isHasMore = (params->valueForKey("more"))->intValue();
    if (roomArray!=NULL && roomArray->count()>0) {
        CCLOGFUNCF("roomArray count %d",roomArray->count());
        
        CCDictionary *item = NULL;
        for(int i=0;i<roomArray->count();i++){
            CCDictionary* item=dynamic_cast<CCDictionary*>(roomArray->objectAtIndex(i));
            
            
            if(item->objectForKey("contentsArr"))
            {
                CCArray* contentsArr = dynamic_cast<CCArray*>(item->objectForKey("contentsArr"));
                if(contentsArr)
                {
                    std::string actualContent = "";
                    CCLOGFUNCF("contentsArr size %d",contentsArr->count());
                    for (int i=0; i<contentsArr->count(); i++) {
                        CCString* content = dynamic_cast<CCString*>(contentsArr->objectAtIndex(i));
                        if (content) {
                            CCLOGFUNCF("content I :%s",content->getCString());
                            actualContent.append(content->getCString());
                        }
                    }
//                    std::string uid = item->valueForKey("uid")->getCString();
//                    
//                    int type = item->valueForKey("type")->intValue();
// 
                    
                    item->setObject(CCString::create(actualContent), "contents");
                }
            }

            
            mailInfoArray->addObject(item);
            
        }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChatServiceCocos2dx::pushSystemMailMsgWithMsgArray(mailInfoArray,isHasMore);
#endif
    }else{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChatServiceCocos2dx::pushSystemMailMsgWithMsgArray(NULL,isHasMore);
#endif
    }

        return true ;
}

GetNewSystemMailMsg_iOS_Command::~GetNewSystemMailMsg_iOS_Command()
{
    
}