//
//  GoldExchangeView.cpp
//  IF
//
//  Created by wangzhenlei on 13-12-13.
//
//

#include "GoldExchangeView.h"
#include "PopupViewController.h"
#include "GoldExchangeItemView.h"
#include "GoldExchangeAtvView.h"
#include "DataRecordCommand.h"
#include "GoldExchangeInfoCommand.h"
#include "GoldExchangeSaleView.h"
#include "GoldFirstPayView.h"
#include "GoldExchangeAdvertisingView.h"

GoldExchangeView::GoldExchangeView(){

}

GoldExchangeView::~GoldExchangeView(){
    
}

GoldExchangeView* GoldExchangeView::create(){
    GoldExchangeView* ret = new GoldExchangeView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeView::init(){
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(102, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(102, false);
        });
        auto bg = CCBLoadFile("GoldExchangeView", this, this,true);
        CCSize winsize = CCDirector::sharedDirector()->getWinSize();
        bg->setContentSize(winsize);
        m_bg->setContentSize(winsize);
        this->setContentSize(winsize);
        
        int exH = getExtendHeight();
        m_listNode->setContentSize(CCSizeMake(m_listNode->getContentSize().width,m_listNode->getContentSize().height+exH));
        

        
        
        //m_BGNode->setPositionY(m_BGNode->getPositionY()+exH);
//        setContentSize(winsize);
        setGoldNum();
        int newBgHeight = exH;
//        int BGcount = (winsize.height)/100+1;
//        for (int i=0; i<BGcount; i++) {
//            auto pic = CCLoadSprite::createSprite("technology_09.png");
//            if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
//            {
//                pic->setScaleX(2.4);
//            }
//            m_BGNode->addChild(pic);
//            pic->setPositionY(-i*100);
//        }
        if(isGetData()){
            getData();
        }else{
            refresh(NULL);
        }
        
        return true;
    }
    return false;
}
void GoldExchangeView::refresh(CCObject* obj){
    if (obj == NULL) {
        initList();
        dataRecord();
    }else{
        CCNode* node = m_advertisNode->getChildByTag(1000);
        if(node){
            CCSize listSize = m_listNode->getContentSize();
            float addH = node->getContentSize().height;
            listSize.height += addH;
            m_listNode->setContentSize(listSize);
            m_listNode->setPositionY(m_listNode->getPositionY()+addH);
            m_scrollView->setViewSize(listSize);
            m_scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height - m_scrollView->getContentSize().height));
            node->removeFromParent();
        }
    }
}
bool GoldExchangeView::isGetData(){
    time_t t;
    t =GlobalData::shared()->getWorldTime();
    if(CCCommonUtils::timeStampToMD(GlobalData::shared()->goldExchageRefreshStamp)==CCCommonUtils::timeStampToMD(t)){
        return false;
    }
    return true;
}
void GoldExchangeView::getData(){

    GoldExchangeInfoCommand* cmd = new GoldExchangeInfoCommand();
    cmd->sendAndRelease();
}
void GoldExchangeView::dataRecord(){
    CCArray* p = CCArray::create();
    p->addObject(CCInteger::create(GOLD_PANEL_SUCCESS));
    DataRecordCommand *cmd = new DataRecordCommand(OPEN_PANEL, p, CCArray::create());
    cmd->sendAndRelease();
}
bool GoldExchangeView::sortExchangeItem(GoldExchangeItem *item1, GoldExchangeItem *item2){
    float a = atof(item1->dollar.c_str());
    float b = atof(item2->dollar.c_str());
    return (a<b);
    
}
void GoldExchangeView::initList(){
    m_scrollView = CCScrollView::create(m_listNode->getContentSize());
    m_scrollView->setTouchMode(kCCTouchesOneByOne);
    m_scrollView->setTouchPriority(Touch_Default);
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);

    float offsetY = 0;
    //float x = m_listNode->getContentSize().width/2;

