//
//  TestFeedBackView.cpp
//  IF
//
//  Created by chenfubi on 15/2/9.
//
//

#include "TestFeedBackView.h"
#include "PopupViewController.h"
#include "TestFeedBackController.h"
//#include "TestFeedBackCommand.h"
#include "GameController.h"

#define TESTFEEDBACK_EDIT "TESTFEEDBACK_EDIT"
#define TestEditView_CLOSE_EVENT "TestEditView.CLOSE.EVENT"

TestFeedBackView* TestFeedBackView::create()
{
    TestFeedBackView* ret = new TestFeedBackView();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TestFeedBackView::init()
{
	bool bRet = false;
	do
	{
        if (!PopupBaseView::init())
        {
            break;
        }
        CCLoadSprite::doResourceByCommonIndex(306, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(306, false);
        });
        auto ccb = CCBLoadFile("TestFeedBackView-1", this, this);
        this->setContentSize(ccb->getContentSize());
        int extH = getExtendHeight();
//        float totalH = extH + 500;
//        CCSprite *segmentSpr = CCLoadSprite::createSprite("BG_equip_middle.png");
//        CCSize segmentSize = segmentSpr->getContentSize();
//        float tempW = CCDirector::sharedDirector()->getWinSize().width;
//        float fScale = tempW * 0.5 / segmentSize.width;
//        int row = ceil(totalH / (segmentSize.height * fScale));
//        int col = 2;
//        int rIndex = 0;
//        int cIndex = 0;
//        m_midBGNode->removeAllChildren();
//        CCSpriteBatchNode* node = CCSpriteBatchNode::createWithTexture(segmentSpr->getTexture(), row * col);
//        while (rIndex<row) {
//            float tmpY = segmentSize.height*fScale * rIndex;
//            while (cIndex<col) {
//                CCSprite* spr = CCLoadSprite::createSprite("BG_equip_middle.png");
//                spr->setScale(fScale);
//                float tmpX = 0;
//                bool flipx = false;
//                if(cIndex == 0){
//                    tmpX = -tempW * 0.25;
//                    flipx = false;
//                }else{
//                    tmpX = tempW * 0.25;
//                    flipx = true;
//                }
//                node->addChild(spr);
//                spr->setScale(fScale);
//                spr->setFlipX(flipx);
//                spr->setPosition(ccp(tmpX,-tmpY));
//                ++cIndex;
//            }
//            cIndex = 0;
//            rIndex++;
//        }
//        m_midBGNode->addChild(node);
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        m_bg1->setPreferredSize(CCSize(winSize.width, getBGHeight() + 20));
        m_bg1->setPositionY(m_bg1->getPositionY() + 20);
        m_bg2->setPositionY(m_bg2->getPositionY() - extH);
        CCSize size = m_listNode->getContentSize();
        size.height = size.height + extH;
        m_listNode->setContentSize(size);
        m_viewNode->setPositionY(m_viewNode->getPositionY() + extH/2);
        m_listNode->setPositionY(m_listNode->getPositionY() - extH);
        
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - extH);
        
        m_titleTTF->setString(_lang("105202"));
        m_infoTTF->setString(_lang("135602"));
        
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("101274").c_str());
        
//        TESTFEEDBACK_CONTROL->test();//todocfb
        m_serverLabel->setString(_lang("105047"));
        m_rawLabel->setString(_lang("105046"));
        
        m_editBoxX = MAX(m_serverLabel->getContentSize().width, m_rawLabel->getContentSize().width) + m_infoTTF->getPositionX();
        m_serverLabel->setPositionX(m_editBoxX);
        m_rawLabel->setPositionX(m_editBoxX);
        m_editBoxX+=10;
        if (TESTFEEDBACK_CONTROL->getHasInit())
        {
            initData(NULL);
        }
        else
        {
            m_waitInterFace = GameController::getInstance()->showWaitInterface(m_viewNode);
            CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TestFeedBackView::initData), TESTFEEDBACK_OPEN, NULL);
        }
        
		bRet = true;
	}while(0);
	return bRet;
}

