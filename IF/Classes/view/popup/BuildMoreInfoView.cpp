//
//  BuildMoreInfoView.cpp
//  IF
//
//  Created by fubin on 14-2-21.
//
//

#include "BuildMoreInfoView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "WatchtowerView.h"
#include "MainCityView.h"
#include "BuildingHospitalPopUpView.h"
#include "ScienceListView.h"

BuildMoreInfoView* BuildMoreInfoView::create(int buildId, bool isUp, int pos){
    BuildMoreInfoView* ret = new BuildMoreInfoView();
    if(ret && ret->init(buildId, isUp, pos)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BuildMoreInfoView::init(int buildId, bool isUp, int pos)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_buildId = buildId;
    m_pos = pos;
    m_isUp = isUp;
    auto tmpCCB = CCBLoadFile("BuildMoreInfoView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    int oldBgHeight = m_buildBG->getContentSize().height;
    int tmpExth = getExtendHeight();
    int newBgHeight = oldBgHeight + tmpExth;
    CCSize temp = m_buildBG->getContentSize();
    temp.height = newBgHeight;
    m_buildBG->setContentSize(temp);
    
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-tmpExth);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+tmpExth));
    
    m_mainNode->setPositionY(m_mainNode->getPositionY()+tmpExth/2);
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    m_itemBgNode = CCNode::create();
    m_itemNode = CCNode::create();
    
//    updateInfo();
    
    return true;
}

