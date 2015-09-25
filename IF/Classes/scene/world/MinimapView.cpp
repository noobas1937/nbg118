//
//  MinimapView.cpp
//  IF
//
//  Created by 邹 程 on 14-5-5.
//
//

#include "MinimapView.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#include "GetAllianceMemberPointCommand.h"
#include "ServerListPopUpView.h"
#include "WorldController.h"
#include "ParticleController.h"
#include "SoundController.h"
#include "SceneController.h"
#include "YesNoDialog.h"
#include "DynamicResourceController.h"
#include "FunBuildController.h"

#define Legend_Node_Tag 10000
#define Title_Node_Tag 9999
#define Map_Node_Tag 10001//常驻7块 移动时添加3块 最多10块
#define Cover_Node_Tag 10011
#define Server_Banner_Tag 10012
void MinimapView::addToMiniMap(CCNode *node, int layerIndex){
    
}
MinimapView::MinimapView(int serverId):m_currentServerId(serverId){
};

MinimapView::~MinimapView(){
};

MinimapView *MinimapView::create(int serverId){
    MinimapView *ret = new MinimapView(serverId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MinimapView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(208, false);
            DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_MINIMAPVIEW_TEXTURE,true);
        });
        DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_MINIMAPVIEW_TEXTURE,false);
        CCLoadSprite::doResourceByCommonIndex(208, true);
        setIsHDPanel(true);
        isAllShow = true;
        m_bCanClickServerBtn = true;
        m_InAction = false;
        singleTouchClose = false;
        this->_modelLayer->setTouchEnabled(false);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        m_container = CCNode::create();

        m_cityBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("position_city.png")->getTexture(), 448);
        m_mapBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("minimap_1.png")->getTexture(), 448);
        m_edgeBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("edge_light_0.png")->getTexture(), 448);
        m_resBeltBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("resource_belt.png")->getTexture(), 448);
        m_resourceIntroPointer = CCNode::create();
        m_resourceIntroText = CCNode::create();
        m_nameContainer = CCNode::create();
        m_arrowBatchNode = CCNode::create();
        
        m_markNode = CCNode::create();
        m_markTouchNode = CCNode::create();
        
        m_container->addChild(m_cityBatchNode, 7);
        m_container->addChild(m_mapBatchNode,1);
        m_container->addChild(m_edgeBatchNode,2);
        m_container->addChild(m_arrowBatchNode,3);
        m_container->addChild(m_resourceIntroPointer, 5);
        m_container->addChild(m_resourceIntroText, 6);
        m_container->addChild(m_resBeltBatchNode,4);
        
        m_container->addChild(m_markNode,40);
        
        m_mapInfo = MinimapInfo::create();
        m_mapInfo->selfPt = WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint);
        
        m_viewPort = HFViewport::create();
        m_viewPort->setViewPortTarget(m_container);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_mapW = 1920;
            m_mapH = 1400;
        }
        else {
            m_mapW = 1920*MINIMAP_SCALE;
            m_mapH = 1160*MINIMAP_SCALE;
        }
        viewCenterPoint = ccp(m_mapW / 2, m_mapH / 2);

        m_viewPort->setSceneSize(m_mapW,m_mapH);////
        m_viewPort->allowZoom = false;
        m_viewPort->setZoomLimitationMin(1.0);
        m_viewPort->setZoomLimitationMax(1.0);
        m_viewPort->setWorldBound(CCRect(0,0,m_mapW,m_mapH));
        m_viewPort->setTouchDelegate(this);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_container->setScale(MINIMAP_HD_SCALE);
//            m_viewPort->setCurZoomScale(1.0 * MINIMAP_HD_SCALE);
            m_viewPort->setZoomLimitationMin(1.0 * MINIMAP_HD_SCALE);
            m_viewPort->setZoomLimitationMax(1.0 * MINIMAP_HD_SCALE);
            m_nameContainer->setScale(MINIMAP_HD_SCALE);
        }
        this->addChild(m_container);
        this->addChild(m_viewPort);
        
        auto size = CCDirector::sharedDirector()->getWinSize();
        auto pos = ccp(size.width / 2, size.height - 40);
        m_nameContainer->setPosition(pos);
        m_nameContainer->setTag(Title_Node_Tag);
        this->addChild(m_nameContainer);
        
        //返回按钮
        m_returnBtn = CCLoadSprite::createSprite("bnt_02.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            m_returnBtn->setScale(2);
        }
        this->addChild(m_returnBtn);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_returnBtn->setPosition(ccp(m_returnBtn->getContentSize().width, 120));
        }
        else
            m_returnBtn->setPosition(ccp(m_returnBtn->getContentSize().width / 2, 60));
        
        auto btnSprite2 = CCLoadSprite::createSprite("fanhui.png");
        this->addChild(btnSprite2);
        if (CCCommonUtils::isIosAndroidPad()) {
            btnSprite2->setScale(2);
        }
        btnSprite2->setPosition(m_returnBtn->getPosition());
        
        //服务器列表按钮
        m_serverListBtn = CCLoadSprite::createSprite("bnt_02.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            m_serverListBtn->setScale(2);
        }
        this->addChild(m_serverListBtn);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_serverListBtn->setPosition(m_returnBtn->getPosition() + ccp(0, 240));
        }
        else
            m_serverListBtn->setPosition(m_returnBtn->getPosition() + ccp(0, 120));
        
        auto btnSprite1 = CCLoadSprite::createSprite("diqiu.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            btnSprite1->setScale(2);
        }
        this->addChild(btnSprite1);
        btnSprite1->setPosition(m_serverListBtn->getPosition());
        
//        //返回自己所在服务器按钮
//        m_goBackMyServerBtn = CCLoadSprite::createSprite("bnt_02.png");
//        this->addChild(m_goBackMyServerBtn);
//        m_goBackMyServerBtn->setPosition(m_returnBtn->getPosition() + ccp(0, 240));
//        
//        auto btnSprite3 = CCLoadSprite::createSprite("fanhui.png");
//        this->addChild(btnSprite3);
//        btnSprite3->setPosition(m_goBackMyServerBtn->getPosition());
        
        //去其他服的按钮
        m_goToServerBtn1 = CCLoadSprite::createSprite("minimap_arrow1.png");
        m_goToServerBtn1->setPositionX(viewCenterPoint.x + 422 + 30);
        m_goToServerBtn1->setPositionY(viewCenterPoint.y + 279 + 30);
        m_arrowBatchNode->addChild(m_goToServerBtn1);
        
        m_rServerBg = CCLoadSprite::createSprite("minimap_serBg.png");
        m_rServerBg->setOpacity(115);
        m_rServerBg->setPositionX(m_goToServerBtn1->getPositionX());
        m_rServerBg->setPositionY(m_goToServerBtn1->getPositionY() + m_goToServerBtn1->getContentSize().height / 2 + 10);
        m_arrowBatchNode->addChild(m_rServerBg);
        
        m_goToServerBtn2 = CCLoadSprite::createSprite("minimap_arrow2.png");
        m_goToServerBtn2->setPositionX(viewCenterPoint.x + 422 + 30);
        m_goToServerBtn2->setPositionY(viewCenterPoint.y - 279 - 30);
        m_arrowBatchNode->addChild(m_goToServerBtn2);
        
        m_dServerBg = CCLoadSprite::createSprite("minimap_serBg.png");
        m_dServerBg->setOpacity(115);
        m_dServerBg->setPositionX(m_goToServerBtn2->getPositionX());
        m_dServerBg->setPositionY(m_goToServerBtn2->getPositionY() - m_goToServerBtn2->getContentSize().height / 2);
        m_arrowBatchNode->addChild(m_dServerBg);
        
        m_goToServerBtn3 = CCLoadSprite::createSprite("minimap_arrow1.png");
        m_goToServerBtn3->setFlipX(true);
        m_goToServerBtn3->setPositionX(viewCenterPoint.x - 422 - 30);
        m_goToServerBtn3->setPositionY(viewCenterPoint.y + 279 + 30);
        m_arrowBatchNode->addChild(m_goToServerBtn3);
        
        m_uServerBg = CCLoadSprite::createSprite("minimap_serBg.png");
        m_uServerBg->setOpacity(115);
        m_uServerBg->setPositionX(m_goToServerBtn3->getPositionX());
        m_uServerBg->setPositionY(m_goToServerBtn3->getPositionY() + m_goToServerBtn3->getContentSize().height / 2 + 10);
        m_arrowBatchNode->addChild(m_uServerBg);
        
        m_goToServerBtn4 = CCLoadSprite::createSprite("minimap_arrow2.png");
        m_goToServerBtn4->setFlipX(true);
        m_goToServerBtn4->setPositionX(viewCenterPoint.x - 422 - 30);
        m_goToServerBtn4->setPositionY(viewCenterPoint.y - 279 - 30);
        m_arrowBatchNode->addChild(m_goToServerBtn4);
        
        m_lServerBg = CCLoadSprite::createSprite("minimap_serBg.png");
        m_lServerBg->setOpacity(115);
        m_lServerBg->setPositionX(m_goToServerBtn4->getPositionX());
        m_lServerBg->setPositionY(m_goToServerBtn4->getPositionY() - m_goToServerBtn4->getContentSize().height / 2);
        m_arrowBatchNode->addChild(m_lServerBg);
        
        //其他服务器title
        m_lSerTitle = CCLabelIF::create();
        m_uSerTitle = CCLabelIF::create();
        m_rSerTitle = CCLabelIF::create();
        m_dSerTitle = CCLabelIF::create();
        m_arrowBatchNode->addChild(m_lSerTitle);
        m_arrowBatchNode->addChild(m_uSerTitle);
        m_arrowBatchNode->addChild(m_rSerTitle);
        m_arrowBatchNode->addChild(m_dSerTitle);
        //设置按钮：资源带和联盟玩家位置
        m_settingNode = CCNode::create();
        m_settingBtn = CCLoadSprite::createSprite("d-biyan.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            m_settingBtn->setScale(2);
        }
        this->addChild(m_settingNode);
        this->addChild(m_settingBtn);
        
        CCSize setBtnSize = m_settingBtn->getContentSize();
        CCPoint orgPos = CCDirector::sharedDirector()->getVisibleOrigin();
        m_settingNode->setPosition(ccp(winSize.width - orgPos.x - setBtnSize.width, m_returnBtn->getPositionY()+ 5));
         if (CCCommonUtils::isIosAndroidPad()) {
             m_settingNode->setPosition(ccp(winSize.width - orgPos.x - setBtnSize.width-setBtnSize.width/3, m_returnBtn->getPositionY()+ 5));
         }
        
        m_settingBtn->setPosition(ccp(m_settingNode->getPositionX() + setBtnSize.width * 0.5, m_returnBtn->getPositionY() + 5));
        CCScale9Sprite *bgScale = CCScale9Sprite::createWithSpriteFrameName ("shu-hei.png");
        bgScale->setAnchorPoint(ccp(0.5, 0));
        bool isInAlliance = GlobalData::shared()->playerInfo.isInAlliance();
        int bgH = 323;
        if (CCCommonUtils::isIosAndroidPad()) {
            bgScale->setPreferredSize(CCSize(setBtnSize.width*0.8, bgH * 2));
        }
        else
            bgScale->setPreferredSize(CCSize(setBtnSize.width*0.8, bgH));
        m_settingNode->addChild(bgScale);
        if (CCCommonUtils::isIosAndroidPad()) {
            bgScale->setScaleY(isInAlliance?1.6:1);
        }
        else
            bgScale->setScaleY(isInAlliance?0.8:0.5);
        bgScale->setPosition(ccp(setBtnSize.width*0.5, 0));
        bgScale->setTag(1);
        bgScale->setOpacity(0);
        CCSize touchSize = CCSize(setBtnSize.width, 80);
        if (CCCommonUtils::isIosAndroidPad()) {
            touchSize = CCSize(setBtnSize.width, 160);
        }
        if(isInAlliance){
            m_allAlianceBtn = CCLoadSprite::createSprite("Btn_shoucang.png");
            if (CCCommonUtils::isIosAndroidPad()) {
                m_allAlianceBtn->setScale(2);
            }
            CCSize resBtnSize = m_allAlianceBtn->getContentSize();
            m_settingNode->addChild(m_allAlianceBtn);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_allAlianceBtn->setPosition(ccp(bgScale->getPositionX(),220));
            }
            else
                m_allAlianceBtn->setPosition(ccp(bgScale->getPositionX(),110));
            m_allAlianceBtn->setTag(6);
            m_alianceTouchLayer = CCLayerColor::create(ccc4(255, 0, 0, 0), touchSize.width,touchSize.height);
            m_settingNode->addChild(m_alianceTouchLayer);
            m_alianceTouchLayer->setPosition(m_allAlianceBtn->getPosition() - ccp(touchSize.width*0.5, touchSize.height*0.5));
            auto btnSprite3 = CCLoadSprite::createSprite("alliances_help.png");
            if (CCCommonUtils::isIosAndroidPad()) {
                btnSprite3->setScale(2);
            }
            m_settingNode->addChild(btnSprite3);
            btnSprite3->setTag(7);
            CCSize sprSize = btnSprite3->getContentSize();
            btnSprite3->setPosition(m_allAlianceBtn->getPosition());
            btnSprite3->setOpacity(0);
            m_alianceHideIcon = CCLoadSprite::createSprite("forbidden.png");
            if (CCCommonUtils::isIosAndroidPad()) {
                m_alianceHideIcon->setScale(2);
            }
            m_alianceHideIcon->setTag(8);
            m_settingNode->addChild(m_alianceHideIcon);
            m_alianceHideIcon->setAnchorPoint(ccp(1, 0));
            CCSize hideSize = m_alianceHideIcon->getContentSize();
            m_alianceHideIcon->setPosition(m_allAlianceBtn->getPosition() + ccp(resBtnSize.width - hideSize.width,0));
            m_allAlianceBtn->setOpacity(0);
            m_alianceHideIcon->setOpacity(0);
        }
        int posY = isInAlliance == true?200:110;
        if (CCCommonUtils::isIosAndroidPad()) {
            posY *= 2;
        }
        m_resourceBtn = CCLoadSprite::createSprite("Btn_shoucang.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            m_resourceBtn->setScale(2);
        }
        m_settingNode->addChild(m_resourceBtn);
        m_resourceBtn->setTag(2);
        m_resourceBtn->setPosition(ccp(bgScale->getPositionX(),posY));
        CCSize resBtnSize = m_resourceBtn->getContentSize();
        m_resourceTouchLayer = CCLayerColor::create(ccc4(255, 0, 0, 0), touchSize.width,touchSize.height);
        m_settingNode->addChild(m_resourceTouchLayer);
        m_resourceTouchLayer->setPosition(m_resourceBtn->getPosition() - ccp(touchSize.width*0.5, touchSize.height*0.5));
        
        auto btnSprite = CCLoadSprite::createSprite("ziyuan.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            btnSprite->setScale(2);
        }
        m_settingNode->addChild(btnSprite);
        btnSprite->setTag(3);
        CCSize sprSize = btnSprite->getContentSize();
        btnSprite->setPosition(m_resourceBtn->getPosition());
        btnSprite->setOpacity(0);
        m_hideIcon = CCLoadSprite::createSprite("forbidden.png");
        m_settingNode->addChild(m_hideIcon);
        m_hideIcon->setTag(4);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_hideIcon->setScale(2);
        }
        m_hideIcon->setAnchorPoint(ccp(1, 0));
        CCSize hideSize = m_hideIcon->getContentSize();
        m_hideIcon->setPosition(m_resourceBtn->getPosition() + ccp(resBtnSize.width - hideSize.width,0));
        m_hideIcon->setVisible(true);
        m_hideIcon->setOpacity(0);
        m_resourceBtn->setOpacity(0);
        
        // 转着圈圈表示正在加载的图片
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            m_loadingIcon->setScale(2);
        }
        auto loadingSize = m_loadingIcon->getContentSize();
        m_loadingIcon->setPosition(m_serverListBtn->getPosition());
        m_loadingIcon->setVisible(false);
        this->addChild(m_loadingIcon);
        
        initMap();
        addEdge(0,0);
        getCurrentServerData();
        showSelfCity();
        onShowMarkLine(NULL);
        addRelic();
        getData();
        addTitle();
        addLegend();
        update(1.0);
       
        ret = true;
    }
    return ret;
}

