
#include "BattleSoldier2.h"

//六个方向 SE S SW NW N NE 1为上，0为下
//兵种图片命名 a010_1_NE_attack_0.png
// tao.yu int armType 不知道是什么
BattleSoldier2* BattleSoldier2::create(CCNode* batchNode,CCNode* pNode,int armType,int side,std::string icon,std::string direct,bool isHead){
    BattleSoldier2* ret = new BattleSoldier2(batchNode,pNode,armType,side,icon,direct,isHead);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void BattleSoldier2::onEnter(){
    CCNode::onEnter();
    scheduleUpdate();
}

void BattleSoldier2::onExit(){
    CCNode::onExit();
}

bool BattleSoldier2::init(){
    // CCString* filePath = CCString::createWithFormat("%s_%d_%s_attack_0.png", m_icon.c_str(), m_side, "N");
    CCString* filePath = CCString::createWithFormat("%s_%d_%s_move_0.png", m_icon.c_str(), m_side, m_direct != "SW" ? "N" : "SW"); // guo.jiang
    if(CCLoadSprite::getSF(filePath->getCString())==NULL){
        CCLOG("filePath=%s",filePath->getCString());
    }
    m_shadow = CCLoadSprite::createSprite("shadow.png");
    m_shadow->setAnchorPoint(ccp(0.5, 0.5));
    m_shadow->setOpacity(127);
    m_batchNode->addChild(m_shadow);
    
    m_iconSpr = CCLoadSprite::createSprite(filePath->getCString());
    m_iconSpr->setAnchorPoint(ccp(0.5,0.5));
    m_batchNode->addChild(m_iconSpr);
    //begin a by ljf  //解决造出兵时的遮挡问题
    m_iconSpr->setCameraMask(m_batchNode->getCameraMask(), true);
    //end a by ljf
    changeDirect(m_direct);
    float scale = 0.5;
    if(m_isHead){
        scale = 2;
    }
    m_iconSpr->setScale(scale);
    return true;
}

void BattleSoldier2::playAnimation(ActionStatus status,float delayTime,CCCallFunc* completeFunc,int loopTime){
    if(m_iconSpr==NULL) return ;
    setCompleteFun(completeFunc);
    m_status = status;
    if(m_status==ACTION_STAND){
        m_iconSpr->stopAllActions();
    }
    if(loopTime==0 && (m_armType==101 || m_armType==100) && m_status==ACTION_ATTACK){
        m_loopTimes = 1;
    }else{
        m_loopTimes = loopTime==0?std::numeric_limits<int>::max():loopTime;
    }
    if(delayTime>0){
        CCDelayTime* delay = CCDelayTime::create(delayTime);
        m_iconSpr->runAction(CCSequence::create(delay,CCCallFunc::create(this, callfunc_selector(BattleSoldier2::delayPlayAnimation)),NULL));
    }else{
        this->delayPlayAnimation();
    }
}

void BattleSoldier2::setSoldierPosition(CCPoint p)
{
    m_point = p;
    m_iconSpr->setPosition(m_point);
    update(0);
}

CCPoint BattleSoldier2::getSoldierPosition()
{
    return m_iconSpr->getPosition();
}

void BattleSoldier2::playAttack(float t){
    if((m_armType == 1 || m_armType == 3 || m_armType == 4 || m_armType == 7) && m_status==ACTION_ATTACK){
        CCPoint pos;
        CCPoint p2 = m_iconSpr->getParent()->convertToWorldSpace(m_iconSpr->getPosition());
        CCPoint localP = m_pNode->convertToNodeSpaceAR(p2);
        if(m_armType == 1){
            pos = ccp(localP.x+CCMathUtils::getRandomInt(450, 600), localP.y-CCMathUtils::getRandomInt(300, 400));
        }
        else if(m_armType == 7){
            pos = ccp(localP.x+CCMathUtils::getRandomInt(600, 700), localP.y-CCMathUtils::getRandomInt(400, 500));
        }
        else if(m_armType == 3){
            pos = ccp(localP.x-CCMathUtils::getRandomInt(600, 700), localP.y+CCMathUtils::getRandomInt(400, 500));
        }else{
            pos = ccp(localP.x-CCMathUtils::getRandomInt(450, 600), localP.y+CCMathUtils::getRandomInt(300, 400));
        }
        if (m_armType == 3 || m_armType == 7) {
            GongJian2* gong = GongJian2::create(m_pNode, localP, pos, 1, "stone_che.png");
        }
        else {
            GongJian2* gong = GongJian2::create(m_pNode, localP, pos, 1, "jian_0.png");
        }
    }
}

void BattleSoldier2::removeFromBatchNode(float dt){
    if (dt>=0) {
        m_iconSpr->runAction(CCFadeOut::create(dt));
    }
    else {
        m_iconSpr->runAction(CCFadeOut::create(CCMathUtils::getRandom(1.5,4.0)));
    }
}

void BattleSoldier2::playDie(){
    if(m_iconSpr == NULL || m_iconSpr->getParent()==NULL) return ;
//    SoldierHpAni* ani = SoldierHpAni::create(m_side);
//    ani->setBatchNode(m_pNode);
//    CCPoint p2 = m_iconSpr->getParent()->convertToWorldSpace(m_point);
//    CCPoint localP = m_pNode->convertToNodeSpaceAR(p2);
//    ani->setLose(0, -CCMathUtils::getRandom(1,100),localP,CCMathUtils::getRandom(0,0.3)+0.2);
    this->playAnimation(ACTION_DEATH,0,NULL,1);
//    this->scheduleOnce(schedule_selector(BattleSoldier2::removeFromBatchNode), 1);
}


void BattleSoldier2::changeDirect(std::string direct,bool replay){
    std::string realDirect = BattleManager::shared()->getWalkDirect(direct);
    m_direct = realDirect;
    if(m_iconSpr){
        if(direct=="NE" || direct=="SE" || direct=="E"){
            CCString* filePath = CCString::createWithFormat("%s_%d_%s_move_0.png",m_icon.c_str(),m_side,m_direct.c_str());
            CCSpriteFrame* cf = CCLoadSprite::getSF(filePath->getCString());
            if(cf==NULL){
                CCLOG("filePath=%s",filePath->getCString());
            }else{
                m_iconSpr->setDisplayFrame(cf);
            }
            m_iconSpr->setFlipX(true);
        }else{
          m_iconSpr->setFlipX(false);
        }
        if(replay){
            this->delayPlayAnimation();
        }
    }
    if(m_iconSpr && replay){
         this->delayPlayAnimation();
    }
}

void BattleSoldier2::moveToPosition(vector<CCPoint> posV,float delayTime, float moveSpd){
    m_movePositions = posV;
    m_moveIndex = 0;
    m_moveSpd = moveSpd==0?90:moveSpd;
    if(delayTime<=0){
        this->moveOnePoint(0);
    }else{
        this->scheduleOnce(schedule_selector(BattleSoldier2::moveOnePoint), delayTime);
    }
}
void BattleSoldier2::moveOnePoint()
{
    moveOnePoint(0);
}
void BattleSoldier2::moveOnePoint(float t){
    if(m_movePositions.size()>m_moveIndex){
        CCPoint onePos = m_movePositions[m_moveIndex];
        CCPoint lastPos = m_point;
        if(m_moveIndex!=0){
            lastPos = m_movePositions[m_moveIndex-1];
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHECK_ZORDER, this);
        m_lastDirect = getDirection(lastPos,onePos);
        this->changeDirect(m_lastDirect);
        m_status = ACTION_MOVE;
        m_loopTimes = numeric_limits<int>::max();
        
        this->unschedule(schedule_selector(BattleSoldier2::playAttack));
        if (m_armType == 1 || m_armType == 4 || m_armType == 3 || m_armType == 7)
        {
            this->schedule(schedule_selector(BattleSoldier2::playAttack), 2);
            playAttack(0);
        }
        
        CCPoint gap = ccpSub(lastPos, onePos);
        float len = ccpLength(gap);
        float useTime = len/m_moveSpd;//len/120.0;
        CCSequence* sc = CCSequence::create(CCMoveTo::create(useTime, onePos),CCCallFunc::create(this, callfunc_selector(BattleSoldier2::moveOnePoint)),NULL);
        Action* action = createAnimation();
        if (action)
        {
            auto sp = Spawn::create(sc, action, NULL);
            m_iconSpr->runAction(sp);
        }
        else
        {
            m_iconSpr->runAction(sc);
        }
        m_moveIndex += 1;
    }else{
        m_status = ACTION_STAND;
//        this->changeDirect("NE",true);
        this->changeDirect(m_lastDirect,true);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SOLDIER_STOP, this);
    }
}

