//
//  BattleManager.cpp
//  IF
//
//  Created by ganxiaohua on 13-9-17.
//
//

#include "BattleManager.h"
#include "UIComponent.h"
#include "GeneralInfo.h"
#include "CCFloatingText.h"
#include "GeneralManager.h"
#include "SkillHurtResult.h"
#include "MailFightReportCommand.h"
#include <spine/Json.h>

using namespace cocos2d;
 
CCPoint postion[11][3] =
{
    { ccp(-380, -300), ccp(-132, -300), ccp(100, -300), },//0
    { ccp(-370, -130), ccp(-132, -130), ccp(100, -130), },//1
    { ccp(-360, 0), ccp(-132, 0), ccp(100, 0), },
    { ccp(-350, 105), ccp(-132, 105), ccp(100, 105), },
    { ccp(-350, 200), ccp(-132, 200), ccp(90, 200), },
    { ccp(-350, 285), ccp(-132, 285), ccp(85, 285), },
    { ccp(-350, 370), ccp(-132, 370), ccp(85, 370), },
    { ccp(-340, 450), ccp(-132, 450), ccp(85, 450), },
    { ccp(-340, 515), ccp(-132, 515), ccp(85, 515), },
    { ccp(-340, 570), ccp(-132, 570), ccp(85, 570), },//9
    { ccp(-340, 620), ccp(-132, 620), ccp(85, 620), },//10
};

BattleManager* BattleManager::instance = NULL;

BattleManager* BattleManager::shared()
{
    if( NULL == instance )
    {
        instance = new BattleManager();
        instance->isDuringRequestingBattleInfo = false;
    }
    return instance;
}

void BattleManager::requestBattleInfo(int cityId,int checkpointId,int cost){
    m_checkpointId = checkpointId;
    m_cityId = cityId;
    GlobalData::shared()->lordInfo.energy = GlobalData::shared()->lordInfo.energy - cost;
//    UIComponent::getInstance()->onCityEnergyUpdate(NULL);
    UIComponent::getInstance()->UIHide();
    m_lastSence = SceneController::getInstance()->currentSceneId;
//    BattleReportCommand* cmd = new BattleReportCommand(cityId,cost);
//    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(BattleManager::goToBattle), NULL));
//    cmd->sendAndRelease();
    this->parseBattleReport(NULL);
    goToBattle(NULL);
    
}

void BattleManager::mailFightReport(std::string reportUid){
    if(GlobalData::shared()->playerInfo.gmFlag==1){
        MailFightReportCommand* cmd = new MailFightReportCommand(reportUid);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(BattleManager::goToBattle), NULL));
        cmd->sendAndRelease();
    }
}

void BattleManager::goToBattle(CCObject* p){
//    BattleReportCommand* cmd = new BattleReportCommand(0,0);
//    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(BattleManager::goToBattle), NULL));
//    cmd->sendAndRelease();
//    return ;
//    GameController::getInstance()->removeWaitInterface();
//    SceneController::getInstance()->gotoScene(SCENE_ID_BATTLE,false,true,1);
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRANSITION_FINISHED);
}

std::string BattleManager::getWalkDirect(std::string direct){
    std::string changeDirect = "N";
    if(direct=="NE"){
        changeDirect = "NW";
    }else if(direct=="SE"){
        changeDirect = "SW";
    }else if(direct=="E"){
        changeDirect = "W";
    }else{
        changeDirect = direct;
    }
    return changeDirect;
}