void MinimapView::loadingIconRun()
{
    m_loadingIcon->setVisible(true);
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
}

void MinimapView::loadingIconStopRun()
{
    m_loadingIcon->setVisible(false);
    m_loadingIcon->stopAllActions();
}

void MinimapView::addRelic(){
    addCity(WorldController::getInstance()->getIndexByPoint(ccp(_relic_center_x, _relic_center_y)), 3);
}

void MinimapView::addLegend(){
    if(this->getChildByTag(Legend_Node_Tag)){
        return;
    }
    auto node = CCNode::create();
    node->setTag(Legend_Node_Tag);
    if (CCCommonUtils::isIosAndroidPad()) {
        node->setScale(MINIMAP_HD_SCALE);
    }
    this->addChild(node);
    int totalW = 0;
    int totalH = 10;
    auto bg = CCLoadSprite::createScale9Sprite("technology_11.png");
    node->addChild(bg, 1);
    bg->setAnchorPoint(ccp(0, 0));
    auto addText = [](CCLabelIF *label, std::string iconStr, int &w, int &h, CCNode *parent){
        label->setColor(ccc3(221, 221, 221));
        label->setAnchorPoint(ccp(0, 0.5));
        label->setAlignment(kCCTextAlignmentLeft);
        label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        auto sprite = CCLoadSprite::createSprite(iconStr.c_str());
        float s = 1.0;
        if(iconStr == "position_alli.png"){
            s = 0.8;
        }
        int orgW = 40;
        int spriteW = orgW * s;
        int spriteH = orgW * s;

        CCCommonUtils::setSpriteMaxSize(sprite, spriteW);
        auto spriteSize = sprite->getContentSize();
        
        parent->addChild(sprite, 2);
        parent->addChild(label, 2);
        int textW = label->getContentSize().width * label->getOriginScaleX();
        int textH = label->getContentSize().height * label->getOriginScaleY();
        
        int gapX = 5;
        int gapY = 3;
        int showH = textH;
        int showW = textW + spriteW + gapX * 3;

        if(showW > w){
            w = showW;
        }
        if(showH < spriteH){
            showH = spriteH;
        }
        showH += 2 * gapY;
        sprite->setPosition(ccp(orgW / 2 + gapX, h + showH / 2));
        label->setPosition(orgW + gapX * 2, h + showH / 2);
        h += showH;
    };

    if(GlobalData::shared()->playerInfo.isInAlliance()){
        addText(CCLabelIF::create(_lang("115357").c_str()), "position_territory.png", totalW, totalH, node);
        addText(CCLabelIF::create(_lang("110099").c_str()), "position_alli.png", totalW, totalH, node);
        addText(CCLabelIF::create(_lang("103756").c_str()), "position_lord.png", totalW, totalH, node);
    }
    addText(CCLabelIF::create(_lang("108678").c_str()), "position_city.png", totalW, totalH, node);

    totalW += 15;
    totalH += 10;
    bg->setPreferredSize(CCSize(totalW, totalH));
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto orgPos = CCDirector::sharedDirector()->getVisibleOrigin();
    node->setPosition((size.width - totalW) * 0.5, orgPos.y + 20);
    if(isAllShow){
        showLegendEffect();
        if(GlobalData::shared()->playerInfo.isInAlliance()){
            m_alianceHideIcon->setVisible(false);
        }
    }
}

void MinimapView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MinimapView::onMemberDataBack), GET_MEMBER_POINT_BACK, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    if(WorldMapView::instance()){
        WorldMapView::instance()->m_touchDelegateView->notMove = true;
    }
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MinimapView::onShowMarkLine), MSG_MARK_LINE_AL, NULL);
}

void MinimapView::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    if(WorldMapView::instance()){
        WorldMapView::instance()->m_touchDelegateView->notMove = false;
    }
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GET_MEMBER_POINT_BACK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MARK_LINE_AL);
    PopupBaseView::onExit();
}