std::string BattleSoldier2::getDirection(CCPoint from,CCPoint to){
    std::string direct = "";
    int degree = CCMathUtils::getAngle(from, to);
    degree = 360 + degree;
    degree = degree%360;
    if(degree>=22.5&&degree<67.5){
        direct = "NE";
    }else if(degree>=67.5&&degree<112.5){
        direct = "N";
    }else if(degree>=112.5&&degree<157.5){
        direct = "NW";
    }else if(degree>=157.5&&degree<202.5){
        direct = "W";
    }else if(degree>=202.5&&degree<247.5){
        direct = "SW";
    }else if(degree>247.5&&degree<292.5){
        direct = "S";
    }else if(degree>292.5&&degree<337.5){
        direct = "SE";
    }else{
        direct = "E";
    }
    return direct;
}

Action* BattleSoldier2::createAnimation()
{
    Action* result = nullptr;
    if (m_iconSpr == NULL || m_iconSpr->getReferenceCount() <= 0)
    {
        return result;
    }
    
    m_iconSpr->stopAllActions();
    int size = 12;
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *cf;
    
    int totalFrame = 0;
    std::string actionName = MOVE;
    if (m_status == S_DEATH)
    {
        actionName = DEATH;
    }
    else if (m_status == S_ATTACK)
    {
        actionName = ATTACK;
    }
    else if (m_status == S_MOVE)
    {
        actionName = MOVE;
    }
    else if (m_status == S_HURT)
    {
        actionName = HURT;
    }
    else if (m_status == S_STAND)
    {
        actionName = STAND;
    }
    
    if (m_icon == "a010")
    { // 步兵
        if (m_status == S_DEATH)
        {
            totalFrame = 0;
        }
        else if (m_status == S_ATTACK)
        {
            totalFrame = 0;
        }
        else if (m_status == S_MOVE)
        {
            totalFrame = 7;
        }
        else if (m_status == S_HURT)
        {
            totalFrame = 0;
        }
        else if (m_status == S_STAND)
        {
            totalFrame = 10;
        }
    }
    else if (m_icon == "a020")
    { // 骑兵
        if (m_status == S_DEATH)
        {
            totalFrame = 0;
        }
        else if (m_status == S_ATTACK)
        {
            totalFrame = 0;
        }
        else if (m_status == S_MOVE)
        {
            totalFrame = 8;
        }
        else if (m_status == S_HURT)
        {
            totalFrame = 0;
        }
        else if (m_status == S_STAND)
        {
            totalFrame = 8;
        }
    }
    else if (m_icon == "a060")
    { // 弓兵
        if (m_status == S_DEATH)
        {
            totalFrame = 0;
        }
        else if (m_status == S_ATTACK)
        {
            totalFrame = 0;
        }
        else if (m_status == S_MOVE)
        {
            totalFrame = 6;
        }
        else if (m_status == S_HURT)
        {
            totalFrame = 0;
        }
        else if (m_status == S_STAND)
        {
            totalFrame = 7;
        }
    }
    else if (m_icon == "zhanche")
    { // 法师
        if (m_status == S_DEATH)
        {
            totalFrame = 0;
        }
        else if (m_status == S_ATTACK)
        {
            totalFrame = 0;
        }
        else if (m_status == S_MOVE)
        {
            totalFrame = 8;
        }
        else if (m_status == S_HURT)
        {
            totalFrame = 0;
        }
        else if (m_status == S_STAND)
        {
            totalFrame = 8;
        }
    }
    
    for (int j = 0; j < totalFrame; j++)
    {
        cf = CCLoadSprite::getSF(CCString::createWithFormat("%s_%d_%s_%s_%d.png", m_icon.c_str(), m_side, m_direct.c_str(), actionName.c_str(), j)->getCString());
        CC_BREAK_IF(!cf);
        myArray.pushBack(cf);
        m_iconSpr->setDisplayFrame(cf);
    }
    if (m_status == S_STAND)
    {
        int j = 0;
        int cnt = 10 + rand() % 20;
        for (int i = 0; i < cnt; i++)
        {
            cf = CCLoadSprite::getSF(CCString::createWithFormat("%s_%d_%s_%s_%d.png", m_icon.c_str(), m_side,m_direct.c_str(), actionName.c_str(), j)->getCString());
            if (cf == nullptr)
            {
                continue;
            }
            myArray.pushBack(cf);
            m_iconSpr->setDisplayFrame(cf);
        }
    }
    
    m_iconSpr->setColor(ccWHITE);
    float delayPerUnit = 0.1;
    if (m_status == ACTION_ATTACK)
    {
        delayPerUnit = ATTACK_SOLDIER_DELAY_PER_UNIT;
    }
    else if (m_status==ACTION_MOVE)
    {
        delayPerUnit = MOVE_SOLDIER_DELAY_PER_UNIT;
    }
    else if (m_status == ACTION_HURT)
    {
        delayPerUnit = HURT_SOLDIER_DELAY_PER_UNIT;
    }
    else if (m_status == ACTION_DEATH)
    {
        delayPerUnit = 0.13;
    }
    else if (m_status == S_STAND)
    {
        float random_variable = rand() % 10 + 5.0;
        delayPerUnit = 0.1 + 1.0 / random_variable;
    }
    else
    {
        delayPerUnit = SOLDIER_DELAY_PER_UNIT;
    }
    
    float factor = GlobalData::shared()->speedFactor;
    if (m_status != ACTION_MOVE)
    {
        delayPerUnit = delayPerUnit * factor;
    }
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(myArray, delayPerUnit);
    animation->setLoops(m_loopTimes);
    animation->setRestoreOriginalFrame(false);
    m_iconSpr->stopAllActions();
    if (m_status != ACTION_HURT)
    {
        if (m_completeTarget && m_completeFunc)
        {
            result = CCSequence::create(CCAnimate::create(animation), getCompleteFun(), NULL);
        }
        else
        {
            float random_variable = rand() % 5 + 0.1;
            if (m_status != S_STAND) random_variable = 0;
            result = CCSequence::create(DelayTime::create(random_variable), CCAnimate::create(animation), NULL);
        }
    }
    else if (m_status != ACTION_DEATH)
    {
        CCSequence* s2;
        if (m_completeTarget && m_completeFunc)
        {
            s2 = CCSequence::create(CCAnimate::create(animation), getCompleteFun(), NULL);
        }
        else
        {
            s2 = CCSequence::create(CCAnimate::create(animation), NULL);
        }
        CCSpawn* sp = CCSpawn::create(s2, NULL);
        result = sp;
    }
    else
    {
        if (m_completeFunc && m_completeTarget)
        {
            result = CCSequence::create(CCAnimate::create(animation), getCompleteFun(), NULL);
        }
        else
        {
            result = CCSequence::create(CCAnimate::create(animation), NULL);
        }
    }
    
    return result;
}

void BattleSoldier2::delayPlayAnimation(){
    
    if (m_iconSpr == NULL||m_iconSpr->getReferenceCount()<=0) {
        return ;
    }
    
    Action* action = createAnimation();
    if (action) m_iconSpr->runAction(action);
    
   // myArray->release();
    this->unschedule(schedule_selector(BattleSoldier2::playAttack));
    if (m_armType==1||m_armType==4||m_armType==3||m_armType==7) {//
        this->schedule(schedule_selector(BattleSoldier2::playAttack), 2);
        playAttack(0);
    }
}

void BattleSoldier2::setSprScale(float sc)
{
    m_iconSpr->setScale(sc);
}

void BattleSoldier2::update(float delta)
{
    if (m_shadow && m_iconSpr)
    {
        m_shadow->setPosition(m_iconSpr->getPosition() + Vec2(-5, -14));
    }
}
