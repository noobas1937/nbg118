//
//  LocalController.h
//  IF
//
//  Created by zhaohf on 13-8-30.
//
//

#ifndef __IF__LocalController__
#define __IF__LocalController__

#include    "cocos2d.h"
#include    "CCRapidXMLParser.h"
#include    "CCINIParser.h"

class LocalController : public CCObject
{
public:
    static LocalController* shared();
    LocalController();
    ~LocalController();
    void init();
    //database.local.xml
    CCRapidXMLParser* DBXMLManager();
    //多语言文件读取方法
    CCINIParser* TextINIManager();
    void purgeData();
    void setLang(std::string lang){m_lang=lang;};
    std::string updatePath;
    
    std::string getLanguageFileName();
    std::string getLanguageFileNameBasedOnUserSystem();
private:
    CCRapidXMLParser* m_objXMLParser;
    CCINIParser* m_objINIFileParser;
    static LocalController* m_LocalController;
    std::string m_lang;
    
};
#define _lang(MSG) LocalController::shared()->TextINIManager()->getObjectByKey((MSG))
#define _lang_1(_msg,_param1) LocalController::shared()->TextINIManager()->getObjectByKey((_msg),"1",(_param1)).c_str()
#define _lang_2(_msg,_param1,_param2) LocalController::shared()->TextINIManager()->getObjectByKey((_msg),"2",(_param1),(_param2)).c_str()
#define _lang_3(_msg,_param1,_param2,_param3) LocalController::shared()->TextINIManager()->getObjectByKey((_msg),"3",(_param1),(_param2),(_param3)).c_str()
#define _lang_4(_msg,_param1,_param2,_param3,_param4) LocalController::shared()->TextINIManager()->getObjectByKey((_msg),"4",(_param1),(_param2),(_param3),(_param4)).c_str()
#define _lang_5(_msg,_param1,_param2,_param3,_param4,_param5) LocalController::shared()->TextINIManager()->getObjectByKey((_msg),"5",(_param1),(_param2),(_param3),(_param4),(_param5)).c_str()
#endif /* defined(__IF__CCLocalFileParser__) */