void TestFeedBackView::initData(CCObject* pObj)
{
    if (m_waitInterFace)
    {
        m_waitInterFace->removeFromParent();
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TESTFEEDBACK_OPEN);
    m_data = TESTFEEDBACK_CONTROL->getSuggestArr();
    
    string serverId = TESTFEEDBACK_CONTROL->getServerId();
    string name = TESTFEEDBACK_CONTROL->getName();
    
    ccColor3B color1;
    ccColor3B color2;
    if(TESTFEEDBACK_CONTROL->getName().empty()){
        color1 = {130, 130, 130};
        color2 = {130, 130, 130};
    }else{
        color1 = ccBLACK;
        color2 = ccBLACK;
    }
    
    auto sprite9 = CCLoadSprite::createScale9Sprite("hospital_text_bg.png");
    m_serverEditBox = CCEditBox::create(CCSizeMake(320,50), sprite9);
    m_serverEditBox->setFont("Helevtica",24);
    m_serverEditBox->setPosition(ccp(160, 25));
    m_serverEditBox->setPlaceholderFontColor(color1);
    m_serverEditBox->setFontColor(color1);
    m_serverEditBox->setPlaceholderFont("Helevtica",24);
    m_serverEditBox->setPlaceHolder(serverId.c_str());
    m_serverEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_serverEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_serverEditBox->setInputMode(kEditBoxInputModeNumeric);
    m_serverEditBox->setMaxLength(10);
    m_serverEditBox->setDelegate(this);
//    m_serverEditBox->setTouchPriority(Touch_Default);
    m_serverNode->addChild(m_serverEditBox);
    m_serverNode->setPositionX(m_editBoxX);
    
    auto sprite10 = CCLoadSprite::createScale9Sprite("hospital_text_bg.png");
    m_rawEditBox = CCEditBox::create(CCSizeMake(320,50), sprite10);
    m_rawEditBox->setFont("Helevtica",24);
    m_rawEditBox->setPosition(ccp(160, 25));
    m_rawEditBox->setPlaceholderFont("Helevtica",24);
    m_rawEditBox->setPlaceholderFontColor(color2);
    m_rawEditBox->setFontColor(color2);
    if(!name.empty()){
        m_rawEditBox->setPlaceHolder(name.c_str());
    }else{
        m_rawEditBox->setPlaceHolder(_lang("105205").c_str());
    }
    m_rawEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_rawEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_rawEditBox->setMaxLength(20);
    m_rawEditBox->setDelegate(this);
//    m_rawEditBox->setTouchPriority(Touch_Default);
    m_rawNode->addChild(m_rawEditBox);
    m_rawNode->setPositionX(m_editBoxX);
    
    m_tabView = CCTableView::create(this, m_listNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_listNode->addChild(m_tabView);
    m_tabView->setTouchEnabled(m_tabView->getContentSize().height>m_listNode->getContentSize().height);
    CCLOG("TestFeedBackView-m_tabView:%f", m_tabView->getContentSize().height);
}
void TestFeedBackView::onCloseBtnClick(CCObject* pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
}
void TestFeedBackView::onOkClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
    if (TESTFEEDBACK_CONTROL->getServerId().length() == 0 || TESTFEEDBACK_CONTROL->getName().length() == 0 ||  TESTFEEDBACK_CONTROL->getServerId() == "0")
    {
        CCCommonUtils::flyHint("", "", "请输入服务器号和名字");//todocfb
        return;
    }
    TESTFEEDBACK_CONTROL->sendCmd();
    PopupViewController::getInstance()->removePopupView(this);
}

void TestFeedBackView::editBoxEditingDidBegin(CCEditBox *editBox)
{
    if (editBox){
        editBox->setFontColor(ccBLACK);
        editBox->setPlaceholderFontColor(ccBLACK);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TESTFEEDBACK_EDIT, ccs("0"));
}

void TestFeedBackView::editBoxEditingDidEnd(CCEditBox *editBox)
{
    CCLOGFUNCF("editBoxEditingDidEnd");
    this->scheduleOnce(schedule_selector(TestFeedBackView::onGameTick), 0.5f);
    
}

void TestFeedBackView::editBoxTextChanged(CCEditBox *editBox, const std::string &text)
{
    
}

void TestFeedBackView::editBoxReturn(CCEditBox *editBox)
{
    std::string content = editBox->getText();
    if (editBox == m_serverEditBox)
    {
        if(content.empty()){
            if(!TESTFEEDBACK_CONTROL->getServerId().empty()){
                content = TESTFEEDBACK_CONTROL->getServerId();
            }else{
                m_serverEditBox->setFontColor(ccc3(130, 130, 130));
            }
        }else{
            TESTFEEDBACK_CONTROL->setServerId(content);
        }
        editBox->setText(content.c_str());
    }
    if (editBox == m_rawEditBox)
    {
        if(content.empty()){
            if(!TESTFEEDBACK_CONTROL->getName().empty()){
                content =TESTFEEDBACK_CONTROL->getName();
            }else{
                 m_serverEditBox->setFontColor(ccc3(130, 130, 130));
            }
        }else{
            TESTFEEDBACK_CONTROL->setName(content);
        }
        editBox->setText(content.c_str());
    }
    CCLOGFUNCF("editBoxReturn-serverid:%s,name:%s", m_serverEditBox->getText(), m_rawEditBox->getText());
}