void  BattleManager::parseBattleReport(CCObject* report){
//    if(report!=NULL){
//        m_report = _dict(report)->valueForKey("report")->getCString();
//    }
//    if(m_report==""){
//        CCLOG("battle report is NULL");//
//        //return;
//    }
//    CCLOG("result=%s",m_report.c_str());//
//    //107310
//    std::string strReport1 = "{\"battlereport\":{\"winside\":0,\"battleType\":3,\"report\":\"2_1_3|gj|1_1_1|0|0|0|sh|0;2_1_4|gj|1_1_1|0|0|0|sh|0;xj|4|107920|1_1_1|sh|0;xj|4|107900|1_1_1|sh|0;1_1_1|mv|0|70|2_1_1|1|5;2_1_1|mv|0|70|1_1_1|1|5;2_1_1|gj|1_1_1|6|0|0|sh|0;1_1_1|gj|2_1_1|6|1|938|sh|938;1_1_1|mv|0|80|2_1_2|7|1;2_1_2|mv|0|80|1_1_1|1|7;2_1_2|gj|1_1_1|8|0|0|sh|0;1_1_1|gj|2_1_2|8|0|711|sh|711\",\"maxRound\":8},\"attacker\":{\"member\":\"1|107009|41100|41100|0|40\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\",\"general\":\"1|240020|6\"},\"defender\":{\"member\":\"1|107002|285|285|0|100;2|107301|102|102|0|110;3|107800|1|1|-30|130;4|107800|1|1|30|130\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\",\"general\":\"\"}}";
//    
//    std::string strReport2 = "{\"battlereport\":{\"winside\":1,\"battleType\":3,\"report\":\"2_1_4|mv|0|110|1_1_1|1|3;xj|4|107900|1_1_1|sh|5;1_1_1|mv|0|70|2_1_1|1|5;2_1_1|mv|0|70|1_1_1|1|5;2_1_2|mv|0|70|1_1_1|1|7;2_1_3|mv|0|70|1_1_1|1|9;xj|10|107900|1_1_1|sh|5;xj|16|107900|1_1_1|sh|5;1_1_1|gj|2_1_1|6|12|3|sh|0|sh|1|sh|0|sh|1|sh|0|sh|1|sh|0;2_1_1|gj|1_1_1|6|12|4|sh|0|sh|0|sh|0|sh|1|sh|1|sh|1|sh|1;2_1_2|gj|1_1_1|8|10|37|sh|6|sh|6|sh|6|sh|6|sh|6|sh|7;2_1_3|gj|1_1_1|10|8|15|sh|3|sh|3|sh|3|sh|3|sh|3;2_1_4|gj|1_1_1|4|14|17|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|3;2_1_5|gj|1_1_1|2|16|23|sh|2|sh|2|sh|3|sh|3|sh|2|sh|2|sh|3|sh|3|sh|3\",\"maxRound\":18},\"attacker\":{\"member\":\"1|107000|110|110|0|40\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\",\"general\":\"1|240020|2\"},\"defender\":{\"member\":\"1|107000|97|97|0|100;2|107002|140|140|0|110;3|107100|160|160|0|120;4|107200|110|110|0|130;5|107300|20|20|0|140\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\",\"general\":\"1|240020|5\"}}";
//
//    std::string strReport3 = "{\"battlereport\":{\"winside\":1,\"battleType\":3,\"report\":\"2_1_9|gj|1_1_1|0|0|2|sh|2;2_1_10|gj|1_1_1|0|0|2|sh|2;1_1_5|mv|0|10|2_1_1|1|1;2_1_7|mv|0|150|1_1_1|1|1;1_1_3|mv|0|30|2_1_1|1|3;1_1_4|mv|0|30|2_1_1|1|3;2_1_6|mv|0|140|1_1_1|1|3;2_1_8|mv|0|160|1_1_1|1|3;xj|4|107900|1_1_1|sh|2;xj|4|107901|1_1_1|sh|15;xj|4|107911|1_1_1|sh|18;1_1_1|mv|0|70|2_1_1|1|5;2_1_1|mv|0|70|1_1_1|1|5;1_1_2|mv|0|70|2_1_1|1|7;2_1_2|mv|0|70|1_1_1|1|7;2_1_3|mv|0|70|1_1_1|1|9;xj|10|107900|1_1_1|sh|2;xj|10|107901|1_1_1|sh|15;xj|10|107911|1_1_1|sh|19;2_1_4|mv|0|70|1_1_1|1|11;2_1_5|mv|0|70|1_1_1|1|13;xj|16|107900|1_1_1|sh|2;xj|16|107901|1_1_1|sh|17;xj|16|107911|1_1_1|sh|20;2_1_9|gj|1_1_1|18|0|3|sh|3;2_1_10|gj|1_1_1|18|0|3|sh|3;1_1_1|gj|2_1_1|6|16|12|sh|2|sh|2|sh|2|sh|2|sh|1|sh|1|sh|1|sh|1|sh|0;xj|22|107900|1_1_1|sh|0;xj|22|107901|1_1_1|sh|0;xj|22|107911|1_1_1|sh|0;2_1_1|gj|1_1_1|6|18|176|sh|19|sh|19|sh|19|sh|19|sh|19|sh|19|sh|19|sh|21|sh|22;2_1_2|gj|1_1_1|8|16|157|sh|18|sh|19|sh|19|sh|19|sh|20|sh|20|sh|20|sh|22;2_1_3|gj|1_1_1|10|14|25|sh|3|sh|3|sh|3|sh|4|sh|4|sh|4|sh|4;2_1_4|gj|1_1_1|12|12|37|sh|6|sh|6|sh|5|sh|6|sh|7|sh|7;2_1_5|gj|1_1_1|14|10|101|sh|19|sh|20|sh|20|sh|20|sh|22;2_1_6|gj|1_1_1|4|20|226|sh|22|sh|21|sh|22|sh|22|sh|22|sh|22|sh|23|sh|23|sh|24|sh|25;2_1_7|gj|1_1_1|2|22|105|sh|9|sh|9|sh|9|sh|9|sh|9|sh|9|sh|10|sh|9|sh|10|sh|11|sh|11;2_1_8|gj|1_1_1|4|20|284|sh|27|sh|27|sh|27|sh|28|sh|28|sh|28|sh|29|sh|29|sh|29|sh|32;xj|28|107900|1_1_1|sh|0;xj|28|107901|1_1_1|sh|0;xj|28|107911|1_1_1|sh|0;xj|34|107900|1_1_1|sh|0;xj|34|107901|1_1_1|sh|0;xj|34|107911|1_1_1|sh|0;2_1_9|gj|1_1_1|36|0|0|sh|0;2_1_10|gj|1_1_1|36|0|0|sh|0;xj|40|107900|1_1_1|sh|0;xj|40|107901|1_1_1|sh|0;xj|40|107911|1_1_1|sh|0;xj|46|107900|1_1_1|sh|0;xj|46|107901|1_1_1|sh|0;xj|46|107911|1_1_1|sh|0;xj|52|107900|1_1_1|sh|0;xj|52|107901|1_1_1|sh|0;xj|52|107911|1_1_1|sh|0;2_1_9|gj|1_1_1|54|0|0|sh|0;2_1_10|gj|1_1_1|54|0|0|sh|0;xj|58|107900|1_1_1|sh|0;xj|58|107901|1_1_1|sh|0;xj|58|107911|1_1_1|sh|0;xj|64|107900|1_1_1|sh|0;xj|64|107901|1_1_1|sh|0;xj|64|107911|1_1_1|sh|0;xj|70|107900|1_1_1|sh|0;xj|70|107901|1_1_1|sh|0;xj|70|107911|1_1_1|sh|0;2_1_9|gj|1_1_1|72|0|0|sh|0;2_1_10|gj|1_1_1|72|0|0|sh|0;xj|76|107900|1_1_1|sh|0;xj|76|107901|1_1_1|sh|0;xj|76|107911|1_1_1|sh|0;xj|82|107900|1_1_1|sh|0;xj|82|107901|1_1_1|sh|0;xj|82|107911|1_1_1|sh|0;xj|88|107900|1_1_1|sh|0;xj|88|107901|1_1_1|sh|0;xj|88|107911|1_1_1|sh|0;2_1_9|gj|1_1_1|90|0|0|sh|0;2_1_10|gj|1_1_1|90|0|0|sh|0;xj|94|107900|1_1_1|sh|0;xj|94|107901|1_1_1|sh|0;xj|94|107911|1_1_1|sh|0;2_1_1|gj|1_1_2|24|76|513|sh|12|sh|13|sh|13|sh|13|sh|12|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|14|sh|13|sh|13|sh|13|sh|13|sh|14|sh|14|sh|14|sh|13|sh|13|sh|13|sh|14|sh|13|sh|14|sh|14|sh|14;1_1_2|gj|2_1_1|8|92|25|sh|1|sh|1|sh|0|sh|1|sh|1|sh|1|sh|0|sh|1|sh|0|sh|1|sh|1|sh|0|sh|0|sh|1|sh|1|sh|0|sh|0|sh|1|sh|1|sh|0|sh|0|sh|1|sh|1|sh|0|sh|0|sh|1|sh|1|sh|1|sh|0|sh|0|sh|0|sh|1|sh|1|sh|1|sh|0|sh|0|sh|0|sh|0|sh|0|sh|1|sh|1|sh|1|sh|1|sh|1|sh|0|sh|0|sh|0;2_1_2|gj|1_1_2|24|76|432|sh|11|sh|11|sh|11|sh|11|sh|11|sh|10|sh|11|sh|11|sh|11|sh|11|sh|11|sh|11|sh|10|sh|11|sh|11|sh|11|sh|11|sh|11|sh|11|sh|11|sh|11|sh|11|sh|11|sh|11|sh|11|sh|11|sh|12|sh|11|sh|11|sh|11|sh|11|sh|12|sh|11|sh|12|sh|11|sh|12|sh|12|sh|11|sh|11;1_1_3|gj|2_1_1|4|96|16|sh|1|sh|0|sh|0|sh|0|sh|1|sh|0|sh|0|sh|0|sh|1|sh|0|sh|1|sh|0|sh|0|sh|1|sh|1|sh|0|sh|0|sh|1|sh|1|sh|0|sh|0|sh|1|sh|1|sh|0|sh|0|sh|0|sh|1|sh|0|sh|0|sh|0|sh|0|sh|1|sh|1|sh|0|sh|0|sh|0|sh|0|sh|0|sh|1|sh|1|sh|1|sh|0|sh|0|sh|0|sh|0|sh|0|sh|0|sh|0|sh|0;2_1_3|gj|1_1_2|24|76|82|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|3|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|3|sh|2|sh|2|sh|2|sh|2|sh|3|sh|3;1_1_4|gj|2_1_1|4|96|72|sh|1|sh|2|sh|2|sh|1|sh|1|sh|2|sh|1|sh|2|sh|1|sh|2|sh|1|sh|1|sh|2|sh|1|sh|1|sh|2|sh|2|sh|1|sh|1|sh|1|sh|2|sh|1|sh|1|sh|1|sh|2|sh|2|sh|1|sh|1|sh|1|sh|2|sh|2|sh|1|sh|1|sh|1|sh|2|sh|2|sh|2|sh|2|sh|1|sh|1|sh|1|sh|1|sh|1|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2;2_1_4|gj|1_1_2|24|76|135|sh|3|sh|3|sh|3|sh|3|sh|3|sh|4|sh|3|sh|3|sh|3|sh|3|sh|3|sh|3|sh|4|sh|3|sh|3|sh|3|sh|4|sh|3|sh|4|sh|4|sh|3|sh|4|sh|4|sh|3|sh|4|sh|3|sh|3|sh|4|sh|4|sh|4|sh|4|sh|4|sh|3|sh|4|sh|4|sh|4|sh|4|sh|4|sh|3;1_1_5|gj|2_1_1|2|98|182|sh|3|sh|4|sh|3|sh|3|sh|4|sh|4|sh|4|sh|4|sh|4|sh|4|sh|3|sh|4|sh|4|sh|3|sh|4|sh|4|sh|3|sh|3|sh|4|sh|4|sh|4|sh|3|sh|4|sh|4|sh|4|sh|3|sh|4|sh|4|sh|4|sh|4|sh|3|sh|4|sh|4|sh|4|sh|4|sh|3|sh|3|sh|3|sh|4|sh|4|sh|4|sh|4|sh|4|sh|4|sh|3|sh|3|sh|3|sh|3|sh|3|sh|4;2_1_5|gj|1_1_2|24|76|529|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|13|sh|14|sh|13|sh|14|sh|14|sh|14|sh|13|sh|14|sh|13|sh|13|sh|14|sh|13|sh|13|sh|14|sh|14|sh|14|sh|14|sh|14|sh|13|sh|14|sh|14|sh|14|sh|14|sh|14|sh|14|sh|14|sh|14|sh|14|sh|15;1_1_6|gj|2_1_1|2|98|111|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|3|sh|2|sh|2|sh|3|sh|2|sh|2|sh|2|sh|3|sh|2|sh|2|sh|2|sh|3|sh|2|sh|2|sh|2|sh|3|sh|2|sh|2|sh|2|sh|2|sh|3|sh|2|sh|2|sh|2|sh|2|sh|2|sh|3|sh|3|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|2|sh|3|sh|3|sh|3|sh|2;2_1_6|gj|1_1_2|24|76|603|sh|15|sh|15|sh|15|sh|15|sh|15|sh|15|sh|16|sh|15|sh|15|sh|15|sh|15|sh|15|sh|15|sh|15|sh|15|sh|15|sh|16|sh|15|sh|16|sh|16|sh|15|sh|16|sh|16|sh|15|sh|15|sh|15|sh|16|sh|16|sh|16|sh|15|sh|16|sh|16|sh|16|sh|16|sh|16|sh|16|sh|16|sh|16|sh|16;2_1_7|gj|1_1_2|24|76|250|sh|6|sh|6|sh|6|sh|6|sh|7|sh|6|sh|6|sh|7|sh|6|sh|6|sh|7|sh|6|sh|7|sh|6|sh|6|sh|6|sh|6|sh|6|sh|6|sh|6|sh|7|sh|6|sh|6|sh|7|sh|7|sh|7|sh|6|sh|6|sh|7|sh|7|sh|6|sh|6|sh|7|sh|6|sh|7|sh|7|sh|7|sh|7|sh|7;2_1_8|gj|1_1_2|24|76|780|sh|19|sh|19|sh|19|sh|20|sh|19|sh|19|sh|19|sh|19|sh|20|sh|20|sh|19|sh|20|sh|19|sh|20|sh|20|sh|20|sh|20|sh|20|sh|20|sh|20|sh|20|sh|20|sh|20|sh|20|sh|20|sh|20|sh|21|sh|20|sh|20|sh|20|sh|21|sh|21|sh|21|sh|21|sh|21|sh|20|sh|21|sh|21|sh|21\",\"maxRound\":100},\"attacker\":{\"member\":\"1|107102|1170|1170|0|40;2|107301|4991|4991|0|30;3|107200|1315|1315|0|20;4|107201|4137|4137|0|10;5|107202|5773|5773|0|0;6|107300|2863|2863|0|-10\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\",\"general\":\"\"},\"defender\":{\"member\":\"1|107003|22147|22147|0|100;2|107002|2808|2808|0|110;3|107100|724|724|0|120;4|107101|606|606|0|130;5|107102|7384|7384|0|140;6|107202|4010|4010|0|150;7|107300|907|907|0|160;8|107302|1860|1860|0|170;9|107801|1|1|-30|130;10|107801|1|1|30|130\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\",\"general\":\"1|240020|12\"}}";
//    
//    std::string strReport4 = "{\"battlereport\":{\"winside\":1,\"battleType\":0,\"report\":\"25|gj|11|0|1|99|sh|99;11|mv|0|70|21|1|5;21|mv|0|70|11|1|5;13|mv|0|30|21|5|1;111|mv|-30|70|211|1|5;211|mv|-30|70|111|1|5;121|mv|30|70|221|1|5;221|mv|30|70|121|1|5;12|mv|0|70|21|3|7;14|mv|0|30|21|7|3;11|gj|21|6|10|459|sh|86|sh|88|sh|89|sh|94|sh|102;11|jn|101009|16|21|sh|240;21|gj|11|6|10|173|sh|46|sh|42|sh|38|sh|29|sh|18;21|jn|101009|16|11|sh|10;13|gj|21|6|10|768|sh|144|sh|147|sh|150|sh|158|sh|169;13|jn|101009|16|21|sh|380;111|gj|211|6|10|270|sh|56|sh|55|sh|54|sh|53|sh|52;111|jn|101009|16|211|sh|104;211|gj|111|6|10|270|sh|56|sh|55|sh|54|sh|53|sh|52;211|jn|101009|16|111|sh|104;121|gj|221|6|10|270|sh|56|sh|55|sh|54|sh|53|sh|52;121|jn|101009|16|221|sh|104;221|gj|121|6|10|270|sh|56|sh|55|sh|54|sh|53|sh|52;221|jn|101009|16|121|sh|104;12|gj|21|10|7|418|sh|91|sh|97|sh|106|sh|124;14|gj|21|10|7|667|sh|150|sh|158|sh|169|sh|190;25|gj|11|18|1|44|sh|44;11|mv|-30|70|211|17|5;12|mv|-30|70|211|17|5;13|mv|-12|48|211|17|5;14|mv|-12|48|211|17|5;12|gj|211|22|2|91|sh|91;12|jn|101009|24|211|sh|196;14|gj|211|22|2|150|sh|150;14|jn|101009|24|211|sh|318;211|gj|111|18|9|197|sh|50|sh|49|sh|49|sh|36|sh|13;11|gj|211|22|5|292|sh|87|sh|94|sh|111;12|gj|211|26|1|116|sh|116;13|gj|211|22|5|490|sh|150|sh|159|sh|181;14|gj|211|26|1|181|sh|181;111|gj|211|18|9|264|sh|50|sh|49|sh|49|sh|52|sh|64;121|gj|221|18|10|243|sh|50|sh|49|sh|49|sh|48|sh|47;121|jn|101009|28|221|sh|93;221|gj|121|18|10|243|sh|50|sh|49|sh|49|sh|48|sh|47;221|jn|101009|28|121|sh|93;13|mv|4|56|221|27|5;14|mv|4|56|221|27|5;13|gj|221|32|4|316|sh|155|sh|161;13|jn|101009|36|221|sh|339;25|gj|11|36|1|10|sh|10;11|mv|30|70|221|27|11;12|mv|30|70|221|27|11;111|mv|30|70|221|27|11;111|jn|101009|38|221|sh|144;221|gj|121|30|9|155|sh|45|sh|44|sh|35|sh|25|sh|6;11|gj|221|38|1|122|sh|122;12|gj|221|38|1|126|sh|126;13|gj|221|38|1|193|sh|193;14|gj|221|32|7|678|sh|155|sh|161|sh|169|sh|193;121|gj|221|30|9|248|sh|45|sh|44|sh|46|sh|50|sh|63\",\"maxRound\":38},\"attacker\":{\"member\":\"1|107010|3000|3000|0|40;2|107010|3000|3000|0|30;3|107020|3000|3000|0|20;4|107020|3000|3000|0|10;11|107010|2100|2100|-30|40;21|107010|2100|2100|30|40\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\"},\"defender\":{\"member\":\"1|107010|2100|2100|0|100;11|107010|2100|2100|-30|100;21|107010|2100|2100|30|100;5|107800|2100|2100|60|140\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\"}}";
//    
//    std::string strReport5 = "{\"battlereport\":{\"winside\":-1,\"battleType\":0,\"report\":\"25|gj|11|0|1|0|sh|0;xj|107900|11|sh|0|107910|11|sh|0;11|mv|0|70|21|1|5;21|mv|0|70|11|1|5;xj|107900|11|sh|0|107910|11|sh|0;11|gj|21|6|10|270|sh|56|sh|55|sh|54|sh|53|sh|52;11|jn|101009|16|21|sh|104;21|gj|11|6|10|270|sh|56|sh|55|sh|54|sh|53|sh|52;21|jn|101009|16|11|sh|104;xj|107900|11|sh|0|107910|11|sh|0;25|gj|11|18|1|0|sh|0;xj|107900|11|sh|0|107910|11|sh|0;11|gj|21|18|10|243|sh|50|sh|49|sh|49|sh|48|sh|47;11|jn|101009|28|21|sh|93;21|gj|11|18|10|243|sh|50|sh|49|sh|49|sh|48|sh|47;21|jn|101009|28|11|sh|93;xj|107900|11|sh|0|107910|11|sh|0;xj|107900|11|sh|0|107910|11|sh|0;25|gj|11|36|1|0|sh|0;11|gj|21|30|10|216|sh|45|sh|44|sh|43|sh|42|sh|42;11|jn|101009|40|21|sh|83;21|gj|11|30|10|216|sh|45|sh|44|sh|43|sh|42|sh|42;21|jn|101009|40|11|sh|83;xj|107900|11|sh|0|107910|11|sh|0;xj|107900|11|sh|0|107910|11|sh|0;11|gj|21|42|10|191|sh|40|sh|39|sh|38|sh|37|sh|37;11|jn|101009|52|21|sh|72;21|gj|11|42|10|191|sh|40|sh|39|sh|38|sh|37|sh|37;21|jn|101009|52|11|sh|72;xj|107900|11|sh|0|107910|11|sh|0;25|gj|11|54|1|1|sh|1;xj|107900|11|sh|0|107910|11|sh|0;11|gj|21|54|10|165|sh|34|sh|34|sh|33|sh|32|sh|32;11|jn|101009|64|21|sh|62;21|gj|11|54|10|165|sh|34|sh|34|sh|33|sh|32|sh|32;21|jn|101009|64|11|sh|62;xj|107900|11|sh|0|107910|11|sh|0;xj|107900|11|sh|0|107910|11|sh|0;25|gj|11|72|1|1|sh|1;11|gj|21|66|10|141|sh|30|sh|29|sh|28|sh|27|sh|27;11|jn|101009|76|21|sh|53;21|gj|11|66|10|141|sh|30|sh|29|sh|28|sh|27|sh|27;21|jn|101009|76|11|sh|53;xj|107900|11|sh|0|107910|11|sh|0;xj|107900|11|sh|0|107910|11|sh|0;11|gj|21|78|10|117|sh|25|sh|24|sh|23|sh|23|sh|22;11|jn|101009|88|21|sh|43;21|gj|11|78|10|117|sh|25|sh|24|sh|23|sh|23|sh|22;21|jn|101009|88|11|sh|43;xj|107900|11|sh|0|107910|11|sh|0;25|gj|11|90|1|1|sh|1;xj|107900|11|sh|0|107910|11|sh|0;11|gj|21|90|10|92|sh|20|sh|19|sh|18|sh|18|sh|17;11|jn|101009|100|21|sh|33;21|gj|11|90|10|93|sh|20|sh|19|sh|19|sh|18|sh|17;21|jn|101009|100|11|sh|34\",\"maxRound\":100},\"attacker\":{\"member\":\"1|107010|2100|2100|0|40\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\"},\"defender\":{\"member\":\"1|107010|2100|2100|0|100;5|107800|2100|2100|60|140\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\"}}";
//    
//    std::string strReport6 = "{\"battlereport\":{\"winside\":0,\"battleType\":0,\"report\":\"25|gj|11|0|1|0|sh|0;26|gj|11|0|1|0|sh|0;11|mv|0|70|21|1|5;21|mv|0|70|11|1|5;111|mv|-30|70|211|1|5;211|mv|-30|70|111|1|5;11|gj|21|6|10|270|sh|56|sh|55|sh|54|sh|53|sh|52;11|jn|101009|16|21|sh|104;21|gj|11|6|10|270|sh|56|sh|55|sh|54|sh|53|sh|52;21|jn|101009|16|11|sh|104;111|gj|211|6|10|270|sh|56|sh|55|sh|54|sh|53|sh|52;111|jn|101009|16|211|sh|104;211|gj|111|6|10|270|sh|56|sh|55|sh|54|sh|53|sh|52;211|jn|101009|16|111|sh|104;25|gj|11|18|1|0|sh|0;26|gj|11|18|1|0|sh|0;11|gj|21|18|10|243|sh|50|sh|49|sh|49|sh|48|sh|47;11|jn|101009|28|21|sh|93;21|gj|11|18|10|243|sh|50|sh|49|sh|49|sh|48|sh|47;21|jn|101009|28|11|sh|93;111|gj|211|18|10|243|sh|50|sh|49|sh|49|sh|48|sh|47;111|jn|101009|28|211|sh|93;211|gj|111|18|10|243|sh|50|sh|49|sh|49|sh|48|sh|47;211|jn|101009|28|111|sh|93;25|gj|11|36|1|0|sh|0;26|gj|11|36|1|0|sh|0;11|gj|21|30|10|216|sh|45|sh|44|sh|43|sh|42|sh|42;11|jn|101009|40|21|sh|83;21|gj|11|30|10|216|sh|45|sh|44|sh|43|sh|42|sh|42;21|jn|101009|40|11|sh|83;111|gj|211|30|10|216|sh|45|sh|44|sh|43|sh|42|sh|42;111|jn|101009|40|211|sh|83;211|gj|111|30|10|216|sh|45|sh|44|sh|43|sh|42|sh|42;211|jn|101009|40|111|sh|83;11|gj|21|42|10|191|sh|40|sh|39|sh|38|sh|37|sh|37;11|jn|101009|52|21|sh|72;21|gj|11|42|10|191|sh|40|sh|39|sh|38|sh|37|sh|37;21|jn|101009|52|11|sh|72;111|gj|211|42|10|191|sh|40|sh|39|sh|38|sh|37|sh|37;111|jn|101009|52|211|sh|72;211|gj|111|42|10|191|sh|40|sh|39|sh|38|sh|37|sh|37;211|jn|101009|52|111|sh|72;25|gj|11|54|1|1|sh|1;26|gj|11|54|1|1|sh|1;11|gj|21|54|10|165|sh|34|sh|34|sh|33|sh|32|sh|32;11|jn|101009|64|21|sh|62;21|gj|11|54|10|165|sh|34|sh|34|sh|33|sh|32|sh|32;21|jn|101009|64|11|sh|62;111|gj|211|54|10|165|sh|34|sh|34|sh|33|sh|32|sh|32;111|jn|101009|64|211|sh|62;211|gj|111|54|10|165|sh|34|sh|34|sh|33|sh|32|sh|32;211|jn|101009|64|111|sh|62;25|gj|11|72|1|1|sh|1;26|gj|11|72|1|1|sh|1;11|gj|21|66|10|140|sh|29|sh|29|sh|28|sh|27|sh|27;11|jn|101009|76|21|sh|52;21|gj|11|66|10|142|sh|30|sh|29|sh|28|sh|28|sh|27;21|jn|101009|76|11|sh|53;111|gj|211|66|10|141|sh|30|sh|29|sh|28|sh|27|sh|27;111|jn|101009|76|211|sh|53;211|gj|111|66|10|141|sh|30|sh|29|sh|28|sh|27|sh|27;211|jn|101009|76|111|sh|53;11|gj|21|78|10|115|sh|24|sh|24|sh|23|sh|22|sh|22;11|jn|101009|88|21|sh|43;21|gj|11|78|10|118|sh|25|sh|24|sh|24|sh|23|sh|22;21|jn|101009|88|11|sh|44;111|gj|211|78|10|117|sh|25|sh|24|sh|23|sh|23|sh|22;111|jn|101009|88|211|sh|43;211|gj|111|78|10|117|sh|25|sh|24|sh|23|sh|23|sh|22;211|jn|101009|88|111|sh|43;25|gj|11|90|1|1|sh|1;26|gj|11|90|1|1|sh|1;11|gj|21|90|10|91|sh|20|sh|19|sh|18|sh|17|sh|17;11|jn|101009|100|21|sh|32;21|gj|11|90|10|95|sh|20|sh|20|sh|19|sh|18|sh|18;21|jn|101009|100|11|sh|35;111|gj|211|90|10|93|sh|20|sh|19|sh|19|sh|18|sh|17;111|jn|101009|100|211|sh|34;211|gj|111|90|10|93|sh|20|sh|19|sh|19|sh|18|sh|17;211|jn|101009|100|111|sh|34\",\"maxRound\":100},\"attacker\":{\"member\":\"1|107010|2100|2100|0|40;11|107010|2100|2100|-30|40\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\"},\"defender\":{\"member\":\"1|107010|2100|2100|0|100;11|107010|2100|2100|-30|100;5|107800|2100|2100|-60|150;6|107800|2100|2100|50|150\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\"}}";
//    
//
//    std::string strReport11 = "{\"battlereport\":{\"winside\":1,\"battleType\":0,\"report\":\"2_1_3|gj|1_1_1|0|0|36|sh|36;2_1_4|gj|1_1_1|0|0|36|sh|36;2_1_2|mv|0|100|1_1_1|1|1;1_1_1|mv|0|50|2_1_1|1|1;2_1_1|mv|0|90|1_1_1|1|1;2_1_2|gj|1_1_1|2|0|2|sh|2\",\"maxRound\":2},\"attacker\":{\"member\":\"1|107000|50|50|0|40\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\",\"general\":\"\"},\"defender\":{\"member\":\"1|107100|150|150|0|100;2|107200|85|85|0|110;3|107800|100|100|-30|130;4|107800|100|100|30|130\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\",\"general\":\"1|240020|4\"}}";
//    
//    std::string strReport12 = "{\"battlereport\":{\"winside\":0,\"battleType\":0,\"report\":\"1_2_1|sk|102020|0|2_1_1|sh|40|2_1_2|sh|40|2_1_3|sh|40;1_2_1|sk|102020|2|2_1_1|sh|40|2_1_2|sh|40|2_1_3|sh|40;1_1_1|mv|0|20|2_1_1|1|3;1_2_1|sk|102020|6|2_1_1|sh|40|2_1_2|sh|40|2_1_3|sh|40;2_2_1|sk|102020|6|1_1_1|sh|40|1_1_2|sh|40|1_1_3|sh|40;2_2_2|sk|102020|8|1_1_1|sh|40|1_1_2|sh|40|1_1_3|sh|40;1_2_2|sk|102020|10|2_1_1|sh|40|2_1_2|sh|40|2_1_3|sh|40;1_1_1|mv|0|70|2_1_1|5|9;2_1_1|mv|0|70|1_1_1|1|13;1_1_2|mv|0|70|2_1_1|1|13;2_1_2|mv|0|70|1_1_1|1|13;1_1_3|mv|0|70|2_1_1|1|13;2_1_3|mv|0|70|1_1_1|1|13;1_1_4|mv|0|70|2_1_1|1|13;2_1_4|mv|0|70|1_1_1|1|13;1_1_5|mv|0|70|2_1_1|1|13;2_1_5|mv|0|70|1_1_1|1|13;1_2_1|sk|102020|14|2_1_1|sh|40|2_1_2|sh|40|2_1_3|sh|40;1_2_2|sk|102020|14|2_1_1|sh|40|2_1_2|sh|40|2_1_3|sh|40;1_1_1|gj|2_1_1|14|3|14|sh|14|sh|0;2_1_1|gj|1_1_1|14|2|13|sh|13;1_1_2|gj|2_1_1|14|4|15|sh|15|sh|0;2_1_2|gj|1_1_1|14|4|28|sh|14|sh|14;1_1_3|gj|2_1_1|14|4|23|sh|23|sh|0;2_1_3|gj|1_1_1|14|4|40|sh|20|sh|20;1_1_4|gj|2_1_1|14|4|48|sh|48|sh|0;2_1_4|gj|1_1_1|14|4|97|sh|47|sh|50;1_1_5|gj|2_1_1|14|4|82|sh|82|sh|0;2_1_5|gj|1_1_1|14|4|165|sh|80|sh|85;2_1_2|gj|1_1_2|18|3|18|sh|10|sh|8;2_2_2|sk|102020|20|1_1_2|sh|40|1_1_3|sh|40|1_1_4|sh|40;1_1_2|gj|2_1_2|18|4|27|sh|13|sh|14;1_1_3|gj|2_1_2|18|4|42|sh|19|sh|23;1_1_4|gj|2_1_2|18|4|86|sh|40|sh|46;1_1_5|gj|2_1_2|18|4|146|sh|69|sh|77;1_1_2|gj|2_1_3|22|1|4|sh|4;2_1_3|gj|1_1_2|18|6|48|sh|14|sh|15|sh|19;2_1_4|gj|1_1_2|18|6|122|sh|37|sh|39|sh|46;2_1_5|gj|1_1_2|18|6|209|sh|65|sh|68|sh|76;2_2_1|sk|102020|24|1_1_3|sh|40|1_1_4|sh|40|1_1_5|sh|40;2_1_3|gj|1_1_3|24|3|11|sh|6|sh|5;1_2_1|sk|102020|26|2_1_4|sh|40|2_1_5|sh|40;1_1_3|gj|2_1_3|22|6|30|sh|9|sh|10|sh|11;1_1_4|gj|2_1_3|22|6|67|sh|20|sh|22|sh|25;1_1_4|jn|102000|28|2_1_1|bj|0|2_1_2|bj|0;1_1_5|gj|2_1_3|22|6|123|sh|38|sh|41|sh|44;1_1_3|gj|2_1_4|28|3|32|sh|17|sh|15;2_2_2|sk|102020|30|1_1_4|sh|40|1_1_5|sh|40;2_1_4|gj|1_1_3|24|8|82|sh|19|sh|21|sh|21|sh|21;2_1_5|gj|1_1_3|24|8|151|sh|35|sh|38|sh|38|sh|40;2_1_4|gj|1_1_4|32|3|45|sh|31|sh|14;2_2_1|sk|102020|34|1_1_4|sh|40|1_1_5|sh|40;1_1_4|gj|2_1_4|30|6|130|sh|44|sh|44|sh|42;1_1_5|gj|2_1_4|28|8|319|sh|77|sh|79|sh|78|sh|85;1_1_4|gj|2_1_5|36|1|11|sh|11;2_1_5|gj|1_1_4|32|6|245|sh|78|sh|81|sh|86;1_2_2|sk|102020|38|2_1_5|sh|40;2_1_5|gj|1_1_5|38|4|82|sh|44|sh|38;2_1_5|jn|102000|42|1_1_1|bj|0|1_1_2|bj|0;2_2_2|sk|102020|46|1_1_5|sh|40;2_1_5|gj|1_1_5|44|9|129|sh|33|sh|29|sh|26|sh|23|sh|18;1_1_5|gj|2_1_5|36|17|311|sh|40|sh|41|sh|39|sh|37|sh|38|sh|36|sh|29|sh|27|sh|24\",\"maxRound\":52},\"attacker\":{\"member\":\"1|107000|400|400|0|0;2|107001|400|400|0|0;3|107003|400|400|0|0;4|107002|400|400|0|0;5|107004|400|400|0|0\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\",\"general\":\"1|240000|1;2|240001|1\"},\"defender\":{\"member\":\"1|107000|400|400|0|140;2|107001|400|400|0|140;3|107003|400|400|0|140;4|107002|400|400|0|140;5|107004|400|400|0|140\",\"playerlevel\":1,\"playername\":\"\",\"playerid\":\"\",\"general\":\"1|240000|1;2|240001|1\"}}";
//        
//    //m_report = strReport1;
//    Json* jReport = Json_create(m_report.c_str());
//    Json* attack = Json_getItem(jReport,"attacker");
//    BattlePlayer* attacker = this->createBPlayer(attack,0);
//    BattleObjectManager::shared()->setAttacker(attacker);
//    attacker->release();
//    
//    Json* def = Json_getItem(jReport,"defender");
//    BattlePlayer* defender = this->createBPlayer(def,1);
//    BattleObjectManager::shared()->setDefender(defender);
//    defender->release();
//    
//    //Json* rewardJson = Json_getItem(jReport,"reward");
//    if(BattleObjectManager::shared()->getRewardSpecialItems()==NULL){
//        BattleObjectManager::shared()->setRewardSpecialItems(CCArray::create());
//    }
//    BattleObjectManager::shared()->getRewardSpecialItems()->removeAllObjects();
//    if(BattleObjectManager::shared()->getBattleRewardRes()==NULL){
//        BattleObjectManager::shared()->setBattleRewardRes(CCArray::create());
//    }else{
//        BattleObjectManager::shared()->getBattleRewardRes()->removeAllObjects();
//    }
//    
//    if(BattleObjectManager::shared()->getDefGenerals()==NULL){
//        BattleObjectManager::shared()->setDefGenerals(CCArray::create());
//    }else{
//        BattleObjectManager::shared()->getDefGenerals()->removeAllObjects();
//    }
//        
//    Json* battleR=Json_getItem(jReport,"battlereport");
//    std::string battleReport = Json_getString(battleR,"report","");
//    
//    BattleObjectManager::shared()->setWinside(Json_getInt(battleR,"winside",0));
//    BattleObjectManager::shared()->setBattleType(Json_getInt(battleR,"battleType",0));
//    BattleObjectManager::shared()->setMaxRound(Json_getInt(battleR,"maxRound",0));
//    BattleObjectManager::shared()->setFround(Json_getInt(battleR,"fround",0));
//    BattleObjectManager::shared()->setAttForces(Json_getInt(battleR,"attlost",0));
//    BattleObjectManager::shared()->setDefForces(Json_getInt(battleR,"deflost",0));
//    BattleObjectManager::shared()->setAttRemainForces(Json_getInt(battleR,"attRemainForces",0));
//    BattleObjectManager::shared()->setDefRemainForces(Json_getInt(battleR,"defRemainForces",0));
//    
//    std::vector<std::string> reportItems;
//    CCCommonUtils::splitString(battleReport,";",reportItems);
//    int size = reportItems.size();
//    std::vector<std::string> itemArr;
//    if(BattleObjectManager::shared()->getBattleSequences()==NULL){
//        BattleObjectManager::shared()->setBattleSequences(CCArray::create());
//    }
//    CCArray* battleSequences = BattleObjectManager::shared()->getBattleSequences();
//    battleSequences->removeAllObjects();
//    int num = Json_getInt(battleR,"maxRound",0)+2;//总序列
//    BattleSequenceObject* sequenece;
//    for(int i=0;i<num;i++){
//        sequenece = new BattleSequenceObject();
//        sequenece->setResults(CCArray::create());
//        battleSequences->addObject(sequenece);
//        sequenece->release();
//    }
//    int maxSequence = 0 ;
//    for(int i=0;i<size;i++){
//        itemArr.clear();
//        CCCommonUtils::splitString(reportItems[i],"|",itemArr);
//        
//        std::string info = itemArr[0];
//        std::vector<std::string> pros;
//        CCCommonUtils::splitString(info,"_",pros);
//        int side = 0;
//        int attArmyType = 0;
//        int index = 0;
//        if (pros.size()==3) {
//            side = atoi(pros[0].c_str())-1;//那一边的
//            attArmyType = atoi(pros[1].c_str())-1;
//            index = atoi(pros[2].c_str())-1;//兵位置的索引
//        }
//        std::string m_type = itemArr[1];//动作
//        std::string m_specialAction = itemArr[0];//陷阱
//        int startIndex = -1;
//        int sequenceNum = 0;
//        int targetSide = 0;
//        int targetIndex = 0;
//        int defArmyType = 0;
//        std::string skillId = "";
//        int x = 0;
//        int y = 0;
//        int m_value = 0;
//        std::string m_target = "";
//        std::vector<std::string> targetpros;
//        if(m_type=="mv"){
//            x = atoi(itemArr[2].c_str())-1;
//            y = atoi(itemArr[3].c_str())-1;
//            m_target = itemArr[4];//移动进攻的目标
//            CCCommonUtils::splitString(m_target,"_",targetpros);
//            targetSide = atoi(targetpros[0].c_str())-1;
//            defArmyType = atoi(targetpros[1].c_str())-1;
//            targetIndex = atoi(targetpros[2].c_str())-1;
//            startIndex = atoi(itemArr[5].c_str());//开始的回合索引
//            sequenceNum = atoi(itemArr[6].c_str());//回合数
//        }else if(m_type=="gj"){
//            m_target = itemArr[2];//进攻的目标
//            CCCommonUtils::splitString(m_target,"_",targetpros);
//            targetSide = atoi(targetpros[0].c_str())-1;
//            defArmyType = atoi(targetpros[1].c_str())-1;
//            targetIndex = atoi(targetpros[2].c_str())-1;
//            m_value = atoi(itemArr[5].c_str());// sh
//            startIndex = atoi(itemArr[3].c_str());//开始的回合索引
//            sequenceNum = atoi(itemArr[4].c_str());//回合数
//        }else if(m_type=="jn"){//13|jn|102022|28|23|sh|30;
//            m_target = itemArr[4];//进攻的目标
//            CCCommonUtils::splitString(m_target,"_",targetpros);
//            targetSide = atoi(targetpros[0].c_str())-1;
//            defArmyType = atoi(targetpros[1].c_str())-1;
//            targetIndex = atoi(targetpros[2].c_str())-1;
//            m_value = atoi(itemArr[6].c_str());// sh
//            startIndex = atoi(itemArr[3].c_str());//开始的回合索引
//            sequenceNum = 1;
//            skillId = itemArr[2];
//        }else if(m_specialAction=="xj"){//xj|4|107900|13|sh|0
//            m_type = m_specialAction;
//            m_target = itemArr[3];//进攻的目标
//            CCCommonUtils::splitString(m_target,"_",targetpros);
//            targetSide = atoi(targetpros[0].c_str())-1;
//            defArmyType = atoi(targetpros[1].c_str())-1;
//            targetIndex = atoi(targetpros[2].c_str())-1;
//            m_value = atoi(itemArr[5].c_str());// sh
//            startIndex = atoi(itemArr[1].c_str());//开始的回合索引
//            sequenceNum = 1;
//            skillId = itemArr[2];
//        }else if(m_type=="sk"){//1_2_2|sk|102020|0|2_1_1|sh|40|2_1_2|sh|40
//            m_target = itemArr[4];//进攻的目标
//            startIndex = atoi(itemArr[3].c_str());//开始的回合索引
//            sequenceNum = 1;
//            skillId = itemArr[2];
//        }
//
//        int temp = startIndex+sequenceNum;
//        if(temp>maxSequence){
//            maxSequence = temp;
//            BattleObjectManager::shared()->setMaxRound(maxSequence);
//        }
//        BattleResult* res = new BattleResult();
//        res->m_side = side;
//        res->m_index = index;
//        res->m_attArmyType = attArmyType;
//        res->m_defArmyType = defArmyType;
//        res->m_type = m_type;
//        res->m_skillId = skillId;
////        BattleGrid* grid = new BattleGrid(x,y);
////        res->setAttackPos(grid);
////        grid->release();
//        //lable->setPosition(ccp((j-1)*250, (i-5)*80));
//        res->m_time = sequenceNum;
//        res->m_targetSide = targetSide;
//        res->m_targetIndex = targetIndex;
//        res->m_sequenceIndex = startIndex;
//        res->m_attack = 1;
//        res->m_value = m_value;
//        if(startIndex>battleSequences->count()){
//            sequenece = new BattleSequenceObject();
//            sequenece->setResults(CCArray::create());
//            battleSequences->addObject(sequenece);
//            sequenece->release();
//        }
//        if(startIndex<0) continue;
//        sequenece = (BattleSequenceObject*)battleSequences->objectAtIndex(startIndex);
////        if(sequenece!=NULL){
////            sequenece->getResults()->addObject(res);
////        }
//        res->setHurtList(CCArray::create());
//        if(itemArr.size()>6 && m_type!="jn" && m_type!="sk"){
//            std::string hurt_type = itemArr[6];
//            if(hurt_type=="sh" || hurt_type=="bj"){
//                int len = itemArr.size()-1;
//                for (int i=6; i<len && len-i>=1;i=i+2) {
//                    std::string h_type = itemArr[i];
//                    if(h_type=="sh" || h_type=="bj"){
//                        if(itemArr.size()<=(i+1)) continue;
//                        std::string value = itemArr[i+1];
//                        SequenceResult* sr = new SequenceResult();
//                        sr->m_index = i+res->m_sequenceIndex;
//                        if(value==""){
//                            sr->m_value = 0;
//                        }else{
//                            sr->m_value = atoi(value.c_str());
//                        }
//                        sr->m_type = h_type;
//                        res->getHurtList()->addObject(sr);
//                        sr->release();
//                    }
//                }
//            }
//        }
//        res->setSkillhHurtEffs(CCArray::create());
//        if(m_type=="sk"){//1_2_2|sk|102020|0|2_1_1|sh|40|2_1_2|sh|40
//            int len = itemArr.size()-1;
//            for (int i=4; i<len && len-i>=2;i=i+3) {
//                m_target = itemArr[i];//进攻的目标
//                targetpros.clear();
//                CCCommonUtils::splitString(m_target,"_",targetpros);
//                int side = atoi(targetpros[0].c_str())-1;
//                int armyType = atoi(targetpros[1].c_str())-1;
//                int pos = atoi(targetpros[2].c_str())-1;
//                std::string hurtType = itemArr[i+1];
//                int value = atoi(itemArr[i+2].c_str());
//                SkillHurtResult* hr = new SkillHurtResult(side,armyType,pos,hurtType,value);
//                res->getSkillhHurtEffs()->addObject(hr);
//                hr->release();
//            }
//
//            sequenceNum = 1;
//            skillId = itemArr[2];
//        }
////        res->release();
//    }
//    Json_dispose(jReport);
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
}