void MinimapView::addTitle(){
    m_nameContainer->removeAllChildren();
    auto sprite1 = CCLoadSprite::createSprite("UI-tishikuang.png");
    m_nameContainer->addChild(sprite1);
    auto sprite2 = CCLoadSprite::createSprite("UI-tishikuang.png");
    m_nameContainer->addChild(sprite2);
    auto sprite3 = CCLoadSprite::createSprite("UI-tishikuang.png");
    m_nameContainer->addChild(sprite3);
    auto sprite4 = CCLoadSprite::createSprite("UI-tishikuang.png");
    m_nameContainer->addChild(sprite4);
    //sprite1->setPosition(pos);
    sprite1->setFlipY(true);
    sprite1->setFlipX(true);
    sprite1->setAnchorPoint(ccp(1, 0));
    //sprite2->setPosition(pos);
    sprite2->setFlipY(true);
    sprite2->setAnchorPoint(ccp(0, 0));
    //sprite3->setPosition(pos);
    sprite3->setAnchorPoint(ccp(1, 1));
    sprite3->setFlipX(true);
    //sprite4->setPosition(pos);
    sprite4->setAnchorPoint(ccp(0, 1));
    m_title = CCLabelIF::create();
    m_nameContainer->addChild(m_title);
    //m_title->setPosition(pos);
    m_title->setFontSize(24);
    m_title->setColor(ccc3(220, 180, 90));
    sprite1->setScale(0.45);
    sprite2->setScale(0.45);
    sprite3->setScale(0.45);
    sprite4->setScale(0.45);
    
    std::string kingName = "";
    std::string banner = "";
    std::string allianceName = "";
    std::string titleStr = _lang("108758");
    for(auto it = WorldController::getInstance()->m_serverList.begin(); it != WorldController::getInstance()->m_serverList.end(); it++){
        if(it->serverId == m_currentServerId){
            titleStr = it->serverName;
            kingName = it->kingName;
            banner = it->banner;
            allianceName = it->abbr;
            break;
        }
    }
    int posY = sprite3->getPositionY() - sprite3->getContentSize().height / 2 + 15;
    int posX = sprite3->getPositionX();
    
    if(kingName != ""){
        std::string str = "";
        if(allianceName != ""){
            str = str + "(" + allianceName + ")";
        }
        str = str + kingName;
        auto kingNameBG = CCLoadSprite::createSprite("name_bg.png");
        kingNameBG->setScaleX(312.0 / kingNameBG->getContentSize().width);
        m_nameContainer->addChild(kingNameBG);
        kingNameBG->setPosition(ccp(posX, posY - kingNameBG->getContentSize().height / 2));
        auto nameLabel = CCLabelIF::create(_lang_1("110025", ""));
        nameLabel->setAnchorPoint(ccp(0, 0.5));
        nameLabel->setAlignment(kCCTextAlignmentLeft);
        nameLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        nameLabel->setColor(ccc3(255,194,3));
        nameLabel->setFontSize(18);
        m_nameContainer->addChild(nameLabel);
        int lw = nameLabel->getOriginScaleX() * nameLabel->getContentSize().width;
        auto nameText = CCLabelIFTTF::create(str.c_str());
        nameText->setColor(ccc3(255,194,3));
        nameText->setFontSize(24);
        nameText->setAnchorPoint(ccp(0, 0.5));
        nameText->setAlignment(kCCTextAlignmentLeft);
        nameText->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        int nw = nameText->getOriginScaleX() * nameText->getContentSize().width;
        m_nameContainer->addChild(nameText);
        int lnw = (lw + nw) / 2;
        
        int startX = 200;
        nameLabel->setPosition(posX - kingNameBG->getContentSize().width * kingNameBG->getScaleX() / 2 + 30, kingNameBG->getPositionY());
        nameText->setPosition(ccp(nameLabel->getPositionX() + 5 + lw, kingNameBG->getPositionY()));
        posY -= kingNameBG->getContentSize().height;
        if(banner != "" && CCCommonUtils::isShowFlagForChinese()){
            if((banner=="TW" || banner=="HK") && CCCommonUtils::checkTaiWanFlag()){
                banner = "CN";
            }
            banner = CCCommonUtils::changeChinaFlag(banner);
            auto bannerSprite = CCLoadSprite::createSprite((banner + ".png").c_str());
            m_nameContainer->addChild(bannerSprite);
            bannerSprite->setScale(0.25);
            bannerSprite->setPosition(ccp(posX + lnw + bannerSprite->getContentSize().width / 2 * bannerSprite->getScale() + 20, nameLabel->getPositionY()));
        }
    }
    
    WorldActivityState state = WorldController::getInstance()->getKingActivityStateByType(WorldActivityType::FIGHT_OF_KING);

    if(m_currentServerId == GlobalData::shared()->playerInfo.selfServerId && state != NotOpen){
        auto kingNameBG = CCLoadSprite::createSprite("UI_facebook_title.png");
        m_nameContainer->addChild(kingNameBG);
        kingNameBG->setAnchorPoint(ccp(0, 0.5));
        kingNameBG->setPosition(ccp(posX, posY - kingNameBG->getContentSize().height / 2));
        kingNameBG->setScaleX(312.0 / kingNameBG->getContentSize().width / 2.0);

        auto kingNameBG1 = CCLoadSprite::createSprite("UI_facebook_title.png");
        m_nameContainer->addChild(kingNameBG1);
        kingNameBG1->setFlipX(true);
        kingNameBG1->setAnchorPoint(ccp(1, 0.5));
        kingNameBG1->setPosition(ccp(posX, posY - kingNameBG->getContentSize().height / 2));
        kingNameBG1->setScaleX(312.0 / kingNameBG1->getContentSize().width / 2.0);

        std::string dialogId = "";
        if(state == OpenNoKing){
            dialogId = "110087";
        }else if(state == PeaceTime){
            dialogId = "110086";
        }else if(state == WarTime){
            dialogId = "110085";
        }
        
        auto label = CCLabelIF::create(_lang(dialogId).c_str());
        label->setAlignment(kCCTextAlignmentLeft);
        label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        label->setAnchorPoint(ccp(0, 0.5));
        label->setColor(ccc3(241,15,115));
        label->setFontSize(20);
        m_nameContainer->addChild(label);
        int lw = label->getOriginScaleX() * label->getContentSize().width;
        
        m_timeText = CCLabelIF::create("00:00:00");
        m_timeText->setColor(ccc3(255,242,201));
        m_timeText->setFontSize(20);
        m_timeText->setAnchorPoint(ccp(0, 0.5));
        m_timeText->setAlignment(kCCTextAlignmentLeft);
        m_timeText->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        m_nameContainer->addChild(m_timeText);
        
        int startX = 200;
        label->setPosition(posX - kingNameBG->getContentSize().width * kingNameBG->getScaleX() + 50, kingNameBG->getPositionY());
        m_timeText->setPosition(label->getPositionX() + 5 + lw, kingNameBG->getPositionY());
    }
    
    m_title->setString(titleStr);
}


bool MinimapView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_curPt = pTouch->getLocation();
    if(((isTouchInside(m_resourceTouchLayer, pTouch) && m_resourceBtn->getOpacity()>0))
       || isTouchInside(m_serverListBtn, pTouch)
       || isTouchInside(m_returnBtn, pTouch)
       || isTouchInside(m_goBackMyServerBtn, pTouch)
       || (isTouchInside(m_alianceTouchLayer, pTouch) && m_allAlianceBtn->isVisible() && m_allAlianceBtn->getOpacity()>0 )
       || isTouchInside(m_settingBtn, pTouch)
       || isTouchInside(m_markTouchNode, pTouch)){
        return true;
    }
    else if (isTouchInside(m_goToServerBtn1, pTouch)){
        if (!m_goToServerBtn1->isVisible()) {
            return false;
        }
        m_goToServerBtn1->setScale(0.8);
        return true;
    }
    else if (isTouchInside(m_goToServerBtn2, pTouch)){
        if (!m_goToServerBtn2->isVisible()) {
            return false;
        }
        m_goToServerBtn2->setScale(0.8);
        return true;
    }
    else if (isTouchInside(m_goToServerBtn3, pTouch)){
        if (!m_goToServerBtn3->isVisible()) {
            return false;
        }
        m_goToServerBtn3->setScale(0.8);
        return true;
    }
    else if (isTouchInside(m_goToServerBtn4, pTouch)){
        if (!m_goToServerBtn4->isVisible()) {
            return false;
        }
        m_goToServerBtn4->setScale(0.8);
        return true;
    }
    return false;
}

void MinimapView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if (fabs(pTouch->getLocation().x-m_curPt.x) > 30 || fabs(pTouch->getLocation().y-m_curPt.y) > 30)
    {
        m_goToServerBtn1->setScale(1.0);
        m_goToServerBtn2->setScale(1.0);
        m_goToServerBtn3->setScale(1.0);
        m_goToServerBtn4->setScale(1.0);
    }
}

void MinimapView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if(isTouchInside(m_resourceTouchLayer, pTouch) && m_resourceBtn->getOpacity()>0){
        clickResourceBtn(NULL, Control::EventType::TOUCH_DOWN);
    }else if(isTouchInside(m_serverListBtn, pTouch)){
        clickServerBtn();
    }else if(isTouchInside(m_returnBtn, pTouch)){
        doClickBackBtn();
    }else if (isTouchInside(m_goBackMyServerBtn, pTouch)){
        doClickGoBackMyServerBtn();
    }else if(isTouchInside(m_alianceTouchLayer, pTouch)){
        doClickShowMember();
    }else if (isTouchInside(m_goToServerBtn1, pTouch)){
        m_goToServerBtn1->setScale(1.0);
        doClickGoToServerBtn(1);
    }else if (isTouchInside(m_goToServerBtn2, pTouch)){
        m_goToServerBtn2->setScale(1.0);
        doClickGoToServerBtn(2);
    }else if (isTouchInside(m_goToServerBtn3, pTouch)){
        m_goToServerBtn3->setScale(1.0);
        doClickGoToServerBtn(3);
    }else if (isTouchInside(m_goToServerBtn4, pTouch)){
        m_goToServerBtn4->setScale(1.0);
        doClickGoToServerBtn(4);
    }else if(isTouchInside(m_settingBtn, pTouch)){
        showSettingNode();
    }else if(isTouchInside(m_markTouchNode, pTouch)){
        auto singleTouch = m_container->convertToNodeSpace(pTouch->getLocation());
        onGoToMarkPt(singleTouch);
    }
}

void MinimapView::clickResourceBtn(cocos2d::CCObject *pTarget, CCControlEvent touchEvent){
    if(m_resourceIntroPointer->getChildrenCount() > 0){
        hideResource();
    }else{
        openResourceWithAnimation();
    }
}

void MinimapView::doClickBackBtn(){
    closeSelf();
}

void MinimapView::doClickGoBackMyServerBtn(){
    GlobalData::shared()->playerInfo.currentServerId = GlobalData::shared()->playerInfo.selfServerId;
}

void MinimapView::doClickGoToServerBtn(int index){
    if(m_resourceIntroPointer->getChildrenCount() > 0){
        hideResource();
    }
    m_selectIndex = index;
    std::string goToSerName = getGoToServerData(m_selectIndex);
    CCPoint temp = m_container->getPosition();
    YesNoDialog::show(_lang_1("108899",goToSerName.c_str()), CCCallFunc::create(this, callfunc_selector(MinimapView::moveMapWithAnimation)));
}

std::string MinimapView::getGoToServerData(int index){
    int serverId = -1;
    int curSerX = 0;
    int curSerY = 0;
    int goToSerX = 0;
    int goToSerY = 0;
    switch (index) {
        case 1:{
            CCPoint curSerPt = WorldController::getInstance()->getServerPosById(m_currentServerId);
            curSerX = curSerPt.x;
            curSerY = curSerPt.y;
            goToSerX = curSerX + 1;
            goToSerY = curSerY;
            serverId = WorldController::getInstance()->getServerIdByServerPoint(CCPoint(goToSerX, goToSerY));
        }
            break;
        case 2:{
            CCPoint curSerPt = WorldController::getInstance()->getServerPosById(m_currentServerId);
            curSerX = curSerPt.x;
            curSerY = curSerPt.y;
            goToSerX = curSerX;
            goToSerY = curSerY - 1;
            serverId = WorldController::getInstance()->getServerIdByServerPoint(CCPoint(goToSerX, goToSerY));
        }
            break;
        case 3:{
            CCPoint curSerPt = WorldController::getInstance()->getServerPosById(m_currentServerId);
            curSerX = curSerPt.x;
            curSerY = curSerPt.y;
            goToSerX = curSerX;
            goToSerY = curSerY + 1;
            serverId = WorldController::getInstance()->getServerIdByServerPoint(CCPoint(goToSerX, goToSerY));
        }
            break;
        case 4:{
            CCPoint curSerPt = WorldController::getInstance()->getServerPosById(m_currentServerId);
            curSerX = curSerPt.x;
            curSerY = curSerPt.y;
            goToSerX = curSerX - 1;
            goToSerY = curSerY;
            serverId = WorldController::getInstance()->getServerIdByServerPoint(CCPoint(goToSerX, goToSerY));
        }
            break;
        default:
            break;
    }
    for(auto it = WorldController::getInstance()->m_serverList.begin(); it != WorldController::getInstance()->m_serverList.end(); it++){
        if(it->serverId == serverId){
            return it->serverName;
        }
    }
    return "";
}