void BuildMoreInfoView::updateInfo()
{
    int curY = 0;
    string des = "";
    string name = "";
    m_lv = -1;
    if ( m_pos< 0) {
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        m_lv = m_info.level;
        name = _lang(m_info.name);
        string title = _lang(m_info.name)+" "+_lang_1("102272", CC_ITOA(m_info.level));
        if(m_info.level>=GlobalData::shared()->MaxBuildLv) {
            title += _lang("102183");
        }
        m_titleText->setString(title);
        des = m_info.description;
        
        if(m_info.type == FUN_BUILD_SCIENE&&false) {
            auto label = CCLabelIF::create(_lang("102078").c_str(), 24, CCSizeMake(480, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY = m_infoList->getContentSize().height/2-100;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY = m_infoList->getContentSize().height-200;
        }
        else {
            curY = analyzeInfo(m_info.information);
        }
    }
    else {
        name = CCCommonUtils::getNameById(CC_ITOA(m_buildId));
        m_titleText->setString(name);
        string information = CCCommonUtils::getPropById(CC_ITOA(m_buildId), "information");
        
        if(m_buildId == FUN_BUILD_SCIENE) {
            auto label = CCLabelIF::create(_lang("102078").c_str(), 24, CCSizeMake(480, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setColor(ccWHITE);
            curY = m_infoList->getContentSize().height/2-100;
            int labelW = label->getContentSize().width*label->getOriginScaleX();
            label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
            m_scrollView->addChild(label);
            curY = m_infoList->getContentSize().height-200;
        }
        else {
            curY = analyzeInfo(information);
        }
        
        auto& info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        des = info.description;
    }
    
    if (1) {
        auto linebg = CCLoadSprite::createScale9Sprite("brown_bar_bg.png");
        linebg->setContentSize(CCSize(m_infoList->getContentSize().width, 30));
        linebg->setAnchorPoint(ccp(0, 0));
        linebg->setPosition(ccp(0, curY));
        linebg->setOpacity(150);
        m_scrollView->addChild(linebg);
        
        string nameDetails = name + " " + _lang("102191");
        auto nameLabel = CCLabelIF::create(nameDetails.c_str(), 26, CCSizeMake(500, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        nameLabel->setAnchorPoint(ccp(0.5,0));
        nameLabel->setColor(ccc3(56, 8, 0));
        int nameW = nameLabel->getContentSize().width*nameLabel->getOriginScaleX();
        int tmpX = (m_infoList->getContentSize().width-nameW)/2;
        nameLabel->setPosition(ccp(tmpX, curY));
        m_scrollView->addChild(nameLabel);
        curY += 30;
    }
    
    if (des!="") {
        string tmpDes = "    "+_lang(des.c_str());
        auto desLabel = CCLabelIF::create(tmpDes.c_str(), 20, CCSizeMake(500, 0), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        desLabel->setAnchorPoint(ccp(0.5,0));
        desLabel->setColor(ccc3(56, 8, 0));
        int desHeight = desLabel->getContentSize().height*desLabel->getOriginScaleY();
        int desWidth = desLabel->getContentSize().width*desLabel->getOriginScaleX();
        curY += 20;
        int tmpX = (m_infoList->getContentSize().width-desWidth)/2;
        desLabel->setPosition(ccp(tmpX, curY));
        m_scrollView->addChild(desLabel);
        
        curY +=desHeight;
    }
    
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
}

void BuildMoreInfoView::onEnter()
{
    CCNode::onEnter();
    updateInfo();
}

void BuildMoreInfoView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler BuildMoreInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", BuildMoreInfoView::onCloseClick);
    return NULL;
}

bool BuildMoreInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    return false;
}

void BuildMoreInfoView::onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void BuildMoreInfoView::doWhenClose()
{
}

int BuildMoreInfoView::analyzeInfo(string moreInfomation)
{
//    string resInfo = "102203;102204|102200,1;102205,5|102200,2;102205,7|102200,3;102205,9|102200,4;102205,11|102200,5;102205,13|102200,6;102205,15|102200,7;102205,17|102200,8;102205,19|102200,9;102205,21|102200,10;102205,23|102200,11;102205,25|102200,12;102205,27|102200,13;102205,29|102200,14;102205,31|102200,15;102205,33|102200,16;102205,35|102200,17;102205,37|102200,18;102205,39|102200,19;102205,41|102200,20;102205,43|102200,21;102205,45|102200,22;102205,47|102200,23;102205,49|102200,24;102205,51|102200,25;102205,53|102200,26;102205,55|102200,27;102205,57|102200,28;102205,59|102200,29;102205,61|102200,30;102205,63|102200,31;102205,65|102200,32;102205,67|102200,33;102205,69|102200,34;102205,71|102200,35;102205,73|102200,36;102205,75|102200,37;102205,77|102200,38;102205,79|102200,39;102205,81|102200,40;102205,83|102200,41;102205,85|102200,42;102205,87|102200,43;102205,89|102200,44;102205,95|102200,45;102205,100|102200,46;102205,105|102200,47;102205,110|102200,48;102205,115|102200,49;102205,120|102200,50;102205,125";
    
    string resInfo = moreInfomation;
    
    int maxWidth = m_infoList->getContentSize().width;
    int fristColumn = 100;
    int preColumn = 0;
    int curY = 0;
    int allHeight = 0;
    int cntColumn = 0;
    
    int linePerCell = 5;
    
    int targetNum = 0;
    int startNum = 0;
    std::vector<std::string> lineItems;
    CCCommonUtils::splitString(resInfo, "|", lineItems);
    for (int i=0; i<lineItems.size(); i++)
    {
        string lineInfo = lineItems[i];
        std::vector<std::string> cellItems;
        CCCommonUtils::splitString(lineInfo, ";", cellItems);
        if (cellItems.size() == 1) {
            fristColumn = maxWidth;
        }
        else {
            preColumn = (maxWidth-fristColumn)/(cellItems.size()-1);
        }
        cntColumn = cellItems.size();
        
        startNum = targetNum;
        int curLineMaxH = 0;
        for (int j=0; j<cellItems.size(); j++)
        {
            string cellSumInfo = cellItems[j];
            std::vector<std::string> tinySumItems;
            CCCommonUtils::splitString(cellSumInfo, "*", tinySumItems);
            int curHeight = 0;
            for (int n=0; n<tinySumItems.size(); n++)
            {
                string cellInfo = tinySumItems[n];
                std::vector<std::string> tinyItems;
                CCCommonUtils::splitString(cellInfo, ",", tinyItems);
                string cellMsg = "";
                if (tinyItems.size()==1)
                {
                    cellMsg = _lang(tinyItems[0]);
                }
                else if(tinyItems.size()==2)
                {
                    cellMsg = _lang_1(tinyItems[0],CC_CMDITOA(atoi(tinyItems[1].c_str())).c_str()  );
                }
                else if(tinyItems.size()==3)
                {
                    cellMsg = _lang_2(tinyItems[0],CC_CMDITOA(atoi(tinyItems[1].c_str())).c_str() ,CC_CMDITOA(atoi(tinyItems[2].c_str())).c_str());
                }
//                CCLOG("cellMsg=%s      n=%d",cellMsg.c_str(),n);
                
                if (j==0) {
                    auto cellLabel = CCLabelIF::create(cellMsg.c_str(), 22, CCSizeMake(fristColumn, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
                    cellLabel->setAnchorPoint(ccp(0.5,1));
                    cellLabel->setColor(ccc3(56, 8, 0));
                    cellLabel->setPosition(ccp(fristColumn/2, curY-curHeight));
                    int tmpHeight = cellLabel->getContentSize().height*cellLabel->getOriginScaleY();
                    curHeight += tmpHeight+linePerCell;
                    cellLabel->setTag(targetNum);
                    targetNum++;
                    m_itemNode->addChild(cellLabel);
                    if (curLineMaxH<curHeight) {
                        curLineMaxH = curHeight;
                    }
                }
                else {
                    CCLabelIF* cellLabel = NULL;
                    if(m_buildId/1000==FUN_BUILD_WATCH_TOWER && j==2){
                        cellLabel = CCLabelIF::create(cellMsg.c_str(), 22, CCSizeMake(330, 0), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
                        cellLabel->setPosition(ccp(225, curY-curHeight));
                    }else{
                        cellLabel = CCLabelIF::create(cellMsg.c_str(), 22, CCSizeMake(preColumn-20, 0), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
                        cellLabel->setPosition(ccp(fristColumn+(j-1)*preColumn+5, curY-curHeight));
                    }
                    cellLabel->setAnchorPoint(ccp(0,1));
                    cellLabel->setColor(ccc3(56, 8, 0));
                    int tmpHeight = cellLabel->getContentSize().height*cellLabel->getOriginScaleY();
                    curHeight += tmpHeight+linePerCell;
                    cellLabel->setTag(targetNum);
                    targetNum++;
                    m_itemNode->addChild(cellLabel);
//                    if (curLineMaxH<curHeight) {
//                        curLineMaxH = curHeight;
//                    }
                    if (i==0) {
                        int cellWidth=cellLabel->getContentSize().width;
                        int int2=preColumn;
                        CCLOG("int1=%d",cellWidth);
                        CCLOG("int2=%d",int2);
                              
//                        if(cellWidth>preColumn){
//                            float dw=(cellWidth-preColumn)/11.0;
//                            float fs=cellLabel->getFontSize()-dw>18?cellLabel->getFontSize()-dw:18;
//                            CCLOG("fs=%f,dw=%f,Size:%f",fs,dw,cellLabel->getFontSize());
//                            cellLabel->setFontSize(fs);
//                            if (cellLabel->getFontSize()-dw<14.5) {
////                                float scaleX=1-(11-(cellLabel->getFontSize()-dw))*0.1;
//                                cellLabel->setScaleX(0.5);
//                            }
//                            curHeight=(tmpHeight*fs/22)+linePerCell;
//                        }
                        int newpc=preColumn+28;
                        if(cellWidth>newpc){
                            float scalex=(newpc*1.0)/(cellWidth*1.0);
                            cellLabel->setScaleX(scalex>0.5?scalex:0.5);
                        }else if(cellWidth>preColumn){
                            float dw=(cellWidth-preColumn)/11;
                            float fs=cellLabel->getFontSize()-dw>15?cellLabel->getFontSize()-dw:15;
                            CCLOG("fs=%f,dw=%f,Size:%f",fs,dw,cellLabel->getFontSize());
                            cellLabel->setFontSize(fs);
                            curHeight=(tmpHeight*fs/22)+linePerCell;
                        }

                    }
                    if (curLineMaxH<curHeight) {
                        curLineMaxH = curHeight;
                    }
                }
            }
        }
        
        curLineMaxH = curLineMaxH>0?(curLineMaxH-linePerCell):0;
        
        if (m_lv>=1 && i==m_lv) {
            auto linebg = CCLoadSprite::createScale9Sprite("orange_bar_bg.png");
            linebg->setContentSize(CCSize(maxWidth, curLineMaxH+10));
            linebg->setAnchorPoint(ccp(0, 1));
            linebg->setPosition(ccp(0, curY));
            linebg->setOpacity(150);
            m_itemBgNode->addChild(linebg);
        }
        else {
            if(i%2 == 1)
            {
                auto linebg = CCLoadSprite::createScale9Sprite("hospital_render_bg.png");
                linebg->setContentSize(CCSize(maxWidth, curLineMaxH+10));
                linebg->setAnchorPoint(ccp(0, 1));
                linebg->setPosition(ccp(0, curY));
                linebg->setOpacity(150);
                linebg->setInsetBottom(2);
                linebg->setInsetTop(2);
                linebg->setInsetLeft(2);
                linebg->setInsetRight(2);
                m_itemBgNode->addChild(linebg);
            }
        }
        
//        curY -= (curLineMaxH/2+5);
        for (int m=startNum; m<targetNum; m++)
        {
            auto node = m_itemNode->getChildByTag(m);
            node->setPositionY(node->getPositionY()-5);
        }
        allHeight += (curLineMaxH+10);
        curY = -allHeight;
    }
    
    for (int i=1; i<cntColumn; i++)
    {
        int curX = 0;
        if (i == 0) {
            curX = 0;
        }
        else if (i == 1) {
            curX = fristColumn;
        }
        else if (i == cntColumn) {
            curX = maxWidth;
        }
        else {
            curX = fristColumn+(i-1)*preColumn;
        }
        auto line = CCLoadSprite::createScale9Sprite("xian.png");
        if(m_buildId/1000==FUN_BUILD_WATCH_TOWER){
            if(i==2){
                line->setPosition(ccp(200, 0));
            }else{
               line->setPosition(ccp(curX-15, 0));
            }
        }else{
            line->setPosition(ccp(curX, 0));
        }
        line->setColor(ccBLACK);
        line->setContentSize(CCSize(allHeight, 2));
        line->setAnchorPoint(ccp(0, 0));
        line->setRotation(90);
        m_itemBgNode->addChild(line);
    }
    
    int px = (m_infoList->getContentSize().width-maxWidth)/2;
    curY = -curY;
    m_itemNode->setPosition(ccp(px, curY));
    m_itemBgNode->setPosition(ccp(px, curY));
    m_scrollView->addChild(m_itemBgNode);
    m_scrollView->addChild(m_itemNode);
    return curY;
}