//    map<string, GoldExchangeItem*> sortMap;
    vector<GoldExchangeItem*> sortMap;
    map<string, GoldExchangeItem*> sortMap1;
    map<string, GoldExchangeItem*> sortMap2;
    

    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        if(!it->second)
            continue;
        if (it->second->type == "1") {
            int iID = ( 5 - atoi(it->second->type.c_str())) * 100000 + atoi(it->second->id.c_str());
            string id = CC_ITOA(iID);
            sortMap.push_back(it->second);
//            sortMap[id] = it->second;
        }else if(it->second->type == "2"){
            int iID = it->second->popup*1000000+atoi(it->second->id.c_str());
            sortMap1[CC_ITOA(iID)] = it->second;
        }else if(it->second->type == "3"){
            int iID = it->second->popup*1000000+atoi(it->second->id.c_str());
            sortMap2[CC_ITOA(iID)] = it->second;
        }
    }
    std::sort(sortMap.begin(),sortMap.end(),sortExchangeItem);
    
    int i=0;
    int dy=-155;
    float tmp = 10;
    if (CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
    {
//        dy=-230;
        dy = -170;
    }
    CCNode* listNode = CCNode::create();
    m_scrollView->addChild(listNode);
//    map<string, GoldExchangeItem*>::iterator it = sortMap.begin();
    auto it = sortMap.begin();
    GoldExchangeItemView* cell=NULL;
    do {
        if (true) {
            if((*it)->type=="1" && (*it)->popup_image != "hide"){
                cell =  GoldExchangeItemView::create((*it));
                cell->setPositionY(dy);
//                if(i%2==0){
//                    if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
//                        cell->setPositionX(6.66*2.4);
//                    else
//                        cell->setPositionX(6.66);
//                }else{
//                    if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
//                        cell->setPositionX(6.66*2+310*2.4);
//                    else
//                        cell->setPositionX(6.66*2+310);
////                    dy -= cell->getContentSize().height;
//                    if(CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
//                        dy -= cell->getContentSize().height;
//                    else
//                        dy -= cell->getContentSize().height;
//                }
                
                if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
                    cell->setPositionX(tmp*2.4);
                else
                    cell->setPositionX(tmp);
                
                if(CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
                    dy -= cell->getContentSize().height;
                else
                    dy -= cell->getContentSize().height;
                
                listNode->addChild(cell);
                
                i++;
            }
        }
        it++;
    }while (it != sortMap.end());
    
    
    
    
    
    
    
//    it = sortMap.begin();
//    cell=NULL;
//    do {
//        if (true) {
//            if((*it)->type=="1" && (*it)->popup_image != "hide"){
//                cell =  GoldExchangeItemView::create((*it));
//                cell->setPositionY(dy);
//                //                if(i%2==0){
//                //                    if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
//                //                        cell->setPositionX(6.66*2.4);
//                //                    else
//                //                        cell->setPositionX(6.66);
//                //                }else{
//                //                    if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
//                //                        cell->setPositionX(6.66*2+310*2.4);
//                //                    else
//                //                        cell->setPositionX(6.66*2+310);
//                ////                    dy -= cell->getContentSize().height;
//                //                    if(CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
//                //                        dy -= cell->getContentSize().height;
//                //                    else
//                //                        dy -= cell->getContentSize().height;
//                //                }
//                
//                if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
//                    cell->setPositionX(tmp*2.4);
//                else
//                    cell->setPositionX(tmp);
//                
//                if(CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
//                    dy -= cell->getContentSize().height;
//                else
//                    dy -= cell->getContentSize().height;
//                
//                listNode->addChild(cell);
//                
//                i++;
//            }
//        }
//        it++;
//    }while (it != sortMap.end());
    
    
    
    if(GlobalData::shared()->analyticID == "mycard" || GlobalData::shared()->analyticID == "gash"){
        cell =  GoldExchangeItemView::create(NULL);
        cell->setPositionY(dy);
//        if(i%2==0){
//            cell->setPositionX(6.66);
//        }else{
//            
//            if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
//            {
//                cell->setPositionX(6.66*2+310*2.4);
//                dy -= cell->getContentSize().height;
//            }
//            else
//            {
//                cell->setPositionX(6.66*2+310);
//                dy -= cell->getContentSize().height;
//            }
//            
//            
//        }
        if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
            cell->setPositionX(tmp*2.4);
        else
            cell->setPositionX(tmp);
        
        if(CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
            dy -= cell->getContentSize().height;
        else
            dy -= cell->getContentSize().height;
        

        
    
        listNode->addChild(cell);
        i++;
    }
    
//    if(i%2==1){//fusheng 原本是针对第五个商品的
//        if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
//            cell->setPositionX(6.66*2+310*2.4);
//        else
//            cell->setPositionX(6.66*2+310);
//    }

//    listNode->setPositionY(m_listNode->getContentSize().height);

    
//    listNode->setContentSize(Size(620, -dy));
    offsetY+=(-dy);
    
    offsetY-=155;
    
    listNode->setPositionY(offsetY); //fusheng d 11.23
    i=0;
//    map<string, GoldExchangeItem*>::iterator it1 = sortMap1.end();
//    do {
//        if (sortMap1.size()>0) {
//            it1--;
//            if(it1->second->type=="2"){
//                int endTime = it1->second->end;
//                if(endTime>GlobalData::shared()->getWorldTime()){
//                    GoldExchangeAtvView* cell =  GoldExchangeAtvView::create(it1->second, 0);
//                    cell->setPositionX(0);
//                    cell->setPositionY(offsetY);
//                    offsetY += cell->getContentSize().height;
//                    m_scrollView->addChild(cell);
//                
//                }
//
//            }
//        }
//        
//    }while (it1 != sortMap1.begin());
//    
//    map<string, GoldExchangeItem*>::iterator it2;
//
//    for(it2 = sortMap2.begin(); it2 != sortMap2.end(); it2++){
//        
//        if(it2->second->type=="3"){
//            int endTime = it2->second->end;
//            if(endTime>GlobalData::shared()->getWorldTime()&&(!it2->second->bought)){
//                if(it2->second->popup_image=="new_recharge"||it2->second->popup_image=="close"){
//                    continue;
////                    GoldExchangeFirstPayCell* cell =  GoldExchangeFirstPayCell::create(it2->second, 0);
////                    cell->setPositionX(0);
////                    cell->setPositionY(offsetY);
////                    offsetY += cell->getContentSize().height;
////                    m_scrollView->addChild(cell);
//                }else{
//                    GoldExchangeSaleView* cell =  GoldExchangeSaleView::create(it2->second, 0);
//                    cell->setPositionX(0);
//                    cell->setPositionY(offsetY);
//                    offsetY += cell->getContentSize().height;
//                    m_scrollView->addChild(cell);
//                }
//                
//            }
//            
//        }
//    }
    //显示首充奖励
//    if(GlobalData::shared()->playerInfo.payTotal==0){
//        GoldFirstPayView* firstcell =  GoldFirstPayView::create(1);
//        firstcell->setPositionX(0);
//        firstcell->setPositionY(offsetY);
//        offsetY += firstcell->getContentSize().height;
//        m_scrollView->addChild(firstcell);
//    }
    //显示 advertising
    GoldExchangeAdvertisingView* advertsingcell =  GoldExchangeAdvertisingView::create();
//    advertsingcell->setPositionX(-320);
//    advertsingcell->setPositionY(offsetY);
//    offsetY += 410;

    m_advertisNode->addChild(advertsingcell);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_advertisNode->setScale(2.15f);
    }
    
//    if(monthCardMap.size()>0){
//        //目前只有1个月卡,显示暂时只处理一个
//        auto iter = monthCardMap.begin();
//        MonthCardBuyView* monthCardCell = MonthCardBuyView::create((*iter).second,true);
//        m_advertisNode->addChild(monthCardCell);
//        monthCardCell->setPositionX(12);
//        CCPoint pos = advertsingcell->getPosition();
//        CCSize s = advertsingcell->getContentSize();
//        CCSize size = monthCardCell->getContentSize();
//        monthCardCell->setPosition(-size.width*0.5,pos.y-size.height);
//        CCSize listSize = m_listNode->getContentSize();
//        listSize.height -= size.height;
//        m_listNode->setContentSize(listSize);
//        m_listNode->setPositionY(m_listNode->getPositionY()-size.height);
//        m_scrollView->setViewSize(listSize);
//        monthCardCell->setTag(1000);
//    }
    m_scrollView->setContentSize(CCSize(m_listNode->getContentSize().width,offsetY));
    m_scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height - offsetY));
    

