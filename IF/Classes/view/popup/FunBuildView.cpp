//
//  FunBuildView.cpp
//  IF
//
//  Created by fubin on 14-5-26.
//
//

#include "FunBuildView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "BuildMoreInfoView.h"
#include "ToolController.h"
#include "StoreView.h"
#include "UseResToolView.h"
#include "ArmyController.h"
#include "TipsView.h"
#include "BuildUpgradeView.h"
#include "QueueController.h"
#include "TroopsController.h"

FunBuildView* FunBuildView::create(int buildId){
    FunBuildView* ret = new FunBuildView();
    if(ret && ret->init(buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FunBuildView::init(int buildId)
{
    if (!ArcPopupBaseView::init(TYPE_POS_TO_UP)) {
        return false;
    }
    setIsHDPanel(true);
    m_qid = QID_MAX;
    m_buildId = buildId;
    auto tmpCCB = CCBLoadFile("ResBuildView02",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    m_upBtnMsgLabel->setString(_lang("102105").c_str());//more info
    m_btnMsgLabel->setString(_lang("102121").c_str());//destroy
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    string key = CC_ITOA(m_buildId);
    int qid = QueueController::getInstance()->getQueueQidByKey(key);
    if (qid > 0) {
        m_qid = qid;
    }
    
    updateInfo();
    
    m_openNum=0;
//    m_mainNode->setPositionY(m_mainNode->getPositionY()-200);
//    CCActionInterval * moveBy2 = CCMoveBy::create(0.25, ccp(0,200));
    
//    m_mainNode->setScale(0);
//    CCActionInterval * delay = CCDelayTime::create(0);
//    CCActionInterval * scaleTo = CCScaleTo::create(0.25, 1.0);
//    m_mainNode->runAction(CCSequence::create(delay,scaleTo, NULL));
    
    vector<CCNode*> tmpVec;
    CCCommonUtils::getRGBAProtocols(m_mainNode, &tmpVec);
    for (int i=0; i<tmpVec.size(); i++) {
        CCActionInterval * fadeIn = CCFadeIn::create(0.5);
        tmpVec[i]->runAction(fadeIn);
    }
    
    return true;
}

void FunBuildView::updateInfo()
{
    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    m_nameLabel->setString(_lang(m_info.name));
    string title = _lang_1("102272", CC_ITOA(m_info.level));
    if(m_info.level>=GlobalData::shared()->MaxBuildLv) {
        title += _lang("102183");
    }
    m_lvLabel->setString(title);
    m_nextLvLabel->setString("");
    m_nextLabel->setString("");
    m_nextMsgLabel->setString(_lang(m_info.description));
    float fh = m_nextMsgLabel->getContentSize().height;
    if (!CCCommonUtils::isIosAndroidPad())
    {
        if (fh>345) {
            m_nextMsgLabel->setFontSize(16);
        }
    } else {
        if (fh > 390) {
            m_nextMsgLabel->setFontSize(30);
        }
    }
    
    
    int curX = 30;
    int curY = 0;
    int _itemH = 60;//110
    if (CCCommonUtils::isIosAndroidPad())
    {
        _itemH = 87;
        curX = 60;
        curY = 0;
    }
    
    string addMsg = "";
    
    if (m_info.type == FUN_BUILD_CELLAR){
        int mlv = FunBuildController::getInstance()->getMainCityLv();
        int addValue = CCCommonUtils::getEffectValueByNum(72);
        float proValue = 1+CCCommonUtils::getEffectValueByNum(38)/100.0;
        string addInfo = "";
        
        if (mlv>=FunBuildController::getInstance()->building_base_k4) {
            int oStoneValue = atoi(m_info.para[2].c_str());
            auto item2 = BuildResCell::create(Stone, CCCommonUtils::getResourceNameByType(Stone), oStoneValue);
            addInfo =addInfo+"+"+CC_CMDITOA((oStoneValue+addValue)*proValue-oStoneValue);
            item2->setOtherInfo(addInfo);
            item2->setPosition(ccp(curX, curY));
            m_scrollView->addChild(item2);
            curY += _itemH;
            addInfo = "";
        }
        
        if (mlv>=FunBuildController::getInstance()->building_base_k3) {
            int oIronValue = atoi(m_info.para[3].c_str());
            auto item4 = BuildResCell::create(Iron, CCCommonUtils::getResourceNameByType(Iron), oIronValue);
            addInfo =addInfo+"+"+CC_CMDITOA((oIronValue+addValue)*proValue-oIronValue);
            item4->setOtherInfo(addInfo);
            item4->setPosition(ccp(curX, curY));
            m_scrollView->addChild(item4);
            curY += _itemH;
            addInfo = "";
        }
        
        int oFoodValue = atoi(m_info.para[4].c_str());
        auto item3 = BuildResCell::create(Food, CCCommonUtils::getResourceNameByType(Food), oFoodValue);
        addInfo =addInfo+"+"+CC_CMDITOA((oFoodValue+addValue)*proValue-oFoodValue);
        item3->setOtherInfo(addInfo);
        item3->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item3);
        curY += _itemH;
        addInfo = "";
        
        int oWoodValue = atoi(m_info.para[1].c_str());
        auto item1 = BuildResCell::create(Wood, CCCommonUtils::getResourceNameByType(Wood), oWoodValue);
        addInfo =addInfo+"+"+CC_CMDITOA((oWoodValue+addValue)*proValue-oWoodValue);
        item1->setOtherInfo(addInfo);
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        addInfo = "";
        curY += _itemH;
        if (CCCommonUtils::isIosAndroidPad()) {
            curY -= 30;
        } else {
            curY -= 20;
        }
        
        auto titleItem = UpgradeTitleCell::create(_lang("102497"));
        titleItem->setPosition(ccp(curX, curY));
        m_scrollView->addChild(titleItem);
        if (CCCommonUtils::isIosAndroidPad()) {
            curY += _itemH - 30;
            curY -= 30;
        } else {
            curY += _itemH-20;
            
            curY -= 20;
        }
    }
    else if (m_info.type == FUN_BUILD_ARROW_TOWER) {
        vector<string> vecItems;
        CCCommonUtils::splitString(m_info.information, "|", vecItems);
        string tmpAtk = "";
        string tmpSpd = "";
        if (m_info.level < vecItems.size()) {
            string curMsg = vecItems[m_info.level];
            vector<string> cellItems;
            CCCommonUtils::splitString(curMsg, ";", cellItems);
            if (cellItems.size()>=3) {
                string atkStr = cellItems[1];
                string spdStr = cellItems[2];
                
                vector<string> tinyItem1;
                CCCommonUtils::splitString(atkStr, ",", tinyItem1);
                if (tinyItem1.size()==2) {
                    tmpAtk = _lang_1(tinyItem1[0],CC_CMDITOA(atoi(tinyItem1[1].c_str())).c_str()  );
                }
                
                vector<string> tinyItem2;
                CCCommonUtils::splitString(spdStr, ",", tinyItem2);
                if (tinyItem2.size()==2) {
                    tmpSpd = _lang_1(tinyItem2[0],CC_CMDITOA(atoi(tinyItem2[1].c_str())).c_str()  );
                }
            }
        }
        
        auto item2 = BuildResCell::create(Wood, _lang("102260"), 100, "attack_icon.png", tmpSpd); //攻速提升
        item2->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        auto item1 = BuildResCell::create(Wood, _lang("102259"), 100, "attack_icon.png", tmpAtk); //攻击提升
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        if (CCCommonUtils::isIosAndroidPad()) {
            curY -= 30;
        } else {
            curY -= 20;
        }
    }
    else if (m_info.type == FUN_BUILD_WAR) {
        if (!CCCommonUtils::isIosAndroidPad()) {
            auto label = CCLabelIF::create(_lang("108648").c_str(), 24, CCSizeMake(600, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY += 100;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY += _itemH;
            curY -= 20;
        } else {
            auto label = CCLabelIF::create(_lang("108648").c_str(), 40, CCSizeMake(1345, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY += 150;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY += _itemH;
            curY -= 30;
        }
        
    }
    else if (m_info.type == FUN_BUILD_SCIENE) {
//        string par1 = m_info.para[1];
//        string tmpInfo = _lang_1("102261", par1.c_str());
//        auto item1 = BuildResCell::create(Wood, _lang("102229"), 0, "science_icon.png", tmpInfo); //科技研究速度提升
//        item1->setPosition(ccp(curX, curY));
//        m_scrollView->addChild(item1);
//        curY += _itemH;
//        curY -= 20;
        if (!CCCommonUtils::isIosAndroidPad()) {
            auto label = CCLabelIF::create(_lang("102078").c_str(), 24, CCSizeMake(480, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY = m_infoList->getContentSize().height/2;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY = m_infoList->getContentSize().height;
        } else {
            auto label = CCLabelIF::create(_lang("102078").c_str(), 40, CCSizeMake(1300, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY = m_infoList->getContentSize().height/2;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY = m_infoList->getContentSize().height;
        }
        
        
        if (m_qid == QID_MAX) {
            m_qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
        }
    }
    else if (m_info.type == FUN_BUILD_FORGE) {
        string par2 = m_info.para[2];
        string tmpInfo2 = _lang_1("102205", par2.c_str());
        auto item2 = BuildResCell::create(Wood, _lang("102363"), atoi(par2.c_str()), "science_icon.png", tmpInfo2); //钢材消耗减少
        item2->offSetMsgX(30);
        item2->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        string par1 = m_info.para[1];
        string tmpInfo1 = _lang_1("102205", par1.c_str());
        auto item1 = BuildResCell::create(Wood, _lang("102362"), atoi(par1.c_str()), "science_icon.png", tmpInfo1); //锻造速度
        if (!CCCommonUtils::isIosAndroidPad()) {
            item1->offSetMsgX(30);
        } else {
            item1->offSetMsgX(70);
        }
        
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
    }
    else if (m_info.type == FUN_BUILD_STABLE) {
        string par3 = m_info.para[3];
        auto item3 = BuildResCell::create(Wood, _lang("102258"), atoi(par3.c_str()), "science_icon.png"); //援兵容纳数量
        addMsg = "";
        addMsg =addMsg+"+"+CC_CMDITOA(CCCommonUtils::getEffectValueByNum(141));
        item3->setOtherInfo(addMsg);
        item3->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item3);
        curY += _itemH;
        
        string par2 = m_info.para[2];
//        string tmpInfo = _lang_1("102205", par2.c_str());
        auto item2 = BuildResCell::create(Wood, _lang("102221"), atoi(par2.c_str()), "science_icon.png"); //帮助减少时间
        float eValue=CCCommonUtils::getEffectValueByNum(142)+CCCommonUtils::getEffectValueByNum(504)*0.01*atoi(par2.c_str());
        addMsg = "";
        addMsg =addMsg+"+"+CC_CMDITOA(eValue);
        item2->setOtherInfo(addMsg);
        item2->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        string par1 = m_info.para[1];
        auto item1 = BuildResCell::create(Wood, _lang("102218"), atoi(par1.c_str()), "science_icon.png"); //帮助次数
        eValue=CCCommonUtils::getEffectValueByNum(161);
        addMsg = "";
        addMsg =addMsg+"+"+CC_CMDITOA(eValue);
        item1->setOtherInfo(addMsg);
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
    }
    else if (m_info.type == FUN_BUILD_TAVERN) {
        string par2 = m_info.para[2];
        string tmpInfo = _lang_1("102205", par2.c_str());
        auto item2 = BuildResCell::create(Wood, _lang("102220"), atoi(par2.c_str()), "science_icon.png", tmpInfo, 3); //税率
        addMsg = "";
        addMsg =addMsg+"-"+CC_CMDITOA(CCCommonUtils::getEffectValueByNum(139))+"%";
        item2->setOtherInfo(addMsg);
        item2->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        string par1 = m_info.para[1];
        auto item1 = BuildResCell::create(Wood, _lang("102219"), atoi(par1.c_str()), "science_icon.png"); //商队负重上限
        addMsg = "";
        addMsg =addMsg+"+"+CC_CMDITOA(CCCommonUtils::getEffectValueByNum(140));
        item1->setOtherInfo(addMsg);
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
    }
    else if (m_info.type == FUN_BUILD_WALL) {
        string par2 = m_info.para[2];
        auto item2 = BuildResCell::create(Wood, _lang("102206"), atoi(par2.c_str()), "science_icon.png"); //城防值
        addMsg = "";
        addMsg = addMsg+"+"+CC_CMDITOA(CCCommonUtils::getEffectValueByNum(125));
        item2->setOtherInfo(addMsg);
        item2->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        string par1 = m_info.para[1];
        auto item1 = BuildResCell::create(Wood, _lang("102223"), atoi(par1.c_str()), "science_icon.png"); //陷阱容量
        addMsg = "";
        addMsg = addMsg+"+"+CC_CMDITOA(ArmyController::getInstance()->getMaxNumByType(FORT) - atoi(par1.c_str()));
        item1->setOtherInfo(addMsg);
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
    }
    else if (m_info.type == FUN_BUILD_HOSPITAL) {
        int sum = ArmyController::getInstance()->getMaxNumByType(TREAT_ARMY);
        int sumOri = ArmyController::getInstance()->getMaxNumByType(TREAT_ARMY, true);
        auto item2 = BuildResCell::create(Wood, _lang("102217"), sumOri, "science_icon.png", "", 4); //总伤兵容量
        addMsg = "";
        addMsg = addMsg+"+"+CC_CMDITOA(sum - sumOri);
        item2->setOtherInfo(addMsg);
        item2->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        int par1 = atoi(m_info.para[1].c_str());
        auto item1 = BuildResCell::create(Wood, _lang("102215"), par1, "science_icon.png"); //伤兵容量
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
        
        if (m_qid == QID_MAX) {
            m_qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_HOSPITAL);
        }
    }
    else if (m_info.type == FUN_BUILD_BARRACK) {
        int sum = ArmyController::getInstance()->getMaxNumByType(ARMY);
        auto item2 = BuildResCell::create(Wood, _lang("102214"), sum, "science_icon.png", "", 5); //总训练量
        addMsg = "";
        addMsg = addMsg+"+"+CC_CMDITOA(CCCommonUtils::getEffectValueByNum(131)+10);
        item2->setOtherInfo(addMsg);
        item2->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        string par1 = m_info.para[1];
        auto item1 = BuildResCell::create(Wood, _lang("102213"), atoi(par1.c_str()), "science_icon.png"); //训练量
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
    }
    else if (m_info.type == FUN_BUILD_MAIN) {
        
//        string par1 = m_info.para[1];
//        auto item1 = BuildResCell::create(Wood, _lang("102206"), atoi(par1.c_str()), "science_icon.png", "", 6); //城防值
//        item1->setPosition(ccp(curX, curY));
//        m_scrollView->addChild(item1);
//        curY += _itemH;
//        curY -= 20;
        if (!CCCommonUtils::isIosAndroidPad()) {
            auto label = CCLabelIF::create(_lang("102025").c_str(), 24, CCSizeMake(480, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY = m_infoList->getContentSize().height/2;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY = m_infoList->getContentSize().height;
        } else {
            auto label = CCLabelIF::create(_lang("102025").c_str(), 40, CCSizeMake(1300, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY = m_infoList->getContentSize().height/2;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY = m_infoList->getContentSize().height;
        }
    }
    else if (m_info.type == FUN_BUILD_TRAINFIELD) {
        int par1 = atoi(m_info.para[1].c_str());
        int sum = TroopsController::getInstance()->getMaxSoilder();
        auto item1 = BuildResCell::create(Wood, _lang("102228"), par1, "science_icon.png"); //出征士兵数
        addMsg = "";
        addMsg =addMsg+"+"+CC_CMDITOA(sum - par1);
        item1->setOtherInfo(addMsg);
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
    }
    else if (m_info.type == FUN_BUILD_SMITHY) {
        string par1 = m_info.para[1];
        auto item1 = BuildResCell::create(Wood, _lang("102311"), atoi(par1.c_str()), "science_icon.png");
        addMsg = "";
        addMsg =addMsg+"+"+CC_CMDITOA(CCCommonUtils::getEffectValueByNum(144));
        item1->setOtherInfo(addMsg);
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
    }
    else if(m_info.type == FUN_BUILD_WATCH_TOWER) {
        string cellInfo = _lang("102255");//m_info.getInfomationByLv(m_info.level);//这不用根据等级来取了
        if (!CCCommonUtils::isIosAndroidPad()) {
            auto label = CCLabelIF::create(cellInfo.c_str(), 24, CCSizeMake(480, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY = m_infoList->getContentSize().height/2;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY = m_infoList->getContentSize().height;
        } else {
            auto label = CCLabelIF::create(cellInfo.c_str(), 40, CCSizeMake(1300, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY = m_infoList->getContentSize().height/2;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY = m_infoList->getContentSize().height;
        }
    }
    else if(m_info.type == FUN_BUILD_BARRACK1 || m_info.type == FUN_BUILD_BARRACK2 || m_info.type == FUN_BUILD_BARRACK3 || m_info.type == FUN_BUILD_BARRACK4 || m_info.type == FUN_BUILD_FORT) {
        int maxLv = 99999;
        string armyName = "";
        vector<string> vecItems;
        CCCommonUtils::splitString(m_info.open_arms, "|", vecItems);
        string tmpAtk = "";
        string tmpSpd = "";
        for (int i=0; i<vecItems.size(); i++) {
            vector<string> cellItems;
            CCCommonUtils::splitString(vecItems[i], ";", cellItems);
            if (cellItems.size() == 2) {
                int lv = atoi(cellItems[0].c_str());
                if (lv>m_info.level && lv<maxLv) {
                    maxLv = lv;
                    armyName = CCCommonUtils::getNameById(cellItems[1]);
                }
            }
        }
        
        string cellInfo = "";
        if (armyName != "") {
            cellInfo = _lang_2("102262", CC_ITOA(maxLv), armyName.c_str());
        }
        auto item1 = BuildResCell::create(Wood, cellInfo, 0, "science_icon.png", " "); //新英雄解锁
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
        
        if (m_qid == QID_MAX) {
            int qType = CCCommonUtils::getQueueTypeByBuildType(m_info.type);
            if (qType>=0) {
                m_qid = QueueController::getInstance()->getMinTimeQidByType(qType);
            }
        }
    }
    else if (m_info.type==FUN_BUILD_SACRIFICE) {
        vector<string> vecItems;
        CCCommonUtils::splitString(m_info.information, "|", vecItems);
        vector<string> tmpNames;
        vector<string> tmpValues;
        
        if (m_info.level < vecItems.size()) {
            string curMsg = vecItems[m_info.level];
            CCCommonUtils::splitString(curMsg, ";", tmpValues);
            CCCommonUtils::splitString(vecItems[0], ";", tmpNames);
        }
        
        if (tmpNames.size()>=6 && tmpValues.size()>=6) {
            curY -= 5;
            vector<string> cellValues;
            int cellValue = 0;
            CCCommonUtils::splitString(tmpValues[5], ",", cellValues);
            if (cellValues.size()>=2) {
                cellValue = atoi(cellValues[1].c_str());
            }
            
            auto item5 = BuildResCell::create(Wood, _lang(tmpNames[5]), cellValue, "science_icon.png");
            item5->setPosition(ccp(curX, curY));
            m_scrollView->addChild(item5);
            curY += _itemH-7;
            
            cellValues.clear();
            cellValue = 0;
            CCCommonUtils::splitString(tmpValues[4], ",", cellValues);
            if (cellValues.size()>=2) {
                cellValue = atoi(cellValues[1].c_str());
            }
            auto item4 = BuildResCell::create(Stone, _lang(tmpNames[4]), cellValue);
            item4->setPosition(ccp(curX, curY));
            m_scrollView->addChild(item4);
            curY += _itemH-7;
            
            cellValues.clear();
            cellValue = 0;
            CCCommonUtils::splitString(tmpValues[3], ",", cellValues);
            if (cellValues.size()>=2) {
                cellValue = atoi(cellValues[1].c_str());
            }
            auto item3 = BuildResCell::create(Iron, _lang(tmpNames[3]), cellValue);
            item3->setPosition(ccp(curX, curY));
            m_scrollView->addChild(item3);
            curY += _itemH-7;
            
            cellValues.clear();
            cellValue = 0;
            CCCommonUtils::splitString(tmpValues[2], ",", cellValues);
            if (cellValues.size()>=2) {
                cellValue = atoi(cellValues[1].c_str());
            }
            auto item2 = BuildResCell::create(Food, _lang(tmpNames[2]), cellValue);
            item2->setPosition(ccp(curX, curY));
            m_scrollView->addChild(item2);
            curY += _itemH-7;
            
            cellValues.clear();
            cellValue = 0;
            CCCommonUtils::splitString(tmpValues[1], ",", cellValues);
            if (cellValues.size()>=2) {
                cellValue = atoi(cellValues[1].c_str());
            }
            auto item1 = BuildResCell::create(Wood, _lang(tmpNames[1]), cellValue);
            item1->setPosition(ccp(curX, curY));
            m_scrollView->addChild(item1);
            curY += _itemH-7;
        }
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
    }
    else if (m_info.type==FUN_BUILD_WOOD || m_info.type==FUN_BUILD_STONE || m_info.type==FUN_BUILD_IRON || m_info.type==FUN_BUILD_FOOD || m_info.type==FUN_BUILD_SILVER){
        int curNum = 0;
        int curStorage = 0;
        int outputPreHour = 0;
        int ResType = 0;
        string resName = "";
        int m_type = m_info.type;
        
        if (m_type == FUN_BUILD_WOOD) {
            curNum = GlobalData::shared()->resourceInfo.lWood;
            curStorage = GlobalData::shared()->resourceInfo.lStorageWood;
            outputPreHour = FunBuildController::getInstance()->WoodOutPreSed*3600;
            ResType = Wood;
        }
        else if (m_type == FUN_BUILD_STONE) {
            curNum = GlobalData::shared()->resourceInfo.lStone;
            curStorage = GlobalData::shared()->resourceInfo.lStorageStone;
            outputPreHour = FunBuildController::getInstance()->StoneOutPreSed*3600;
            ResType = Stone;
        }
        else if (m_type == FUN_BUILD_IRON) {
            curNum = GlobalData::shared()->resourceInfo.lIron;
            curStorage = GlobalData::shared()->resourceInfo.lStorageIron;
            outputPreHour = FunBuildController::getInstance()->IronOutPreSed*3600;
            ResType = Iron;
        }
        else if (m_type == FUN_BUILD_FOOD) {
            curNum = GlobalData::shared()->resourceInfo.lFood;
            curStorage = GlobalData::shared()->resourceInfo.lStorageFood;
            outputPreHour = FunBuildController::getInstance()->FoodOutPreSed*3600;
            ResType = Food;
        }
        else if (m_type == FUN_BUILD_SILVER) {
            curNum = GlobalData::shared()->resourceInfo.lMoney;
            curStorage = GlobalData::shared()->resourceInfo.lStorageMoney;
            outputPreHour = FunBuildController::getInstance()->SliverOutPreSed*3600;
            ResType = Silver;
        }
        
//        auto item3 = BuildResCell::create(ResType, _lang_1("102123", _lang(resName).c_str()), curStorage, "icon_deposit.png", "", 1);
//        item3->setPosition(ccp(curX, curY));
//        m_scrollView->addChild(item3);
//        curY += _itemH;
        float effValue = 0;
        if (ResType == Wood) {
            effValue = CCCommonUtils::getEffectValueByNum(50)*1.0/100;
        }
        else if (ResType == Food) {
            effValue = CCCommonUtils::getEffectValueByNum(53)*1.0/100;
        }
        else if (ResType == Iron) {
            effValue = CCCommonUtils::getEffectValueByNum(52)*1.0/100;
        }
        else if (ResType == Stone) {
            effValue = CCCommonUtils::getEffectValueByNum(51)*1.0/100;
        }
        
        int para2 = atoi(m_info.para[2].c_str());
        auto item3 = BuildResCell::create(ResType, _lang_1("102123", ""), para2, "icon_deposit.png", "", 1);
        addMsg = "";
        addMsg =addMsg+"+"+CC_CMDITOA(para2 * effValue);//para2 * CCCommonUtils::getEffectValueByNum(132)*1.0/100
        item3->setOtherInfo(addMsg);
        item3->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item3);
        curY += _itemH;
        
        int outPut = atoi(m_info.para[1].c_str());
        auto item1 = BuildResCell::create(ResType, _lang("102122"), outPut);
        int addValue = outPut*effValue;
        if (addValue>0) {
            string addInfo = "+";
            addInfo =addInfo+CC_CMDITOA(addValue);
            item1->setOtherInfo(addInfo);
        }
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
        
//        if (m_type == FUN_BUILD_FOOD) {
//            auto item0 = BuildResCell::create(ResType, _lang("102125"), ArmyController::getInstance()->getTotalUpKeep(), "", "", 2);
//            item0->setPosition(ccp(curX, curY));
//            m_scrollView->addChild(item0);
//            curY += _itemH;
//        }
//        
//        auto item2 = BuildResCell::create(ResType, _lang("102124"), outputPreHour);
//        item2->setPosition(ccp(curX, curY));
//        m_scrollView->addChild(item2);
//        curY += _itemH;
        
        if (!CCCommonUtils::isIosAndroidPad()) {
            curY -= 20;
        } else {
            curY -= 30;
        }
        
        m_resType = ResType;
    }
    else {
        if (!CCCommonUtils::isIosAndroidPad()){
            auto label = CCLabelIF::create(_lang("102192").c_str(), 24, CCSizeMake(480, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY = m_infoList->getContentSize().height/2;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY = m_infoList->getContentSize().height;
        } else {
            auto label = CCLabelIF::create(_lang("102192").c_str(), 40, CCSizeMake(1300, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY = m_infoList->getContentSize().height/2;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY = m_infoList->getContentSize().height;
        }
    }
    
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
    
    if(curY<=m_infoList->getContentSize().height) {
        m_scrollView->setTouchEnabled(false);
    }
    
//    m_qid = QID_MAX;
    if (m_qid == QID_MAX) {
        if (m_info.limitNum <= 1) {
            if (m_info.state == FUN_BUILD_NORMAL && m_info.type == FUN_BUILD_MAIN) {
                
                auto& toolInfo = ToolController::getInstance()->getToolInfoById(ITEM_MOVE_BUILD);
                if (toolInfo.itemId>0 && toolInfo.price > 0) {
                    m_destroyNode->setPositionX(-124);
                    m_moreNode->setPositionX(124);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        m_destroyNode->setPositionX(-316);
                        m_moreNode->setPositionX(312);
                    }
                    CCCommonUtils::setButtonSprite(m_instantBtn, "btn_green3.png");
                    m_btnMsgLabel->setString(_lang("104082").c_str());//移动建筑
                }else {
                    m_destroyNode->setVisible(false);
                    m_instantBtn->setEnabled(false);
                    m_moreNode->setPositionX(0);
                }
            }else {
                m_destroyNode->setVisible(false);
                m_instantBtn->setEnabled(false);
                m_moreNode->setPositionX(0);
            }
            
        }
        else {
            if (m_info.state != FUN_BUILD_NORMAL) {
                m_instantBtn->setEnabled(false);
            }
        }
    }
    else {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        if (qInfo.type == TYPE_BUILDING) {
            if (m_info.state == FUN_BUILD_CREATE) {
                m_btnMsgLabel->setString(_lang("135002"));
            }else {
                m_btnMsgLabel->setString(_lang("135000"));
                float fw = m_btnMsgLabel->getContentSize().width;
                if (!CCCommonUtils::isIosAndroidPad()) {
                    if (fw>300) {//防爆框
                        m_btnMsgLabel->setScaleX(300.0/fw);
                    }
                } else {
                    if (fw > 425) {
                        m_btnMsgLabel->setScaleX(425.0 / fw);
                    }
                }
            }
        }else if (qInfo.type == TYPE_SCIENCE) {
            m_btnMsgLabel->setString(_lang("135003"));
        }else if (qInfo.type == TYPE_HOSPITAL) {
            m_btnMsgLabel->setString(_lang("135004"));
        }else {
            m_btnMsgLabel->setString(_lang("135001"));
            if (m_info.type == FUN_BUILD_FORT) {
                m_btnMsgLabel->setString(_lang("135002"));
            }
        }
    }
}

void FunBuildView::onEnter()
{
    CCNode::onEnter();
}

void FunBuildView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler FunBuildView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMoreClick", FunBuildView::onMoreClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDestroyClick", FunBuildView::onDestroyClick);
    return NULL;
}

bool FunBuildView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_instantBtn", CCControlButton*, this->m_instantBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_destroyNode", CCNode*, this->m_destroyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moreNode", CCNode*, this->m_moreNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, this->m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtnMsgLabel", CCLabelIF*, this->m_upBtnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLabel", CCLabelIF*, this->m_nextLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLvLabel", CCLabelIF*, this->m_nextLvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextMsgLabel", CCLabelIF*, this->m_nextMsgLabel);
    return false;
}

void FunBuildView::onMoreClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupView(BuildMoreInfoView::create(m_buildId));
}

void FunBuildView::onDestroyClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    if (m_qid != QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        if (qInfo.finishTime <= GlobalData::shared()->getWorldTime()) {
            onResetDesBtn();
        }else {
            string tmpMsg = "";
            if (qInfo.type == TYPE_BUILDING) {
                if (m_info.state == FUN_BUILD_CREATE) {
                    tmpMsg = _lang("135007");
                }else {
                    tmpMsg = _lang("135005");
                }
            }else if (qInfo.type == TYPE_SCIENCE) {
                tmpMsg = _lang("135008");
            }else if (qInfo.type == TYPE_HOSPITAL) {
                tmpMsg = _lang("135009");
            }else {
                tmpMsg = _lang("135006");
                if (m_info.type == FUN_BUILD_FORT) {
                    tmpMsg = _lang("135007");
                }
            }
            YesNoDialog::show( tmpMsg.c_str() , CCCallFunc::create(this, callfunc_selector(FunBuildView::onFirstCancelQueue)));
        }
    }else {
        if (m_info.state == FUN_BUILD_NORMAL && m_info.type == FUN_BUILD_MAIN) {
            PopupViewController::getInstance()->removeAllPopupView();
            FunBuildController::getInstance()->OpenMoveBuild();
        }else {
            YesNoDialog::show( _lang("102182").c_str() , CCCallFunc::create(this, callfunc_selector(FunBuildView::onYesDestroy)));
        }
    }
}

void FunBuildView::onYesDestroy()
{
    if (FunBuildController::getInstance()->startDestroyBuild(m_buildId) )
    {
        PopupViewController::getInstance()->removeAllPopupView();
    }
}

void FunBuildView::onFirstCancelQueue()
{
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    if (qInfo.finishTime <= GlobalData::shared()->getWorldTime()) {
        onResetDesBtn();
    }else {
        YesNoDialog::show( _lang("102195").c_str() , CCCallFunc::create(this, callfunc_selector(FunBuildView::onLastCancelQueue)));
    }
}

void FunBuildView::onLastCancelQueue()
{
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    if (qInfo.finishTime <= GlobalData::shared()->getWorldTime()) {
        onResetDesBtn();
    }else {
        QueueController::getInstance()->startCancelQueue(m_qid);
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void FunBuildView::onResetDesBtn()
{
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    string tmpMsg = "102290";
    if (qInfo.type == TYPE_BUILDING) {
        tmpMsg = "103679";
    }else if (qInfo.type == TYPE_SCIENCE) {
        tmpMsg = "102289";
    }else if (qInfo.type == TYPE_HOSPITAL) {
        tmpMsg = "103677";
    }else if (qInfo.type == TYPE_FORT) {
        tmpMsg = "103675";
    }else if (qInfo.type == TYPE_FORCE || qInfo.type == TYPE_RIDE_SOLDIER || qInfo.type == TYPE_BOW_SOLDIER || qInfo.type == TYPE_CAR_SOLDIER) {
        tmpMsg = "103673";
    }
    CCCommonUtils::flyHint("", "", _lang(tmpMsg));//提示该队列已经完成
    
    m_qid = QID_MAX;
    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    if (m_info.limitNum <= 1) {
        m_destroyNode->setVisible(false);
        m_instantBtn->setEnabled(false);
        m_moreNode->setPositionX(0);
    }
    else {
        if (m_info.state != FUN_BUILD_NORMAL) {
            m_instantBtn->setEnabled(false);
        }
    }
    m_btnMsgLabel->setString(_lang("102121").c_str());
}


///////////
BuildResCell* BuildResCell::create(int type, string msg, int value, string pic, string tmpStr, int infoType)
{
    auto ret = new BuildResCell();
    if (ret && ret->init(type, msg, value, pic, tmpStr, infoType)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BuildResCell::init(int type, string msg, int value, string picStr, string tmpStr, int infoType)
{
    CCBLoadFile("ResCell02",this,this);
    m_addLabel->setString("");
    m_infoType = infoType;
    string picName = CCCommonUtils::getResourceIconByType(type);
    if (picStr != "") {
        picName = picStr;
    }
    
    m_picNode->removeAllChildren();
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    if(pic)
    {
        CCCommonUtils::setSpriteMaxSize(pic, 40, true);
        m_picNode->addChild(pic);
    }
    
    m_nameLabel->setString(msg.c_str());
    m_valueLabel->setString(CC_CMDITOA(value).c_str());
    
    float maxWidth = 200.0;
    if (CCCommonUtils::isIosAndroidPad())
    {
        maxWidth = 690;
    }
    if (tmpStr!="") {
        m_valueLabel->setString(tmpStr);
        maxWidth = 560;
        if (CCCommonUtils::isIosAndroidPad()) {
            maxWidth = 690;
        }
    }
    float fw = m_nameLabel->getContentSize().width;
    if (fw>maxWidth) {//防爆框
        m_nameLabel->setScaleX(maxWidth/fw);
    }
    m_valueLabel->setPositionX(m_valueLabel->getPositionX()+30);
    
    m_helpBtn->setVisible(false);
    m_helpBtn->setEnabled(false);
    if (m_infoType>0) {
        m_helpBtn->setVisible(true);
        m_helpBtn->setEnabled(true);
    }
    
    bool ret = true;
    return ret;
}

void BuildResCell::setOtherInfo(string str)
{
    m_addLabel->setString(str);
    m_addLabel->setPositionX(m_valueLabel->getPositionX()+m_valueLabel->getContentSize().width*m_valueLabel->getOriginScaleX());
}

void BuildResCell::offSetMsgX(int offx)
{
    m_valueLabel->setPositionX(m_valueLabel->getPositionX() + offx);
}

void BuildResCell::onEnter() {
    CCNode::onEnter();
}

void BuildResCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler BuildResCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpBtnClick", BuildResCell::onHelpBtnClick);
    return NULL;
}

bool BuildResCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueLabel", CCLabelIF*, m_valueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addLabel", CCLabelIF*, m_addLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    return false;
}

void BuildResCell::onHelpBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_infoType == 1) {//存储量
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102111")) );
    }
    else if (m_infoType == 2) {//消耗量
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102110")) );
    }
    else if (m_infoType == 3) {//税率
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102166")) );
    }
    else if (m_infoType == 4) {//总伤兵容量
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102165")) );
    }
    else if (m_infoType == 5) {//总训练量
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102181")) );
    }
    else if (m_infoType == 6) {//城防值
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102179")) );
    }
}