void MinimapView::moveMapWithAnimation(){
    switch (m_selectIndex) {
        case 1:{
            addExtraMap(m_selectIndex);
            m_container->stopAllActions();
            auto size = CCDirector::sharedDirector()->getWinSize();
            CCActionInterval *map_move = CCMoveTo::create(0.5, ccp(-1145, -505));
            CCActionInterval *map_delay = CCDelayTime::create(0.5);
            m_container->runAction(CCSequence::create(map_move,map_delay,CCCallFunc::create(this, callfunc_selector(MinimapView::refreshMinimapView)),NULL));
        }
            break;
        case 2:{
            addExtraMap(m_selectIndex);
            m_container->stopAllActions();
            auto size = CCDirector::sharedDirector()->getWinSize();
            CCActionInterval *map_move = CCMoveTo::create(0.5, ccp(-1145, 455));
            CCActionInterval *map_delay = CCDelayTime::create(0.5);
            m_container->runAction(CCSequence::create(map_move,map_delay,CCCallFunc::create(this, callfunc_selector(MinimapView::refreshMinimapView)),NULL));
        }
            break;
        case 3:{
            addExtraMap(m_selectIndex);
            m_container->stopAllActions();
            auto size = CCDirector::sharedDirector()->getWinSize();
            CCActionInterval *map_move = CCMoveTo::create(0.5, ccp(-104, -505));
            CCActionInterval *map_delay = CCDelayTime::create(0.5);
            m_container->runAction(CCSequence::create(map_move,map_delay,CCCallFunc::create(this, callfunc_selector(MinimapView::refreshMinimapView)),NULL));
        }
            break;
        case 4:{
            addExtraMap(m_selectIndex);
            m_container->stopAllActions();
            auto size = CCDirector::sharedDirector()->getWinSize();
            CCActionInterval *map_move = CCMoveTo::create(0.5, ccp(-104, 455));
            CCActionInterval *map_delay = CCDelayTime::create(0.5);
            m_container->runAction(CCSequence::create(map_move,map_delay,CCCallFunc::create(this, callfunc_selector(MinimapView::refreshMinimapView)),NULL));
        }
            break;
        default:
            break;
    }
}

void MinimapView::refreshMinimapView(){
    if (m_cityBatchNode->getChildrenCount() > 0) {
       m_cityBatchNode->removeAllChildrenWithCleanup(true);
    }
    switch (m_selectIndex) {
        case 1:{
            m_container->setPosition(m_container->getPosition() + ccp(960, 480));
            removeExtraMap();
            m_currentServerId = m_rServerId;
        }
            break;
        case 2:{
            m_container->setPosition(m_container->getPosition() + ccp(960, -480));
            removeExtraMap();
            m_currentServerId = m_dServerId;
        }
            break;
        case 3:{
            m_container->setPosition(m_container->getPosition() + ccp(-960, 480));
            removeExtraMap();
            m_currentServerId = m_uServerId;
        }
            break;
        case 4:{
            m_container->setPosition(m_container->getPosition() + ccp(-960, -480));
            removeExtraMap();
            m_currentServerId = m_lServerId;
        }
            break;
        default:
            break;
    }
    addTitle();///////////////
    addRelic();
    if (m_currentServerId == GlobalData::shared()->playerInfo.selfServerId) {
        addCity(m_mapInfo->selfPt, 0);
        CCPoint ptByIndex = WorldController::getPointByIndex(m_mapInfo->selfPt);
        auto disPoint = ccpSub(ptByIndex, WorldMapView::instance()->m_map->centerTilePoint);
        CCPoint worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
        auto cityPt = getPointByWorldPoint(worldPt);
        m_phoneIcon->setVisible(true);
        m_phoneIcon->setPosition(cityPt);
        showAllianceMember();
    }
    getCurrentServerData();
    if (m_currentServerId != GlobalData::shared()->playerInfo.selfServerId) {
        m_phoneIcon->setVisible(false);
    }
    m_selectIndex = 0;
}

void MinimapView::addExtraMap(int index){
    CCPoint point1;
    CCPoint point2;
    CCPoint point3;
    switch (index) {
        case 1:{
            point1 = ccp(viewCenterPoint.x + 1920 - 6, viewCenterPoint.y + 960 - 6);
            point2 = ccp(viewCenterPoint.x + 1920 - 6, viewCenterPoint.y);
            point3 = ccp(viewCenterPoint.x + 960 - 3, viewCenterPoint.y + 1440 - 4.5);
        }
            break;
        case 2:{
            point1 = ccp(viewCenterPoint.x + 1920 - 6, viewCenterPoint.y - 960 + 6);
            point2 = ccp(viewCenterPoint.x + 1920 - 6, viewCenterPoint.y);
            point3 = ccp(viewCenterPoint.x + 960 - 3, viewCenterPoint.y - 1440 + 4.5);
        }
            break;
        case 3:{
            point1 = ccp(viewCenterPoint.x - 1920 + 6, viewCenterPoint.y + 960 - 6);
            point2 = ccp(viewCenterPoint.x - 1920 + 6, viewCenterPoint.y);
            point3 = ccp(viewCenterPoint.x - 960 + 3, viewCenterPoint.y + 1440 - 4.5);
        }
            break;
        case 4:{
            point1 = ccp(viewCenterPoint.x - 1920 + 6, viewCenterPoint.y - 960 + 6);
            point2 = ccp(viewCenterPoint.x - 1920 + 6, viewCenterPoint.y);
            point3 = ccp(viewCenterPoint.x - 960 + 3, viewCenterPoint.y - 1440 + 4.5);
        }
            break;
        default:
            break;
    }
    addMapTile(point1, 7);
    addMapTile(point2, 8);
    addMapTile(point3, 9);
    m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag + 7), 1);
    m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag + 8), 2);
    m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag + 9), 3);
}

void MinimapView::removeExtraMap(){
    m_mapBatchNode->removeChildByTag(Map_Node_Tag + 7, true);
    m_mapBatchNode->removeChildByTag(Map_Node_Tag + 8, true);
    m_mapBatchNode->removeChildByTag(Map_Node_Tag + 9, true);
}

void MinimapView::doClickShowMember(){
    isAllShow = !isAllShow;
    setAllianceMemeberVisible(isAllShow);
}

void MinimapView::clickServerBtn()
{
    if(m_bCanClickServerBtn)
    {
        m_bCanClickServerBtn = false;
        loadingIconRun();
        this->scheduleOnce(schedule_selector(MinimapView::doClickServerBtn),0.1);
    }
}

void MinimapView::doClickServerBtn(float _time)
{
    auto pop = ServerListPopUpView::create();
    pop->setTag(miniMapTag);
    PopupViewController::getInstance()->addPopupInView(pop);
    loadingIconStopRun();
    m_bCanClickServerBtn = true;
}

void MinimapView::getData(){
    if(!GlobalData::shared()->playerInfo.isInAlliance()){
        return;
    }
    if(!GlobalData::shared()->playerInfo.isInSelfServer()){
        return;
    }
    if (m_currentServerId != GlobalData::shared()->playerInfo.selfServerId) {
        return;
    }
    GetAllianceMemberPointCommand *cmd = new GetAllianceMemberPointCommand();
    cmd->sendAndRelease();
}

// ITouchDelegate
bool MinimapView::onBeginScroll(const cocos2d::CCPoint& worldLocation){
    
}

void MinimapView::onStepScroll(const cocos2d::CCPoint& worldLocation,const cocos2d::CCPoint& delta){
    
}

void MinimapView::onEndScroll(const cocos2d::CCPoint& worldLocation){
    
}

void MinimapView::onSingleTouchBegin(const cocos2d::CCPoint& worldLocation){
    
}

void MinimapView::onSingleTouchEnd(const cocos2d::CCPoint& worldLocation){
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    CCPoint pt = ccp(-1, -1);
    int index = 0;
    if(m_cityBatchNode->getChildrenCount()>0){
        index = m_cityBatchNode->getChildrenCount() - 1;
    }
    bool isTouchInCity = false;
    while (index >= 0) {
        auto node = dynamic_cast<CCNode*>(m_cityBatchNode->getChildren().at(index));
        auto size = node->getContentSize();
        CCRect rect = CCRect(node->getPositionX() - size.width / 2, node->getPositionY() - size.height / 2, size.width, size.height);
        if(rect.containsPoint(worldLocation)){
            int cityIndex = node->getTag();
            pt = WorldController::getPointByIndex(cityIndex);
            isTouchInCity = true;
            break;
        }
        index--;
    }
    if(!isTouchInCity){
        pt = getWorldPointByPoint(worldLocation);
    }
    if(WorldController::getInstance()->isInMap(pt)){
        GlobalData::shared()->playerInfo.currentServerId = m_currentServerId;
        if (!GlobalData::shared()->playerInfo.isInSelfServer()) {
            WorldMapView::instance()->removeCover();
            WorldMapView::instance()->addCover();
        }
        else
            WorldMapView::instance()->removeCover();
        
        WorldMapView::instance()->gotoTilePoint(pt,m_currentServerId);
        singleTouchClose = true;
        auto size = CCDirector::sharedDirector()->getWinSize();
        if (!m_phoneIcon->isVisible()) {
            m_phoneIcon->setVisible(true);
        }
        m_phoneIcon->setPosition(worldLocation);
        m_viewPort->scrollTo(-worldLocation + ccp(size.width / 2, size.height / 2), CCCallFunc::create(this, callfunc_selector(MinimapView::moveBack)), true);
    }
}

void MinimapView::moveBack(){
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto pt = m_container->getPosition() - size / 2;
    float s = 6.0f;
    float time = 0.4f;
    CCSpawn *spawn =CCSpawn::createWithTwoActions(CCMoveTo::create(time, pt * s + size / 2), CCScaleTo::create(time, s));
    m_container->runAction(CCSequence::create(spawn,
                                              CCCallFunc::create(this, callfunc_selector(MinimapView::closeSelf)),
                                              NULL));
}

void MinimapView::scaleBack(){
    auto centerPt = getRealPosByPoint(viewCenterPoint);
    auto size = CCDirector::sharedDirector()->getWinSize();
    CCCallFunc *fun = NULL;
    
    bool isOpened = CCUserDefault::sharedUserDefault()->getBoolForKey("isMiniMapOpened");
    if(!isOpened){
        fun = CCCallFunc::create(this, callfunc_selector(MinimapView::showResourceCallBack));
        m_viewPort->scrollTo(-centerPt + ccp(size.width / 2, size.height / 2) - ccp(200, 200), fun, true);
    }else{
        fun = CCCallFunc::create(this, callfunc_selector(MinimapView::hideResource));
    }
}