void TestFeedBackView::onGameTick(float delay)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TESTFEEDBACK_EDIT, ccs("1"));
}

void TestFeedBackView::dataChange(cocos2d::CCObject *pObj)
{
    CCDictionary* dict = _dict(pObj);
    int index = dict->valueForKey("index")->intValue();
    CCObject* obj = NULL;
    CCDictionary* pelem = NULL;
    CCARRAY_FOREACH(m_data, obj)
    {
        pelem = (CCDictionary*)obj;
        if (pelem->valueForKey("index")->intValue() == index)
        {
            pelem->setObject(dict->objectForKey("suggestion"), "suggestion");
            break;
        }
    }
}

void TestFeedBackView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TestFeedBackView::dataChange), TESTFEEDBACK_DATACHANGE, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void TestFeedBackView::onExit()
{
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TESTFEEDBACK_DATACHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TESTFEEDBACK_OPEN);
    CCNode::onExit();
}

bool TestFeedBackView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void TestFeedBackView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

CCSize TestFeedBackView::tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return CCSizeZero;
    }
    CCSize size = CCSizeMake(580, 120);
    CCDictionary* dict = (CCDictionary*)m_data->objectAtIndex(idx);
    float height = getTextDimension(dict->valueForKey("content")->getCString()).height;
    height += 80;
    size.height = height;
    CCLOG("s: [%d - %s]",idx,dict->valueForKey("content")->getCString());
    return size;
}

CCSize TestFeedBackView::cellSizeForTable(  cocos2d::extension::TableView *table)
{
    return CCSize(580, 120);
}

CCSize TestFeedBackView::getTextDimension(const char *text)
{
    int len = (int)strlen(text);
    if ((len==0)  ||  (len==1 && text[0] == '\n')) {
        CCLabelIF *label = CCLabelIF::create("sample", "Helvetica", 24);
        label->setDimensions(CCSizeMake(580, 0));
        CCSize size = label->getContentSize();
        size.width = 0;
        return size;
    }
    CCLabelIF *label = CCLabelIF::create(text, "Helvetica", 24);
    label->setDimensions(CCSizeMake(580, 0));
    return label->getContentSize();
}

CCTableViewCell* TestFeedBackView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return NULL;
    }
    TestFeedBackCell* cell = (TestFeedBackCell*)table->dequeueCell();
    CCDictionary* dict = (CCDictionary*)m_data->objectAtIndex(idx);
    CCLOG("t: [%d - %s]",idx,dict->valueForKey("content")->getCString());
    if (cell)
    {
        cell->setData(dict);
    }
    else
    {
        cell = TestFeedBackCell::create(dict,m_listNode);
    }
//    if(idx == (m_data->count()-1)){
//        cell->m_sprLine->setVisible(false);
//    }else{
//        cell->m_sprLine->setVisible(true);
//    }
    return cell;
}

ssize_t TestFeedBackView::numberOfCellsInTableView(  cocos2d::extension::TableView *table)
{
    return m_data->count();
}

bool TestFeedBackView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_serverLabel", CCLabelIF*, this->m_serverLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rawLabel", CCLabelIF*, this->m_rawLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewNode", CCNode*, this->m_viewNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_midNode", CCNode*, this->m_midNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_serverNode", CCNode*, this->m_serverNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rawNode", CCNode*, this->m_rawNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTTF", CCLabelIF*, this->m_titleTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoTTF", CCLabelIF*, this->m_infoTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg1", CCScale9Sprite*, this->m_bg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg2", CCScale9Sprite*, this->m_bg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_okBtn", CCControlButton*, this->m_okBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_midBGNode", CCNode*, this->m_midBGNode);
    return false;
}

SEL_CCControlHandler TestFeedBackView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkClick", TestFeedBackView::onOkClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", TestFeedBackView::onCloseBtnClick);
    return NULL;
}


//class TestFeedBackCell

