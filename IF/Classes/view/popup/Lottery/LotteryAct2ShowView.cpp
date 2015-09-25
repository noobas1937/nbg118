//
//  LotteryAct2ShowView.cpp
//  IF
//
//  Created by chenfubi on 15/2/6.
//
//

#include "LotteryAct2ShowView.h"
#include "LotteryController.h"
#include "PopupViewController.h"
#include "CCMathUtils.h"


LotteryAct2ShowView* LotteryAct2ShowView::create()
{
    LotteryAct2ShowView* ret = new LotteryAct2ShowView();
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

bool LotteryAct2ShowView::init()
{
	bool bRet = false;
	do
	{
        if (!PopupBaseView::init())
        {
            break;
        }
        setIsHDPanel(true);
        CCBLoadFile("LotteryAct2ShowView", this, this);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        int extH = getExtendHeight();
        m_midNode->setPositionY(m_midNode->getPositionY() + extH/2.0f);
        m_titleTTF->setString(_lang("111121"));
        initAniNode();
        
		bRet = true;
	}while(0);
	return bRet;
}

void LotteryAct2ShowView::initAniNode()
{
    CCNode* nodeB = NULL;
    CCNode* nodeC = NULL;
    CCNode* nodeC2 = NULL;
    CCNode* nodeC3 = NULL;
    CCNode* nodeC4 = NULL;
    CCSprite* bg = NULL;
    CCSprite* icon = NULL;
    CCSprite* iconBg = NULL;
//    CCSprite* iconBg2 = NULL;
    CCSprite* sIcon = NULL;
    CCLabelIF* numTTF = NULL;
    CCLabelIF* stepTTF = NULL;
    CCLabelIF* infoTTF = NULL;
    string itemId = "";
    string itemCnt = "1";
    int colorInt = 0;
    string color = "";
    vector<string> reward;
    
    std::string posArrayStr = CCUserDefault::sharedUserDefault()->getStringForKey(LotteryAct2ShowView_PREVIEW_REWARD);
    vector<string> posArray;
    bool localData =false;
    string savePosArrayStr = "";
    if(!posArrayStr.empty()){
        CCCommonUtils::splitString(posArrayStr, ",", posArray);
        localData = true;
    }else{
        int iTmp = 0;
        while(iTmp<9){
            posArray.push_back(CC_ITOA(iTmp));
            ++iTmp;
        }
        localData=false;
    }
//     CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0, "");
    std::string stepArrayStr = CCUserDefault::sharedUserDefault()->getStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0);
    vector<string> stepArray;
    string saveStepArrayStr = "";
    if(!stepArrayStr.empty()){
        CCCommonUtils::splitString(stepArrayStr, ",", stepArray);
    }
    if(stepArray.size()<9){
        stepArray.clear();
        int iTmp = 0;
        while(iTmp<9){
            stepArray.push_back("-1");
            ++iTmp;
        }
    }
    int index = 0;
    for (int i = 0; i < 9; i++)
    {
        int iChild  = 0;
        if(localData==false){
            int iIndex = CCMathUtils::getRandomInt(0, (posArray.size()-1)*100+99);
            iIndex = floor(iIndex/100);
            std::string indexStr = posArray.at(iIndex);
            iChild = atoi(indexStr.c_str());
            auto iter = posArray.begin();
            iter += iIndex;
            posArray.erase(iter);
            savePosArrayStr = savePosArrayStr.empty()?indexStr:savePosArrayStr+","+indexStr;
        }else{
            iChild =atoi(posArray.at(i).c_str());
        }
        reward.clear();
//        index = (m_itemIndex+i)%9;
        nodeB = m_aniListNode->getChildByTag(iChild);
        if (nodeB)
        {
            bg = (CCSprite*)nodeB->getChildByTag(3);
            nodeC = nodeB->getChildByTag(2);
            nodeC2 = nodeB->getChildByTag(1);
            nodeC3 = nodeB->getChildByTag(4);
            nodeC4 = nodeB->getChildByTag(5);
//            iconBg2 = (CCSprite*)nodeC2->getChildByTag(0);
            icon = (CCSprite*)nodeC2->getChildByTag(1);
            numTTF = (CCLabelIF*)nodeC2->getChildByTag(2);
            iconBg = (CCSprite*)nodeC2->getChildByTag(3);
            nodeC2->removeChildByTag(4);
            
            infoTTF = (CCLabelIF*)nodeC3->getChildByTag(1);
            stepTTF = (CCLabelIF*)nodeC4->getChildByTag(1);
            
            sIcon = (CCSprite*)nodeC->getChildByTag(0);
            
            LotteryController::shared()->getRewardInfo(i, reward);
            itemId = reward.at(0);
            itemCnt = reward.at(1);
            itemCnt = (itemCnt == "" || itemCnt == "0")?"1":itemCnt;
            nodeC2->setVisible(itemId.length());
            if (itemId.length() > 0)
            {
                icon->removeAllChildren();
                bg->setDisplayFrame(CCLoadSprite::loadResource("Lottery_Card1.png"));
                icon->setDisplayFrame(CCLoadSprite::loadResource(getIcon(itemId).c_str()));
                CCCommonUtils::setSpriteMaxSize(icon, 140);
                colorInt = atoi(getColor(itemId).c_str());
                color = CCCommonUtils::getToolBgByColor(colorInt);
                iconBg->setDisplayFrame(CCLoadSprite::loadResource(color.c_str()));
                numTTF->setString(itemCnt);
                int rewardstep = atoi(stepArray.at(i).c_str());
                
                if(rewardstep>-1){
                    vector<string> tmpV;
                    rewardstep = LotteryController::shared()->getStepByPos(rewardstep);
                }else{
                    vector<int> tmpIV;
                    if(LotteryController::shared()->getStep(itemId,itemCnt,tmpIV)){
                        stepArray[i] = CC_ITOA(tmpIV[0]);
                        rewardstep = tmpIV[1];
                    }
                }
                if(rewardstep>0){
                    nodeC3->setVisible(true);
                    nodeC4->setVisible(true);
                    infoTTF->setString(_lang("101312"));
                    std::string step = CC_ITOA(rewardstep);
                    stepTTF->setString(_lang_1("111130",step.c_str()));
                }
                int itemIdInt = atoi(itemId.c_str());
                if (200<=itemIdInt && itemIdInt<=205)
                {
                    CCLabelIFBMFont* sp = LotteryController::shared()->getLabelBMfont(itemIdInt);
                    sp->setScale(2);
                    icon->setVisible(false);
                    nodeC2->addChild(sp, 2, 4);
                }
                int index = atoi(getColor(itemId).c_str()) + 1;
                sIcon->setDisplayFrame(CCLoadSprite::loadResource(CCString::createWithFormat("Lottery_rating%d.png",index)->getCString()));
                LotteryController::addCountInfo(icon, itemId);
            }
        }
    }
    if(!savePosArrayStr.empty()){
        CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD, savePosArrayStr);
    }
    auto iter1 = stepArray.begin();
    saveStepArrayStr = (*iter1);
    iter1++;
    while (iter1!=stepArray.end()) {
        saveStepArrayStr = saveStepArrayStr + ","+(*iter1);
        ++iter1;
    }
    CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0, saveStepArrayStr);
    CCUserDefault::sharedUserDefault()->flush();
}