void MinimapView::openResourceWithAnimation(){
    auto centerPt = getRealPosByPoint(viewCenterPoint);
    auto size = CCDirector::sharedDirector()->getWinSize();
    CCCallFunc *fun = NULL;
    fun = CCCallFunc::create(this, callfunc_selector(MinimapView::openResourceAnimationBack));
    m_viewPort->scrollTo(-centerPt + ccp(size.width / 2, size.height / 2) - ccp(200, 200), fun, true);
}

void MinimapView::openResourceAnimationBack(){
    showResource(true);
}

void MinimapView::onBeginZoom(const CCPoint& point1,const CCPoint& point2){
    
}

void MinimapView::onEndZoom(float targetZoom){
    
}

void MinimapView::closeSelf(){
    if (m_currentServerId == GlobalData::shared()->playerInfo.selfServerId) {
        WorldMapView::instance()->updateDirection();
        PopupViewController::getInstance()->goBackPopupView();
    }
    else {
        if (singleTouchClose) {
            PopupViewController::getInstance()->removeAllPopupView();
        }
        else {
            PopupViewController::getInstance()->removeAllPopupView();
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                if(WorldMapView::instance() && WorldMapView::instance()->m_map){
                    if(m_currentServerId == GlobalData::shared()->playerInfo.currentServerId){
                        WorldMapView::instance()->gotoTilePoint(WorldController::getInstance()->selfPoint, m_currentServerId, true);
                    }else{
                        WorldMapView::instance()->gotoTilePoint(ccp(600, 600), m_currentServerId, true);
                    }
                }
            }else{
                SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
            }
        }
    }
}

void MinimapView::onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2){
}

void MinimapView::addMapTile(cocos2d::CCPoint midPt, int tag){
    auto sprite1 = CCLoadSprite::createSprite("minimap_1.png");
    sprite1->setPosition(midPt);
    m_mapBatchNode->addChild(sprite1);
    sprite1->setTag(Map_Node_Tag + tag);
    sprite1->setBatchNode(m_mapBatchNode);
    
    auto sprite2 = CCLoadSprite::createSprite("minimap_2.png");
    sprite2->setPosition(ccp(442, 628.5));
    sprite1->addChild(sprite2);
    
    auto sprite3 = CCLoadSprite::createSprite("minimap_3.png");
    sprite3->setPosition(ccp(442, -110.5));
    sprite1->addChild(sprite3);
    
    auto sprite4 = CCLoadSprite::createSprite("minimap_4.png");
    sprite4->setPosition(ccp(-259, 259));
    sprite1->addChild(sprite4);
    
    auto sprite5 = CCLoadSprite::createSprite("minimap_5.png");
    sprite5->setPosition(ccp(1143, 259));
    sprite1->addChild(sprite5);
}

void MinimapView::initMap(){
    int mapW = m_mapW;
    int mapH = m_mapH;
    
    //中间
    CCPoint midPoint = ccp(viewCenterPoint.x, viewCenterPoint.y);
    auto sprite6 = CCLoadSprite::createSprite("minimap_Cover.png");
    sprite6->setPosition(midPoint);
    sprite6->setScale(40);
    sprite6->setOpacity(102);
    m_mapBatchNode->addChild(sprite6);
    sprite6->setTag(Cover_Node_Tag);
    sprite6->setBatchNode(m_mapBatchNode);
    
    addMapTile(midPoint, 0);
    
    //左下
    midPoint = ccp(viewCenterPoint.x - 960 + 3, viewCenterPoint.y - 480 + 3);
    addMapTile(midPoint, 1);
    
    //左上
    midPoint = ccp(viewCenterPoint.x - 960 + 3, viewCenterPoint.y + 480 - 3);
    addMapTile(midPoint, 2);
    
    //右上
    midPoint = ccp(viewCenterPoint.x + 960 - 3, viewCenterPoint.y + 480 - 3);
    addMapTile(midPoint, 3);
    
    //右下
    midPoint = ccp(viewCenterPoint.x + 960 - 3, viewCenterPoint.y - 480 + 3);
    addMapTile(midPoint, 4);
    
    //上
    midPoint = ccp(viewCenterPoint.x, viewCenterPoint.y + 960 - 6);
    addMapTile(midPoint, 5);
    
    //下
    midPoint = ccp(viewCenterPoint.x, viewCenterPoint.y - 960 + 6);
    addMapTile(midPoint, 6);
}

void MinimapView::addEdge(int x, int y){
    auto addEdgeFrame = [](CCSpriteBatchNode *parent, CCPoint &realPt, bool lSide, bool uSide, bool scale){
        auto aniSprite = CCLoadSprite::createSprite("edge_light_0.png");
        if (!scale) {
            if(lSide && uSide){
                aniSprite->setAnchorPoint(CCPoint(0, 0));
                aniSprite->setSkewY(26.5);
            }
            else if (lSide && !uSide){
                aniSprite->setAnchorPoint(CCPoint(0, 0));
                aniSprite->setSkewY(-26.5);
            }
            else if (!lSide && uSide){
                aniSprite->setAnchorPoint(CCPoint(1, 0));
                aniSprite->setSkewY(-26.5);
            }
            else if (!lSide && !uSide){
                aniSprite->setAnchorPoint(CCPoint(1, 0));
                aniSprite->setSkewY(26.5);
            }
        }
        else {
            if(lSide && uSide){
                aniSprite->setScaleX(0.5);
                aniSprite->setAnchorPoint(CCPoint(0, 0));
                aniSprite->setSkewY(13.25);
            }
            else if (lSide && !uSide){
                aniSprite->setScaleX(0.5);
                aniSprite->setAnchorPoint(CCPoint(0, 0));
                aniSprite->setSkewY(-13.25);
            }
            else if (!lSide && uSide){
                aniSprite->setScaleX(0.5);
                aniSprite->setAnchorPoint(CCPoint(1, 0));
                aniSprite->setSkewY(-13.25);
            }
            else if (!lSide && !uSide){
                aniSprite->setScaleX(0.5);
                aniSprite->setAnchorPoint(CCPoint(1, 0));
                aniSprite->setSkewY(13.25);
            }
        }
        aniSprite->setPosition(realPt);
        parent->addChild(aniSprite);
        aniSprite->setBatchNode(parent);
        
        int frameCount = 7;
        std::string temp = "edge_light_%d.png";
        Vector<SpriteFrame*> tmpAniArr;
        for (int i=0; i<frameCount; ++i) {
            tmpAniArr.pushBack(CCLoadSprite::loadResource(CCString::createWithFormat(temp.c_str(),i)->getCString()));
        }
        auto animation = CCAnimation::createWithSpriteFrames(tmpAniArr, 1.0 / tmpAniArr.size());
        animation->setLoops(std::numeric_limits<int>::max());
        auto ani = CCAnimate::create(animation);
        if (ani) {
            aniSprite->runAction(ani);
        }
    };
    
    for (int i = 0; i < 8; i++) {
        if (i != 7) {
            CCPoint realPt = viewCenterPoint + ccp(x - 960 + i * 128, y + i * 64);
            bool lSide = true;
            bool uSide = true;
            bool scale = false;
            addEdgeFrame(m_edgeBatchNode, realPt, lSide, uSide, scale);
            
            realPt = viewCenterPoint + ccp(x - 960 + i * 128, y - i * 64);
            lSide = true;
            uSide = false;
            addEdgeFrame(m_edgeBatchNode, realPt, lSide, uSide, scale);
            
            realPt = viewCenterPoint + ccp(x + 960 - i * 128, y + i * 64);
            lSide = false;
            uSide = true;
            addEdgeFrame(m_edgeBatchNode, realPt, lSide, uSide, scale);
            
            realPt = viewCenterPoint + ccp(x + 960 - i * 128, y - i * 64);
            lSide = false;
            uSide = false;
            addEdgeFrame(m_edgeBatchNode, realPt, lSide, uSide, scale);
        }
        else {
            CCPoint realPt = viewCenterPoint + ccp(x - 960 + i * 128, y + i * 64);
            bool lSide = true;
            bool uSide = true;
            bool scale = true;
            addEdgeFrame(m_edgeBatchNode, realPt, lSide, uSide, scale);
            
            realPt = viewCenterPoint + ccp(x - 960 + i * 128, y - i * 64);
            lSide = true;
            uSide = false;
            addEdgeFrame(m_edgeBatchNode, realPt, lSide, uSide, scale);
            
            realPt = viewCenterPoint + ccp(x + 960 - i * 128, y + i * 64);
            lSide = false;
            uSide = true;
            addEdgeFrame(m_edgeBatchNode, realPt, lSide, uSide, scale);
            
            realPt = viewCenterPoint + ccp(x + 960 - i * 128, y - i * 64);
            lSide = false;
            uSide = false;
            addEdgeFrame(m_edgeBatchNode, realPt, lSide, uSide, scale);
        }
    }
}

void MinimapView::getCurrentServerData(){
    CCPoint currentSerPt = WorldController::getInstance()->getServerPosById(m_currentServerId);
    m_currentServerX = currentSerPt.x;
    m_currentServerY = currentSerPt.y;
            
    int lSerX = m_currentServerX - 1;
    int lSerY = m_currentServerY;
    m_lServerId = WorldController::getInstance()->getServerIdByServerPoint(CCPoint(lSerX, lSerY));
    int uSerX = m_currentServerX;
    int uSerY = m_currentServerY + 1;
    m_uServerId = WorldController::getInstance()->getServerIdByServerPoint(CCPoint(uSerX, uSerY));
    int rSerX = m_currentServerX + 1;
    int rSerY = m_currentServerY;
    m_rServerId = WorldController::getInstance()->getServerIdByServerPoint(CCPoint(rSerX, rSerY));
    int dSerX = m_currentServerX;
    int dSerY = m_currentServerY - 1;
    m_dServerId = WorldController::getInstance()->getServerIdByServerPoint(CCPoint(dSerX, dSerY));

    if (m_currentServerId == GlobalData::shared()->playerInfo.selfServerId) {
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag), 11);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+1), 6);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+2), 7);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+3), 8);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+4), 9);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+5), 4);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+6), 5);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Cover_Node_Tag), 10);
//        m_mapBatchNode->getChildByTag(Map_Node_Tag)->setZOrder(11);
//        m_mapBatchNode->getChildByTag(Map_Node_Tag+1)->setZOrder(6);
//        m_mapBatchNode->getChildByTag(Map_Node_Tag+2)->setZOrder(7);
//        m_mapBatchNode->getChildByTag(Map_Node_Tag+3)->setZOrder(8);
//        m_mapBatchNode->getChildByTag(Map_Node_Tag+4)->setZOrder(9);
//        m_mapBatchNode->getChildByTag(Map_Node_Tag+5)->setZOrder(4);
//        m_mapBatchNode->getChildByTag(Map_Node_Tag+6)->setZOrder(5);
//        m_mapBatchNode->getChildByTag(Cover_Node_Tag)->setZOrder(10);
    }
    else if (m_lServerId == GlobalData::shared()->playerInfo.selfServerId) {
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag), 9);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+1), 11);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+2), 6);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+3), 7);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+4), 8);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+5), 4);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+6), 5);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Cover_Node_Tag), 10);
    }
    else if (m_uServerId == GlobalData::shared()->playerInfo.selfServerId) {
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag), 8);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+1), 9);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+2), 11);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+3), 6);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+4), 7);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+5), 4);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+6), 5);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Cover_Node_Tag), 10);
    }
    else if (m_rServerId == GlobalData::shared()->playerInfo.selfServerId) {
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag), 7);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+1), 8);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+2), 9);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+3), 11);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+4), 6);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+5), 4);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+6), 5);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Cover_Node_Tag), 10);
    }
    else if (m_dServerId == GlobalData::shared()->playerInfo.selfServerId) {
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag), 6);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+1), 7);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+2), 8);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+3), 9);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+4), 11);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+5), 4);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+6), 5);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Cover_Node_Tag), 10);
    }
    else {
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag), 10);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+1), 6);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+2), 7);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+3), 8);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+4), 9);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+5), 4);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Map_Node_Tag+6), 5);
        m_mapBatchNode->reorderChild(m_mapBatchNode->getChildByTag(Cover_Node_Tag), 11);
    }
    