TestFeedBackCell* TestFeedBackCell::create(CCDictionary *dict,CCNode* pNode)
{
    TestFeedBackCell* ret = new TestFeedBackCell();
    if (ret && ret->init(dict,pNode))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TestFeedBackCell::init(CCDictionary *dict,CCNode* pNode)
{
    auto ccb = CCBLoadFile("TestFeedBackCell", this, this);
//    setContentSize(ccb->getContentSize());
    m_parentNode = pNode;
    setData(dict);
    return true;
}
void TestFeedBackCell::isEdit(bool isEdit){
    if(isEdit){
        CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Mail_frame10.png");
        m_sprBG->setSpriteFrame(pFrame);
    }else{
        CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Mail_frame009.png");
        m_sprBG->setSpriteFrame(pFrame);
    }
    m_sprBG->setPreferredSize(getContentSize());
}
void TestFeedBackCell::setData(CCDictionary *dict)
{
    m_index = dict->valueForKey("index")->intValue();
    m_content = dict->valueForKey("content")->getCString();
    m_suggestion = dict->valueForKey("suggestion")->getCString();
    m_infoTTF1->setString(m_content);
    
    ccColor3B color = {163, 34, 3};
    string info2 = _lang("135600");
    if (m_suggestion.length() == 0)
    {
        info2 = _lang("135601");
        color = {0, 36, 217};
    }
    m_infoTTF2->setString(info2);
    m_infoTTF2->setColor(color);
    
    float tmpH = m_infoTTF1->getContentSize().height;
    tmpH += 70;
    m_infoTTF1->setPositionY(tmpH-10);
    setContentSize(CCSize(m_touchNode->getContentSize().width,tmpH));
    m_touchNode->setContentSize(CCSize(m_touchNode->getContentSize().width, tmpH));
    m_sprBG->setPreferredSize(getContentSize());
    CCLOG( "[%s]",m_content.c_str());
}

void TestFeedBackCell::notifyBack(cocos2d::CCObject *pObj)
{
    CCString* s = dynamic_cast<CCString*>(pObj);
    if (s)
    {
        m_canTouch = (s->intValue()==1);
    }
}
void TestFeedBackCell::dataChangeBG(CCObject* pObj){
    if(pObj){
        CCInteger *integer = dynamic_cast<CCInteger*>(pObj);
        if(integer && integer->getValue()>0){
            int index = integer->getValue();
            if (m_index == index){
                isEdit(false);
            }
        }
    }
}
void TestFeedBackCell::dataChange(cocos2d::CCObject *pObj)
{
    CCDictionary* dict = _dict(pObj);
    int index = dict->valueForKey("index")->intValue();
    if (m_index == index){
        m_suggestion = dict->valueForKey("suggestion")->getCString();
        ccColor3B color = {163, 34, 3};
        string info2 = _lang("135600");
        if (m_suggestion.length() == 0)
        {
            info2 = _lang("135601");
            color = {0, 36, 217};
        }
        m_infoTTF2->setColor(color);
        m_infoTTF2->setString(info2);
    }
}

void TestFeedBackCell::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TestFeedBackCell::notifyBack), TESTFEEDBACK_EDIT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TestFeedBackCell::dataChange), TESTFEEDBACK_DATACHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TestFeedBackCell::dataChangeBG), TestEditView_CLOSE_EVENT, NULL);
    
}

void TestFeedBackCell::onExit()
{
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TESTFEEDBACK_EDIT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TESTFEEDBACK_DATACHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TestEditView_CLOSE_EVENT);
    CCNode::onExit();
}

bool TestFeedBackCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCLOGFUNCF("ccTouchBegan");
    if (isTouchInside(m_parentNode,pTouch) && isTouchInside(m_touchNode, pTouch) && m_canTouch)
    {
        return true;
    }
    return false;
}

void TestFeedBackCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) > 10)
    {
        return;
    }
    isEdit(true);
    CCLOGFUNCF("onTouchTestFeedBackCell");
    PopupViewController::getInstance()->addPopupView(TestEditView::create(m_index, m_suggestion));
}

bool TestFeedBackCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoTTF1", CCLabelIF*, this->m_infoTTF1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoTTF2", CCLabelIF*, this->m_infoTTF2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sprBG", CCScale9Sprite*, this->m_sprBG);
    return false;
}


//class TestEditView