//    
//    m_scrollView->setContentSize(CCSize(m_listNode->getContentSize().width,m_listNode->getContentSize().height));
//    m_scrollView->setContentSize(CCSize(m_listNode->getContentSize().width,offsetY));
//    ;
//    m_scrollView->setPositionY(m_listNode->getContentSize().height - offsetY);
//    
//    float newH =offsetY;
//    
//    m_scrollView->setViewSize(Size(m_scrollView->getViewSize().width,newH));

//    m_scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height));
    m_listNode->addChild(m_scrollView);
    
    auto svp = m_scrollView->convertToNodeSpaceAR(Vec2::ZERO);
    
    auto mlnp = m_listNode->convertToNodeSpaceAR(Vec2::ZERO);

}

void GoldExchangeView::onEnter(){
    PopupBaseView::onEnter();
    m_title->setString(_lang("101221"));
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeView::refresh), GOLDEXCHANGE_SHOW_COMMAND, NULL);
}

void GoldExchangeView::onExit(){
    PopupBaseView::onExit();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_SHOW_COMMAND);
}

bool GoldExchangeView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIF*, this->m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_advertisNode", CCNode*, this->m_advertisNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldText", CCLabelIF*, this->m_goldText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    return false;
}

SEL_CCControlHandler GoldExchangeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", GoldExchangeView::onCloseClick);
    return NULL;
}

void GoldExchangeView::setGoldNum(){
    auto &playerInfo = GlobalData::shared()->playerInfo;
    this->m_goldText->setString(CC_ITOA(playerInfo.gold));
    this->m_goldText->setVisible(false);
}

void GoldExchangeView::onCloseClick(cocos2d::CCObject *pSender, CCControlEvent event){
    closeSelf();
}