//    m_mapBatchNode->getChildByTag(Map_Node_Tag)->getChildByTag(Cover_Node_Tag)->setVisible(false);
//    m_mapBatchNode->getChildByTag(Map_Node_Tag+1)->getChildByTag(Cover_Node_Tag)->setVisible(false);
//    m_mapBatchNode->getChildByTag(Map_Node_Tag+2)->getChildByTag(Cover_Node_Tag)->setVisible(false);
//    m_mapBatchNode->getChildByTag(Map_Node_Tag+3)->getChildByTag(Cover_Node_Tag)->setVisible(false);
//    m_mapBatchNode->getChildByTag(Map_Node_Tag+4)->getChildByTag(Cover_Node_Tag)->setVisible(false);
//    m_mapBatchNode->getChildByTag(Map_Node_Tag+5)->getChildByTag(Cover_Node_Tag)->setVisible(false);
//    m_mapBatchNode->getChildByTag(Map_Node_Tag+6)->getChildByTag(Cover_Node_Tag)->setVisible(false);
    //判断是否隐藏切服按钮
    //左
    if (m_lServerId > GlobalData::shared()->serverMax || m_lServerId <= 0) {
        m_goToServerBtn4->setVisible(false);
        m_lServerBg->setVisible(false);
        m_lSerTitle->setVisible(false);
    }
    else {
        m_goToServerBtn4->setVisible(true);
        m_lServerBg->setVisible(true);
        m_lSerTitle->setVisible(true);
    }
    //上
    if (m_uServerId > GlobalData::shared()->serverMax || m_uServerId <= 0) {
        m_goToServerBtn3->setVisible(false);
        m_uServerBg->setVisible(false);
        m_uSerTitle->setVisible(false);
    }
    else {
        m_goToServerBtn3->setVisible(true);
        m_uServerBg->setVisible(true);
        m_uSerTitle->setVisible(true);
    }
    //右
    if (m_rServerId > GlobalData::shared()->serverMax || m_rServerId <= 0) {
        m_goToServerBtn1->setVisible(false);
        m_rServerBg->setVisible(false);
        m_rSerTitle->setVisible(false);
    }
    else {
        m_goToServerBtn1->setVisible(true);
        m_rServerBg->setVisible(true);
        m_rSerTitle->setVisible(true);
    }
    //下
    if (m_dServerId > GlobalData::shared()->serverMax || m_dServerId <= 0) {
        m_goToServerBtn2->setVisible(false);
        m_dServerBg->setVisible(false);
        m_dSerTitle->setVisible(false);
    }
    else {
        m_goToServerBtn2->setVisible(true);
        m_dServerBg->setVisible(true);
        m_dSerTitle->setVisible(true);
    }
    
    if (m_arrowBatchNode->getChildByTag(Server_Banner_Tag)) {
        m_arrowBatchNode->removeChildByTag(Server_Banner_Tag);
    }
    if (m_arrowBatchNode->getChildByTag(Server_Banner_Tag + 1)) {
        m_arrowBatchNode->removeChildByTag(Server_Banner_Tag + 1);
    }
    if (m_arrowBatchNode->getChildByTag(Server_Banner_Tag + 2)) {
        m_arrowBatchNode->removeChildByTag(Server_Banner_Tag + 2);
    }
    if (m_arrowBatchNode->getChildByTag(Server_Banner_Tag + 3)) {
        m_arrowBatchNode->removeChildByTag(Server_Banner_Tag + 3);
    }
    
    if (true) {
        int serverCount = 4;
        std::string serverStr = _lang("108758");
        std::string serverBanner = "";
        for(auto it = WorldController::getInstance()->m_serverList.begin(); it != WorldController::getInstance()->m_serverList.end(); it++){
            if(it->serverId == m_lServerId){
                serverStr = it->serverName;
                m_lSerTitle->setString(serverStr);
                m_lSerTitle->setPosition(m_lServerBg->getPosition() - ccp(10, 0));
                int strL = m_lSerTitle->getContentSize().width * m_lSerTitle->getOriginScaleX();
                serverBanner = it->banner;
                if(serverBanner != ""){
                    if((serverBanner=="TW" || serverBanner=="HK") && CCCommonUtils::checkTaiWanFlag()){
                        serverBanner = "CN";
                    }
                    serverBanner = CCCommonUtils::changeChinaFlag(serverBanner);
                    auto bannerSprite = CCLoadSprite::createSprite((serverBanner + ".png").c_str());
                    bannerSprite->setVisible(CCCommonUtils::isShowFlagForChinese());
                    m_arrowBatchNode->addChild(bannerSprite);
                    bannerSprite->setTag(Server_Banner_Tag);
                    bannerSprite->setScale(0.25);
                    bannerSprite->setPosition(ccp(m_lSerTitle->getPositionX() + strL / 2 + bannerSprite->getContentSize().width / 2 * bannerSprite->getScale() + 20, m_lSerTitle->getPositionY()));
                }
                serverCount--;
                if (serverCount == 0) {
                    break;
                }
            }
            else if (it->serverId == m_uServerId){
                serverStr = it->serverName;
                m_uSerTitle->setString(serverStr);
                m_uSerTitle->setPosition(m_uServerBg->getPosition() - ccp(10, 0));
                int strL = m_uSerTitle->getContentSize().width * m_uSerTitle->getOriginScaleX();
                serverBanner = it->banner;
                if(serverBanner != ""){
                    if((serverBanner=="TW" || serverBanner=="HK") && CCCommonUtils::checkTaiWanFlag()){
                        serverBanner = "CN";
                    }
                    serverBanner = CCCommonUtils::changeChinaFlag(serverBanner);
                    auto bannerSprite = CCLoadSprite::createSprite((serverBanner + ".png").c_str());
                    bannerSprite->setVisible(CCCommonUtils::isShowFlagForChinese());
                    m_arrowBatchNode->addChild(bannerSprite);
                    bannerSprite->setTag(Server_Banner_Tag + 1);
                    bannerSprite->setScale(0.25);
                    bannerSprite->setPosition(ccp(m_uSerTitle->getPositionX() + strL / 2 + bannerSprite->getContentSize().width / 2 * bannerSprite->getScale() + 20, m_uSerTitle->getPositionY()));
                }
                serverCount--;
                if (serverCount == 0) {
                    break;
                }
            }
            else if (it->serverId == m_rServerId){
                serverStr = it->serverName;
                m_rSerTitle->setString(serverStr);
                m_rSerTitle->setPosition(m_rServerBg->getPosition() - ccp(10, 0));
                int strL = m_rSerTitle->getContentSize().width * m_rSerTitle->getOriginScaleX();
                serverBanner = it->banner;
                if(serverBanner != ""){
                    if((serverBanner=="TW" || serverBanner=="HK") && CCCommonUtils::checkTaiWanFlag()){
                        serverBanner = "CN";
                    }
                    serverBanner = CCCommonUtils::changeChinaFlag(serverBanner);
                    auto bannerSprite = CCLoadSprite::createSprite((serverBanner + ".png").c_str());
                    bannerSprite->setVisible(CCCommonUtils::isShowFlagForChinese());
                    m_arrowBatchNode->addChild(bannerSprite);
                    bannerSprite->setTag(Server_Banner_Tag + 2);
                    bannerSprite->setScale(0.25);
                    bannerSprite->setPosition(ccp(m_rSerTitle->getPositionX() + strL / 2 + bannerSprite->getContentSize().width / 2 * bannerSprite->getScale() + 20, m_rSerTitle->getPositionY()));
                }
                serverCount--;
                if (serverCount == 0) {
                    break;
                }
            }
            else if (it->serverId == m_dServerId){
                serverStr = it->serverName;
                m_dSerTitle->setString(serverStr);
                m_dSerTitle->setPosition(m_dServerBg->getPosition() - ccp(10, 0));
                int strL = m_dSerTitle->getContentSize().width * m_dSerTitle->getOriginScaleX();
                serverBanner = it->banner;
                if(serverBanner != ""){
                    if((serverBanner=="TW" || serverBanner=="HK") && CCCommonUtils::checkTaiWanFlag()){
                        serverBanner = "CN";
                    }
                    serverBanner = CCCommonUtils::changeChinaFlag(serverBanner);
                    auto bannerSprite = CCLoadSprite::createSprite((serverBanner + ".png").c_str());
                    bannerSprite->setVisible(CCCommonUtils::isShowFlagForChinese());
                    m_arrowBatchNode->addChild(bannerSprite);
                    bannerSprite->setTag(Server_Banner_Tag + 3);
                    bannerSprite->setScale(0.25);
                    bannerSprite->setPosition(ccp(m_dSerTitle->getPositionX() + strL / 2 + bannerSprite->getContentSize().width / 2 * bannerSprite->getScale() + 20, m_dSerTitle->getPositionY()));
                }
                serverCount--;
                if (serverCount == 0) {
                    break;
                }
            }
        }
    }
}

CCPoint MinimapView::getRealPosByPoint(CCPoint &pt){
    auto disPoint = ccpSub(pt, viewCenterPoint);
    return ccp(viewCenterPoint.x + (disPoint.x-disPoint.y) * _tile_width / 2,
               viewCenterPoint.y - (disPoint.x+disPoint.y) * _tile_height / 2);
}

void MinimapView::showResourceCallBack()
{
    showResource(false);
}