TestEditView* TestEditView::create(int index, string suggestion)
{
    TestEditView* ret = new TestEditView();
    if (ret && ret->init(index, suggestion))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TestEditView::init(int index, string suggestion)
{
    if (!PopupBaseView::init())
    {
        return false;
    }
    auto ccb = CCBLoadFile("TestEditView", this, this);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(winSize);
//    int extH = getExtendHeight();
//    m_bottomNode->setPositionY(m_bottomNode->getPositionY() - extH/2.0f);
    CCCommonUtils::setButtonTitle(m_okBtn, _lang("101274").c_str());
    
    m_index = index;
    m_oldSuggerstion = m_suggestion = suggestion;
    string defaultStr = _lang("105039");
    if (m_suggestion.length() != 0)
    {
        defaultStr = m_suggestion;
    }
    
    m_editBox = InputFieldMultiLine::create(CCSizeMake(490,40), "UI_chat_text.png", 20);
    m_editBox->setMaxChars(500);
    m_editBox->setShowLineNum(12);
    m_editBox->setLineNumber(2);
    m_editBox->setFontColor(ccBLACK);
    m_editBox->setPosition(ccp(0,0));
    m_editBox->setSwallowsTouches(true);
    m_editBox->setMoveFlag(true);
    m_editBox->setTouchPriority(0);
    m_editBox->setPlaceHolder(defaultStr.c_str());
    m_infoNode->addChild(m_editBox);
    if (m_suggestion.length() != 0)
    {
        m_editBox->setText(m_suggestion.c_str());
//        CCRect rect = m_editBox->getViewRect();
//        int changHeight = rect.size.height - 40;
//        onAddlineEvent(CCInteger::create(changHeight));
//        CCLOGFUNCF("%f,%d", rect.size.height, changHeight);
    }
    
    return true;
}

void TestEditView::onAddlineEvent(CCObject * params)
{
    int changeHeight = dynamic_cast<CCInteger*>(params)->getValue();
    m_infoNode->setContentSize(CCSizeMake(m_infoNode->getContentSize().width, m_infoNode->getContentSize().height+changeHeight));
    CCLOGFUNCF("%f,%f",m_infoNode->getContentSize().height, m_bg->getContentSize().height);
    if (m_infoNode->getContentSize().height > m_bg->getContentSize().height)
    {
        m_infoNode->setPositionY(m_infoNode->getPositionY()+changeHeight/2);
        m_bg->setVisible(false);
    }
    else
    {
        m_bg->setVisible(true);
        m_infoNode->setPositionY(m_infoNode->getPositionY()-changeHeight/2);
    }
    
}

void TestEditView::onInputFieldCloseEvent(cocos2d::CCObject *params)
{
    m_suggestion = m_editBox->getText();
    CCLOGFUNCF("%s", m_suggestion.c_str());
}

void TestEditView::onOkClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void TestEditView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TestEditView::onAddlineEvent), MSG_INPUT_ADD_LINE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TestEditView::onInputFieldCloseEvent), INPUTFIELD_CLOSE_EVENT, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
}

void TestEditView::onExit()
{
    m_suggestion = m_editBox->getText();
    if (strcmp(m_oldSuggerstion.c_str(), m_suggestion.c_str()) != 0 && m_index != -1)
    {
        if (m_suggestion == "")
        {
            m_suggestion = m_oldSuggerstion;
        }
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(CCString::createWithFormat("%d", m_index), "index");
        dict->setObject(ccs(m_suggestion), "suggestion");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TESTFEEDBACK_DATACHANGE, dict);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TestEditView_CLOSE_EVENT, CCInteger::create(m_index));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INPUT_ADD_LINE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, INPUTFIELD_CLOSE_EVENT);
    setTouchEnabled(false);
    CCNode::onExit();
}

bool TestEditView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void TestEditView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    if (!isTouchInside(m_touchNode, pTouch) && ccpDistance(pTouch->getStartLocation(), pTouch->getLocation()) < 10)
    {
        PopupViewController::getInstance()->removePopupView(this);
    }
    if(isTouchInside(m_okBtn, pTouch)){
        onOkClick(m_okBtn, Control::EventType::TOUCH_DOWN);
        return;
    }
    if (isTouchInside(m_bg, pTouch))
    {
        m_editBox->closeIME();
        m_editBox->openIME();
    }
}

bool TestEditView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_okBtn", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCScale9Sprite*, this->m_bg);
    return false;
}

SEL_CCControlHandler TestEditView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkClick", TestEditView::onOkClick);
    return NULL;
}