void LotteryAct2ShowView::setDesNode(int index)
{
    if (index <= -1)
    {
        m_desNode->setVisible(false);
        return;
    }
//    int position = (m_itemIndex+index)%9;
    int position = index;
    m_desNode->setVisible(true);
    vector<string> infoVec;
//    LotteryController::shared()->getItemDes(position, infoVec, true);
    
    std::string indexStr = CCUserDefault::sharedUserDefault()->getStringForKey(LotteryAct2ShowView_PREVIEW_REWARD);
    vector<std::string> infoVec1;
    CCCommonUtils::splitString(indexStr, ",", infoVec1);
    auto iter = find(infoVec1.begin(), infoVec1.end(), CC_ITOA(position));
    if(iter!=infoVec1.end()){
        int iPos = iter - infoVec1.begin();
        LotteryController::shared()->getItemDes(iPos, infoVec, true);
    }
    
    m_desName->setString(infoVec.at(0));
    m_desLabel->setString(infoVec.at(1));
    float extH = m_desLabel->getContentSize().height - 80;
    extH = extH>0?extH:0;
    m_desBg->setPreferredSize(CCSizeMake(295, 149+extH));
    CCNode* cell = NULL;
    cell = m_aniListNode->getChildByTag(index);
    CCPoint pos = cell->getPosition();
    if (index%3 == 2)
    {
        pos = ccpAdd(pos, ccp(cell->getContentSize().width/2 - 295, 0));
    }
    else
    {
        pos = ccpAdd(pos, ccp(295/2, 0));
    }
    m_desNode->setPosition(pos);
}

string LotteryAct2ShowView::getIcon(string itemId)
{
    int id = atoi(itemId.c_str());
    return LotteryController::shared()->getLotteryIcon(id);
}

string LotteryAct2ShowView::getColor(string itemId)
{
    int id = atoi(itemId.c_str());
    if (id >= 200 && id <= 205)
    {
        return "5";
    }
    else if (id != R_GOODS && id < 100)
    {
        return "4";
    }
    else{
        return CCCommonUtils::getPropById(itemId, "color");
    }
}

void LotteryAct2ShowView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void LotteryAct2ShowView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

int LotteryAct2ShowView::hasTouchNode(cocos2d::CCTouch *pTouch)
{
    CCNode* node = NULL;
    for (int i = 0; i < m_aniListNode->getChildrenCount(); i++)
    {
        node = m_aniListNode->getChildByTag(i);
        if (isTouchInside(node->getChildByTag(0), pTouch))
        {
            return i;
        }
    }
    return -1;
}

bool LotteryAct2ShowView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_aniIndex = hasTouchNode(pTouch);
    if (m_aniIndex > -1)
    {
        setDesNode(m_aniIndex);
        return true;
    }
    if (!isTouchInside(m_bg, pTouch))
    {
        return true;
    }
    return false;
}

void LotteryAct2ShowView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_bg, pTouch))
    {
        PopupViewController::getInstance()->removePopupView(this);
        return;
    }
    m_aniIndex = -1;
    m_desNode->setVisible(false);
}

bool LotteryAct2ShowView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aniList", CCNode*, m_aniListNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midNode", CCNode*, m_midNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTTF", CCLabelIF*, m_titleTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desBg", CCScale9Sprite*, m_desBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    return false;
}

SEL_CCControlHandler LotteryAct2ShowView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    return NULL;
}