void MinimapView::showResource(bool flag){
    hideResource();
    m_hideIcon->setVisible(false);
    
    float s = 2.2;
    auto sprite1 = CCLoadSprite::createSprite("resource_belt.png");
    sprite1->setAnchorPoint(ccp(1, 0));
    sprite1->setOpacity(204);
    
    auto sprite2 = CCLoadSprite::createSprite("resource_belt.png");
    sprite2->setAnchorPoint(ccp(0, 0));
    sprite2->setFlipX(true);
    sprite2->setOpacity(204);
    
    auto sprite3 = CCLoadSprite::createSprite("resource_belt.png");
    sprite3->setAnchorPoint(ccp(1, 1));
    sprite3->setFlipY(true);
    sprite3->setOpacity(204);
    
    auto sprite4 = CCLoadSprite::createSprite("resource_belt.png");
    sprite4->setAnchorPoint(ccp(0, 1));
    sprite4->setFlipX(true);
    sprite4->setFlipY(true);
    sprite4->setOpacity(204);
    
    m_resBeltBatchNode->addChild(sprite1);
    m_resBeltBatchNode->addChild(sprite2);
    m_resBeltBatchNode->addChild(sprite3);
    m_resBeltBatchNode->addChild(sprite4);
    
    sprite1->setPosition(viewCenterPoint);
    sprite2->setPosition(viewCenterPoint);
    sprite3->setPosition(viewCenterPoint);
    sprite4->setPosition(viewCenterPoint);
    sprite1->setScale(s);
    sprite2->setScale(s);
    sprite3->setScale(s);
    sprite4->setScale(s);

    auto addResorceIntro = [](std::string str, CCNode *spriteParent, CCNode *textParent,CCPoint &pt, int index, bool showOneByOne){
        auto label = CCLabelIF::create();
        label->setColor(ccc3(0, 0, 0));
        label->setFontSize(20);
        textParent->addChild(label);
        label->setPosition(pt);
        label->setString(str.c_str());
        label->setMaxScaleXByWidth(300);
        float delayTime = 0.2 * (index - 1);
        float fadeTime = 0.3;
        if(showOneByOne){
            label->setOpacity(0);
            label->runAction(CCSequence::create(CCDelayTime::create(delayTime),
                                                CCFadeTo::create(fadeTime, 255 * 0.7),
                                                NULL
                                                ));
        }
        auto bg1 = CCLoadSprite::createScale9Sprite("Mail_frame009.png");
        bg1->setPreferredSize(CCSize(250, 60));
        bg1->setPosition(pt);
        if(showOneByOne){
            bg1->setOpacity(0);
            bg1->runAction(CCSequence::create(CCDelayTime::create(delayTime),
                                              CCFadeTo::create(fadeTime, 255 * 0.7),
                                              NULL
                                              ));
        }
        auto bg2 = CCLoadSprite::createSprite("Mail_frame_xiao.png");
        bg2->setPosition(ccp(-bg1->getContentSize().width / 2 - bg2->getContentSize().width / 2 + 1, 0) + pt);
        if(showOneByOne){
            bg2->setOpacity(0);
            bg2->runAction(CCSequence::create(CCDelayTime::create(delayTime),
                                              CCFadeTo::create(fadeTime, 255 * 0.7),
                                              NULL
                                              ));
        }

        spriteParent->addChild(bg2);
        spriteParent->addChild(bg1);
    };
    CCPoint pt = viewCenterPoint;
    CCPoint realPt = getRealPosByPoint(pt);
    auto beltPt = realPt;
    int gap = 75;
    realPt = realPt + ccp(200, 10);
    bool isOpened = CCUserDefault::sharedUserDefault()->getBoolForKey("isMiniMapOpened");
    CCUserDefault::sharedUserDefault()->setBoolForKey("isMiniMapOpened", true);
    bool showOneByOne = !isOpened || flag;
    addResorceIntro(_lang("103755"), m_resourceIntroPointer, m_resourceIntroText, realPt, 6, showOneByOne);
    realPt = realPt + ccp(20, gap);
    addResorceIntro(_lang("103754"), m_resourceIntroPointer, m_resourceIntroText, realPt, 5, showOneByOne);
    realPt = realPt + ccp(20, gap);
    addResorceIntro(_lang("103753"), m_resourceIntroPointer, m_resourceIntroText, realPt, 4, showOneByOne);
    realPt = realPt + ccp(20, gap);
    addResorceIntro(_lang("103752"), m_resourceIntroPointer, m_resourceIntroText, realPt, 3, showOneByOne);
    realPt = realPt + ccp(20, gap);
    addResorceIntro(_lang("103751"), m_resourceIntroPointer, m_resourceIntroText, realPt, 2, showOneByOne);
    realPt = realPt + ccp(20, gap);
    addResorceIntro(_lang("103750"), m_resourceIntroPointer, m_resourceIntroText, realPt, 1, showOneByOne);
    if(!isOpened){
        float time = 1.8;
        this->scheduleOnce(schedule_selector(MinimapView::showFirstOpenGuide), time);
    }
}

void MinimapView::showFirstOpenGuide(float _time){
    GuideController::share()->setGuide(FIRST_IN_MINI_MAP_GUIDE);
}

void MinimapView::hideResource(){
    m_hideIcon->setVisible(true);
    m_resourceIntroPointer->removeAllChildren();
    m_resourceIntroText->removeAllChildren();
    m_resBeltBatchNode->removeAllChildren();
}

void MinimapView::showSelfCity(){
    if(m_currentServerId == GlobalData::shared()->playerInfo.selfServerId){
        addCity(m_mapInfo->selfPt, 0);
    }
    
    auto disPoint = ccpSub(WorldMapView::instance()->m_map->currentTilePoint, WorldMapView::instance()->m_map->centerTilePoint);
    CCPoint worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
    auto currentPt = getPointByWorldPoint(worldPt);
    auto size = CCDirector::sharedDirector()->getWinSize();
    m_viewPort->scrollTo(-currentPt + ccp(size.width / 2, size.height / 2), NULL);
    
    m_phoneIcon = CCNode::create();
    CCBLoadFile("UIMAPArrow", m_phoneIcon, NULL);
    m_container->addChild(m_phoneIcon, 100);
    m_phoneIcon->setPosition(currentPt);
    
    auto finalContainerPt = m_container->getPosition();
    float s = 6.0f;
    float time = 0.4f;
    float sEnd = 1.0f;
    if (CCCommonUtils::isIosAndroidPad()) {
        sEnd = 1.0f * MINIMAP_HD_SCALE;
        s = s * MINIMAP_HD_SCALE;
    }
    m_container->setScale(s);
    auto containerPt = (m_container->getPosition() - size / 2) * s + size / 2;
    m_container->setPosition(containerPt);
    CCSpawn *spawn =CCSpawn::createWithTwoActions(CCMoveTo::create(time, finalContainerPt), CCScaleTo::create(time, sEnd));
    m_container->runAction(CCSequence::create(spawn,
                                                CCDelayTime::create(0.2),
                                                CCCallFunc::create(this, callfunc_selector(MinimapView::scaleBack)),
                                                NULL));
}
void MinimapView::endShowLegendEffect(){
    CCNode *legendNode = this->getChildByTag(Legend_Node_Tag);
    if(!legendNode)
        return;
    legendNode->stopAllActions();
    legendNode->setVisible(false);
    auto& arr = legendNode->getChildren();
    for (auto child : arr)
    {
        CCNode *node = dynamic_cast<CCNode*>(child);
        if(node){
            node->stopAllActions();
            if(dynamic_cast<CCRGBAProtocol*>(child)){
                dynamic_cast<CCRGBAProtocol*>(child)->setOpacity(255);
            }else{
                node->setVisible(true);
            }
        }
    }
}
void MinimapView::showLegendEffect(){
    CCNode *legendNode = this->getChildByTag(Legend_Node_Tag);
    if(!legendNode)
        return;
    legendNode->stopAllActions();
    CCObject *obj = NULL;
    auto& cArray = legendNode->getChildren();
    if(isAllShow){
        legendNode->setVisible(true);
        for (auto child : cArray)
        {
            CCNode *node = dynamic_cast<CCNode*>(child);
            if(node){
                node->stopAllActions();
                if(dynamic_cast<CCRGBAProtocol*>(obj)){
                    CCSequence* sequence = CCSequence::create(CCFadeIn::create(0.5),CCDelayTime::create(5),CCFadeOut::create(0.5),NULL);
                    node->runAction(sequence);
                }else{
                    CCSequence* sequence = CCSequence::create(CCShow::create(),CCDelayTime::create(6),CCHide::create(),NULL);
                    node->runAction(sequence);
                }
            }
        }
        CCSequence* sequence = CCSequence::create(CCDelayTime::create(6),CCCallFunc::create(this, callfunc_selector(MinimapView::endShowLegendEffect)),NULL);
        legendNode->runAction(sequence);
    }else{
        if(legendNode->isVisible()){
            for (auto child : cArray)
            {
                CCNode *node = dynamic_cast<CCNode*>(child);
                if(node){
                    CCNode *node = dynamic_cast<CCNode*>(obj);
                    node->stopAllActions();
                    if(dynamic_cast<CCRGBAProtocol*>(obj)){
                        if(dynamic_cast<CCRGBAProtocol*>(obj)->getOpacity()>0){
                            CCSequence* sequence = CCSequence::create(CCFadeOut::create(0.2),NULL);
                            node->runAction(sequence);
                        }
                    }
                }
            }
        }
        CCSequence* sequence = CCSequence::create(CCDelayTime::create(0.2),CCHide::create(),NULL);
        legendNode->runAction(sequence);
    }
}
void MinimapView::setAllianceMemeberVisible(bool b){
    auto& arr = m_cityBatchNode->getChildren();
//    if(arr){
//        return;
//    }
    int center = WorldController::getInstance()->getIndexByPoint(ccp(_relic_center_x, _relic_center_y));
    int index = 0;
    for (auto child : arr)
    {
        auto sprite = dynamic_cast<CCSprite*>(child);
        if(sprite){
            if(b){
                sprite->setVisible(b);
            }else{
                int tag = sprite->getTag();
                if(tag != m_mapInfo->selfPt && tag != m_mapInfo->chiefPt && tag != center){
                    sprite->setVisible(b);
                }
            }
        }
    }
//    while (index < arr->count()) {
//        auto sprite = dynamic_cast<CCSprite*>(arr->objectAtIndex(index++));
//        if(sprite){
//            if(b){
//                sprite->setVisible(b);
//            }else{
//                int tag = sprite->getTag();
//                if(tag != m_mapInfo->selfPt && tag != m_mapInfo->chiefPt && tag != center){
//                    sprite->setVisible(b);
//                }
//            }
//        }
//    }
    m_alianceHideIcon->setVisible(!isAllShow);
    showLegendEffect();
}

void MinimapView::onMemberDataBack(CCObject *obj){
    auto dict = _dict(obj);
    if(GlobalData::shared()->playerInfo.allianceInfo.leaderUid == GlobalData::shared()->playerInfo.uid){
        m_mapInfo->chiefPt = m_mapInfo->selfPt;
    }
    if(!dict->objectForKey("pointId")){
        m_mapInfo->chiefPt = -1;
    }else{
        int chiefPt = dict->valueForKey("pointId")->intValue();
        m_mapInfo->chiefPt = chiefPt;
        if(m_mapInfo->chiefPt == 0){
            m_mapInfo->chiefPt = -1;
        }else{
            if(m_mapInfo->chiefPt != m_mapInfo->selfPt){
                m_mapInfo->m_members[chiefPt] = 1;
            }
        }
        if(dict->objectForKey("memberPointId")){
            CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey("memberPointId"));
            CCObject *obj;
            CCARRAY_FOREACH(arr, obj){
                auto ptDict = _dict(obj);
                int pt = ptDict->valueForKey("pointId")->intValue();
                if(pt <= 0 || pt > WorldController::getInstance()->_current_tile_count_x * WorldController::getInstance()->_current_tile_count_y){
                    continue;
                }
                m_mapInfo->m_members[pt] = 1;
            }
        }
        if (dict->objectForKey("territoryPointId")) {
            CCArray *array = dynamic_cast<CCArray*>(dict->objectForKey("territoryPointId"));
            CCObject *object;
            CCARRAY_FOREACH(array, object){
                auto tPtDict = _dict(object);
                int tPt = tPtDict->valueForKey("pointId")->intValue();
                if(tPt <= 0 || tPt > WorldController::getInstance()->_current_tile_count_x * WorldController::getInstance()->_current_tile_count_y){
                    continue;
                }
                m_mapInfo->m_members[tPt] = 2;
            }
        }
    }
    showAllianceMember();
}