//BattlePlayer* BattleManager::createBPlayer(Json* json,int side){
//    if(json==NULL) return NULL;
//    BattlePlayer* player = new BattlePlayer();
//    player->setPlayerid(Json_getString(json,"playerid",""));
//    player->setPlayername(Json_getString(json,"playername",""));
//    player->setPlayerlevel(Json_getInt(json,"playerlevel",1));
//    player->setPic(Json_getString(json,"pic",""));
//    player->setSide(side);
//    CCArray* mArr = CCArray::create();
//    player->setMember(mArr);
//    
//    Json* arenaJson = Json_getItem(json,"arenaRWD");
//    if(arenaJson!=NULL){
//        player->setReputation(Json_getInt(arenaJson,"reputation",0));
//    }
//    std::string member = Json_getString(json,"member","");
//    std::vector<std::string> heros;
//    CCCommonUtils::splitString(member,";",heros);
//    int size = heros.size();
//    BattleHero* hero;
//    std::vector<std::string> heroPros;
//    for(int i=0;i<size;i++){
//        heroPros.clear();
//        CCCommonUtils::splitString(heros[i],"|",heroPros);
//        hero = new BattleHero();
//        hero->setPostion(atoi(heroPros[0].c_str())-1);
//        //hero->setColor(atoi(CCCommonUtils::getPropById(heroPros[1],"color").c_str()));
//        hero->setArm(atoi(heroPros[1].c_str()));//
//        hero->setInitForces(atoi(heroPros[2].c_str()));
//        hero->setCurrForces(hero->getInitForces());
//        hero->setMaxForces(atoi(heroPros[3].c_str()));
//        BattleGrid* grid = new BattleGrid(atoi(heroPros[4].c_str())-1,atoi(heroPros[5].c_str())-1);
//        hero->setGrid(grid);
//        grid->release();
//        mArr->addObject(hero);
//        hero->release();
//    }
//    
//    CCArray* mGeneral = CCArray::create();
//    player->setGenerals(mGeneral);
//    std::vector<std::string> generals;
//     std::string generalStr = Json_getString(json,"general","");
//    CCCommonUtils::splitString(generalStr,";", generals);
//    size = generals.size();
//    std::vector<std::string> gvector;
//    for(int i=0;i<size;i++){
//        gvector.clear();
//        CCCommonUtils::splitString(generals[i],"|", gvector);
//        hero = new BattleHero();
//        hero->setPostion(atoi(gvector[0].c_str())-1);
//        hero->setArm(atoi(gvector[1].c_str()));//
//        hero->setInitForces(9999);
//        hero->setCurrForces(9999);
//        hero->setMaxForces(9999);
//        hero->setLevel(atoi(gvector[2].c_str()));
//        mGeneral->addObject(hero);
//        hero->release();
//    }
//    
//    return player;
//}