CCPoint MinimapView::getPointByWorldPoint(CCPoint &pt){
    CCPoint realPtSub = pt - WorldMapView::instance()->m_map->centerViewPoint;
    int scale = MINIMAP_SCALE;
    if (CCCommonUtils::isIosAndroidPad()) {
        scale = MINIMAP_HD_SCALE;
    }
    CCPoint miniPtSub = realPtSub * 1920 * scale * 1.0f / _map_width;
    CCPoint miniPt = getRealPosByPoint(viewCenterPoint) + miniPtSub;
    return miniPt;
}

CCPoint MinimapView::getWorldPointByPoint(const CCPoint &pt){
    CCPoint miniPtSub = pt - getRealPosByPoint(viewCenterPoint);
    int scale = MINIMAP_SCALE;
    if (CCCommonUtils::isIosAndroidPad()) {
        scale = MINIMAP_HD_SCALE;
    }
    CCPoint realPtSub = miniPtSub * _map_width * 1.0f / 1920 * scale;
    CCPoint realPt = WorldMapView::instance()->m_map->centerViewPoint + realPtSub;
    CCPoint indexPt = WorldMapView::instance()->m_map->getTileMapPointByViewPoint(realPt);
    return indexPt;
}

void MinimapView::showAllianceMember(){
    for (auto it = m_mapInfo->m_members.begin(); it != m_mapInfo->m_members.end(); it++) {
        int territoryFlag = it->second;
        int index = it->first;
        int type = 2;
        if(index == m_mapInfo->selfPt && territoryFlag == 1){
            type = 0;
        }else if (index == m_mapInfo->chiefPt && territoryFlag == 1){
            type = 1;
        }else if (territoryFlag == 2){
            type = 4;
        }
        addCity(index, type);
    }
}

void MinimapView::update(float time){
    if(GlobalData::shared()->playerInfo.isInSelfServer()){
        WorldActivityState state = WorldController::getInstance()->getKingActivityStateByType(WorldActivityType::FIGHT_OF_KING);
        if(state == NotOpen){
            return;
        }
        if(!m_timeText){
            return;
        }
        auto it = WorldController::getInstance()->m_worldActivity.find(FIGHT_OF_KING);
        if(it == WorldController::getInstance()->m_worldActivity.end()){
            return;
        }
        WorldActivityInfo &activityInfo = it->second;
        auto now = WorldController::getInstance()->getTime();
        double time = 0;
        double startTime = activityInfo.timeInfo.startTime;
        double endTime = activityInfo.timeInfo.endTime;
        if(state == OpenNoKing){
            time = startTime - now;
        }else if(state == PeaceTime){
            time = startTime - now;
        }else if(state == WarTime){
            time = endTime - now;
        }
        time = time / 1000;
        if(time < 0){
            time = 0;
        }
        m_timeText->setString(CCCommonUtils::timeLeftToCountDown(time));
    }
}

void MinimapView::addCity(int index, int type){//0 self 1 chief 2 member 3 relic 4 territory
    // 添加图片
    std::string iconPic = "position_city.png";
    int zOrder = index;
    float s = 0.5;
    if(type == 0){
        zOrder = WorldController::getInstance()->_current_tile_count_x * WorldController::getInstance()->_current_tile_count_y + 3;
    }else if(type == 1){
        iconPic = "position_lord.png";
        zOrder = WorldController::getInstance()->_current_tile_count_x * WorldController::getInstance()->_current_tile_count_y + 1;
    }else if(type == 2){
        iconPic = "position_alli.png";
    }else if(type == 3){
        iconPic = "relic_city.png";
        s = 1.0f;
    }else if(type == 4){
        iconPic = "position_territory.png";
        zOrder = WorldController::getInstance()->_current_tile_count_x * WorldController::getInstance()->_current_tile_count_y + 2;
    }
    auto sprite = CCLoadSprite::createSprite(iconPic.c_str());
    m_cityBatchNode->addChild(sprite);
    sprite->setTag(index);
    sprite->setZOrder(zOrder);
    CCPoint ptByIndex = WorldController::getPointByIndex(index);
    auto disPoint = ccpSub(ptByIndex, WorldMapView::instance()->m_map->centerTilePoint);
    CCPoint worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
//    auto worldPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(index));
    auto cityPt = getPointByWorldPoint(worldPt);
    auto spritePt = cityPt + ccp(0, sprite->getContentSize().height / 2 * s);
    sprite->setPosition(spritePt);
    if(type == 3){
        sprite->setPosition(cityPt);
    }
    sprite->setScale(s);
    if(0 == index)
    {
        sprite->setVisible(false);
    }
    else
    {
        sprite->setVisible(true);
    }
}
void MinimapView::resetSettingNode(CCObject* ccObj){
    m_InAction = false;
    m_settingNode->stopAllActions();
    int sIndex = 1;
    int eIndex = 9;
    CCScale9Sprite* bg = dynamic_cast<CCScale9Sprite*>(m_settingNode->getChildByTag(1));
    bool isShow = bg->getOpacity()>0?true:false;
    while (sIndex<eIndex) {
        CCNode *child = dynamic_cast<CCNode*>(m_settingNode->getChildByTag(sIndex));
        if(child /*&& child->isVisible()*/){
            child->stopAllActions();
            
        }
        sIndex++;
    }
    m_settingNode->setVisible(isShow);
}
void MinimapView::showSettingNode(){
    if(m_InAction == true){
        //resetSettingNode();
        return;
    }
    m_InAction = true;
    CCScale9Sprite *bgScale = dynamic_cast<CCScale9Sprite*>(m_settingNode->getChildByTag(1));
    if(!bgScale)
        return;
    
    CCSpriteFrame * frame;
    if(bgScale->getOpacity() > 0){
        float showTime = 0.2;
        float easeTime = 0.2;
        float showScale = 0;
        frame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("d-biyan.png");
        
        CCScaleTo *scaleTo = CCScaleTo::create(easeTime, 1.0, showScale);
        m_settingNode->runAction(CCSequence::create(scaleTo,NULL));
//        CCActionInterval* move_ease_out = CCEaseBackOut::create((CCActionInterval*)(scaleTo->copy()->autorelease()));
        int sIndex = 2;
        int eIndex = 9;
        while (sIndex<eIndex) {
            CCNode *child = dynamic_cast<CCNode*>(m_settingNode->getChildByTag(sIndex));
            if(child /*&& child->isVisible()*/){
                child->runAction(CCSequence::create(CCFadeOut::create(showTime),NULL));
            }
            sIndex++;
        }
        bgScale->runAction(CCSequence::create(CCFadeOut::create(showTime),CCCallFuncO::create(this, callfuncO_selector(MinimapView::resetSettingNode),NULL),NULL));
    }else{
        float showTime = 0.1;
        float easeTime = 0.3;
        float showScale = 0.7;
        
        m_settingNode->setScaleY(showScale);
        m_settingNode->setVisible(true);
        bgScale->runAction(CCSequence::create(CCFadeIn::create(showTime),NULL));
        
        int sIndex = 2;
        int eIndex = 9;
        while (sIndex<eIndex) {
            CCNode *child = dynamic_cast<CCNode*>(m_settingNode->getChildByTag(sIndex));
            if(child /*&& child->isVisible()*/){
                child->runAction(CCSequence::create(CCFadeIn::create(showTime),NULL));
            }
            sIndex++;
        }
        CCDelayTime *delayTime = CCDelayTime::create(showTime-0.02);
        CCScaleTo *scaleTo = CCScaleTo::create(easeTime, 1.0, 1.0);
        CCActionInterval* move_ease_out = CCEaseBackOut::create((CCActionInterval*)(scaleTo->clone()));
        m_settingNode->runAction(CCSequence::create(delayTime,move_ease_out,CCCallFuncO::create(this, callfuncO_selector(MinimapView::resetSettingNode),NULL),NULL));
        
        frame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("d-zhengyan.png");
    }
    if(frame){
        m_settingBtn->setDisplayFrame(frame);
    }
}

void MinimapView::onShowMarkLine(CCObject* obj)
{
    m_markNode->removeAllChildren();
    m_markTouchNode->setContentSize(CCSize(0, 0));
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        int rallyPoint = GlobalData::shared()->playerInfo.allianceInfo.rallyPoint;
        if (rallyPoint != 0)
        {
            m_markNode->addChild(m_markTouchNode);
            m_markTouchNode->setContentSize(CCSize(60, 150));
            
            std::string iconPic = "mark_flag.png";
            float s = 0.8;
            auto sprite = CCLoadSprite::createSprite(iconPic.c_str());
            sprite->setAnchorPoint(ccp(0.5, 0));
            m_markNode->addChild(sprite);
            CCPoint ptByIndex = WorldController::getPointByIndex(rallyPoint);
            auto disPoint = ccpSub(ptByIndex, WorldMapView::instance()->m_map->centerTilePoint);
            CCPoint worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
            auto cityPt = getPointByWorldPoint(worldPt);
            sprite->setPosition(cityPt);
            sprite->setScale(s);
            
            m_markTouchNode->setPosition(cityPt+ccp(-30, 0));
            
            auto tmpNode = CCNode::create();
            CCBLoadFile("UIMAPArrow", tmpNode, NULL);
            m_markNode->addChild(tmpNode);
            tmpNode->setPosition(cityPt);
        }
    }
}

void MinimapView::onGoToMarkPt(const cocos2d::CCPoint& worldLocation)
{
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        int rallyPoint = GlobalData::shared()->playerInfo.allianceInfo.rallyPoint;
        if (rallyPoint != 0)
        {
            CCPoint ptByIndex = WorldController::getPointByIndex(rallyPoint);
            if(WorldController::getInstance()->isInMap(ptByIndex)){
                GlobalData::shared()->playerInfo.currentServerId = m_currentServerId;
                if (!GlobalData::shared()->playerInfo.isInSelfServer()) {
                    WorldMapView::instance()->removeCover();
                    WorldMapView::instance()->addCover();
                }
                else
                    WorldMapView::instance()->removeCover();
                
                WorldMapView::instance()->gotoTilePoint(ptByIndex,m_currentServerId);
                singleTouchClose = true;
                auto size = CCDirector::sharedDirector()->getWinSize();
                if (!m_phoneIcon->isVisible()) {
                    m_phoneIcon->setVisible(true);
                }
                m_phoneIcon->setPosition(worldLocation);
                m_viewPort->scrollTo(-worldLocation + ccp(size.width / 2, size.height / 2), CCCallFunc::create(this, callfunc_selector(MinimapView::moveBack)), true);
            }
            
        }
    }
}