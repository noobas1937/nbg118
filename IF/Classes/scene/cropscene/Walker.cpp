
#include "Walker.h"
#include "FunBuildController.h"

//六个方向 SE S SW NW N NE 1为上，0为下
//兵种图片命名 a010_1_NE_attack_0.png
// tao.yu int armType 不知道是什么
Walker* Walker::create(CCNode* batchNode,CCNode* pNode,int armType,int side,std::string icon,std::string direct,bool isHead){
    Walker* ret = new Walker(batchNode,pNode,armType,side,icon,direct,isHead);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void Walker::onEnter(){
    CCNode::onEnter();
    scheduleUpdate();
}

void Walker::onExit(){
    CCNode::onExit();
}

bool Walker::init(){
    // CCString* filePath = CCString::createWithFormat("%s_%d_%WALKER_S_WAVE_0.png", m_icon.c_str(), m_side, "N");
    CCString* filePath = CCString::createWithFormat("%s_%d_%s_move_0.png", m_icon.c_str(), m_side, m_direct != "SW" ? "N" : "SW"); // guo.jiang
    if(CCLoadSprite::getSF(filePath->getCString())==NULL){
        CCLOG("filePath=%s",filePath->getCString());
    }
    m_shadow = CCLoadSprite::createSprite("walker_shadow.png");
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
    
    initPath();
    
    
    startMove();
    
    return true;
}

void Walker::playAnimation(WalkerActionStatus status,float delayTime,CCCallFunc* completeFunc,int loopTime){
    if(m_iconSpr==NULL) return ;
    cocos2d::CCObject* varTarget = m_completeTarget;
    cocos2d::SEL_CallFunc varFunc = m_completeFunc;
    if(completeFunc)
    {
        cocos2d::CCObject* varTarget2 = completeFunc->getTargetCallback();
        cocos2d::SEL_CallFunc varFunc2 = completeFunc->getCallFunc();
        int a = 9;
    }
    setCompleteFun(completeFunc);
    cocos2d::CCObject* varTarget1 = m_completeTarget;
    cocos2d::SEL_CallFunc varFunc1 = m_completeFunc;
    m_status = status;
    //if(m_status==WALKER_ACTION_STAND)
    {
        m_iconSpr->stopAllActions();
    }
    if(loopTime==0 && (m_armType==101 || m_armType==100) && m_status==WALKER_ACTION_WAVE){
        m_loopTimes = 1;
    }else{
        m_loopTimes = loopTime==0?std::numeric_limits<int>::max():loopTime;
    }
    if(delayTime>0){
        CCDelayTime* delay = CCDelayTime::create(delayTime);
        m_iconSpr->runAction(CCSequence::create(delay,CCCallFunc::create(this, callfunc_selector(Walker::delayPlayAnimation)),NULL));
    }else{
        this->delayPlayAnimation();
    }
}

void Walker::setSoldierPosition(CCPoint p)
{
    m_point = p;
    m_iconSpr->setPosition(m_point);
    update(0);
}

CCPoint Walker::getSoldierPosition()
{
    return m_iconSpr->getPosition();
}

void Walker::playAttack(float t){
    if((m_armType == 1 || m_armType == 3 || m_armType == 4 || m_armType == 7) && m_status==WALKER_ACTION_WAVE){
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

void Walker::removeFromBatchNode(float dt){
    if (dt>=0) {
        m_iconSpr->runAction(CCFadeOut::create(dt));
    }
    else {
        m_iconSpr->runAction(CCFadeOut::create(CCMathUtils::getRandom(1.5,4.0)));
    }
}

void Walker::playDie(){
    if(m_iconSpr == NULL || m_iconSpr->getParent()==NULL) return ;
//    SoldierHpAni* ani = SoldierHpAni::create(m_side);
//    ani->setBatchNode(m_pNode);
//    CCPoint p2 = m_iconSpr->getParent()->convertToWorldSpace(m_point);
//    CCPoint localP = m_pNode->convertToNodeSpaceAR(p2);
//    ani->setLose(0, -CCMathUtils::getRandom(1,100),localP,CCMathUtils::getRandom(0,0.3)+0.2);
    this->playAnimation(WALKER_ACTION_WORK,0,NULL,1);
//    this->scheduleOnce(schedule_selector(Walker::removeFromBatchNode), 1);
}


void Walker::changeDirect(std::string direct,bool replay){
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

//void Walker::moveToPosition(int currentSeq, int targetSeq, vector<CCPoint> posV,float delayTime, float moveSpd){
void Walker::moveToPosition(vector<int> seqs, vector<CCPoint> posV,float delayTime, float moveSpd){
    m_movePositions = posV;
    mTargetSeq = seqs[0];
    mCurrentSeq = seqs[0];
    m_moveSeqs = seqs;
    m_moveIndex = 0;
    m_moveSpd = moveSpd==0?90:moveSpd;
    if(delayTime<=0){
        this->moveOnePoint(0);
    }else{
        this->scheduleOnce(schedule_selector(Walker::moveOnePoint), delayTime);
    }
}
void Walker::moveOnePoint()
{
    moveOnePoint(0);
}
void Walker::moveOnePoint(float tab){
    if(m_movePositions.size()>m_moveIndex){
        mCurrentSeq = mTargetSeq;
        mTargetSeq = m_moveSeqs[m_moveIndex];
        CCPoint onePos = m_movePositions[m_moveIndex];
        CCPoint lastPos = m_point;
        if(m_moveIndex!=0){
            lastPos = m_movePositions[m_moveIndex-1];
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHECK_ZORDER, this);
        m_lastDirect = getDirection(lastPos,onePos);
        if(m_moveIndex != 0)
        {
            this->changeDirect(m_lastDirect);
        }
        m_status = WALKER_ACTION_MOVE;
        m_loopTimes = numeric_limits<int>::max();
        
        this->unschedule(schedule_selector(Walker::playAttack));
        if (m_armType == 1 || m_armType == 4 || m_armType == 3 || m_armType == 7)
        {
            this->schedule(schedule_selector(Walker::playAttack), 2);
            playAttack(0);
        }
        
        CCPoint gap = ccpSub(lastPos, onePos);
        float len = ccpLength(gap);
        float useTime = len/m_moveSpd;//len/120.0;
        CCSequence* sc = nullptr;
        Action* action = nullptr;
        if(useTime == 0)
        {
            sc = CCSequence::create(CCCallFunc::create(this, callfunc_selector(Walker::moveOnePoint)),NULL);
        }
        else
        {
            sc = CCSequence::create(CCMoveTo::create(useTime, onePos),CCCallFunc::create(this, callfunc_selector(Walker::moveOnePoint)),NULL);
            action = createAnimation();
        }
        //Action* action = createAnimation();
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
        /*
        m_status = WALKER_ACTION_STAND;
        this->changeDirect(m_lastDirect,true);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SOLDIER_STOP, this);
        */
        onMoveFinished();
    }
}

std::string Walker::getDirection(CCPoint from,CCPoint to){
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

Action* Walker::createAnimation()
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
    std::string actionName = "move";
    if (m_status == WALKER_S_WORK)
    {
        actionName = "work";
    }
    else if (m_status == WALKER_S_WAVE)
    {
        actionName = "wave";
    }
    else if (m_status == WALKER_S_MOVE)
    {
        actionName = "move";
    }
    else if (m_status == WALKER_S_REST)
    {
        actionName = "rest";
    }
    else if (m_status == WALKER_S_STAND)
    {
        actionName = "move";
    }
    
    //if (m_icon == "a010")
    if (m_icon == "b010")
    { // 步兵
        if (m_status == WALKER_S_WORK)
        {
            totalFrame = 10;
        }
        else if (m_status == WALKER_S_WAVE)
        {
            totalFrame = 9;
        }
        else if (m_status == WALKER_S_MOVE)
        {
            totalFrame = 8;
        }
        else if (m_status == WALKER_S_REST)
        {
            totalFrame = 8;
        }
        else if (m_status == WALKER_S_STAND)
        {
            totalFrame = 10;
        }
    }
    else if (m_icon == "b020")
    { // 骑兵
        if (m_status == WALKER_S_WORK)
        {
            totalFrame = 0;
        }
        else if (m_status == WALKER_S_WAVE)
        {
            totalFrame = 0;
        }
        else if (m_status == WALKER_S_MOVE)
        {
            totalFrame = 8;
        }
        else if (m_status == WALKER_S_REST)
        {
            totalFrame = 8;
        }
        else if (m_status == WALKER_S_STAND)
        {
            totalFrame = 8;
        }
    }
    else if (m_icon == "a060")
    { // 弓兵
        if (m_status == WALKER_S_WORK)
        {
            totalFrame = 0;
        }
        else if (m_status == WALKER_S_WAVE)
        {
            totalFrame = 0;
        }
        else if (m_status == WALKER_S_MOVE)
        {
            totalFrame = 6;
        }
        else if (m_status == WALKER_S_REST)
        {
            totalFrame = 0;
        }
        else if (m_status == WALKER_S_STAND)
        {
            totalFrame = 7;
        }
    }
    else if (m_icon == "zhanche")
    { // 法师
        if (m_status == WALKER_S_WORK)
        {
            totalFrame = 0;
        }
        else if (m_status == WALKER_S_WAVE)
        {
            totalFrame = 0;
        }
        else if (m_status == WALKER_S_MOVE)
        {
            totalFrame = 8;
        }
        else if (m_status == WALKER_S_REST)
        {
            totalFrame = 0;
        }
        else if (m_status == WALKER_S_STAND)
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
    if (m_status == WALKER_S_STAND)
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
    if (m_status == WALKER_ACTION_WAVE)
    {
        delayPerUnit = 0.2;
    }
    else if (m_status==WALKER_ACTION_MOVE)
    {
        delayPerUnit = MOVE_SOLDIER_DELAY_PER_UNIT;
    }
    else if (m_status == WALKER_ACTION_REST)
    {
        delayPerUnit = HURT_SOLDIER_DELAY_PER_UNIT;
    }
    else if (m_status == WALKER_ACTION_WORK)
    {
        delayPerUnit = 0.13;
    }
    else if (m_status == WALKER_S_STAND)
    {
        float random_variable = rand() % 10 + 5.0;
        delayPerUnit = 0.1 + 1.0 / random_variable;
    }
    else
    {
        delayPerUnit = SOLDIER_DELAY_PER_UNIT;
    }
    
    float factor = GlobalData::shared()->speedFactor;
    if (m_status != WALKER_ACTION_MOVE)
    {
        delayPerUnit = delayPerUnit * factor;
    }
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(myArray, delayPerUnit);
    animation->setLoops(m_loopTimes);
    animation->setRestoreOriginalFrame(false);
    m_iconSpr->stopAllActions();
    if (m_status != WALKER_ACTION_REST)
    {
        if (m_completeTarget && m_completeFunc)
        {
            result = CCSequence::create(CCAnimate::create(animation), getCompleteFun(), NULL);
        }
        else
        {
            float random_variable = rand() % 5 + 0.1;
            if (m_status != WALKER_S_STAND) random_variable = 0;
            result = CCSequence::create(DelayTime::create(random_variable), CCAnimate::create(animation), NULL);
        }
    }
    else if (m_status != WALKER_ACTION_WORK)
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

void Walker::delayPlayAnimation(){
    
    if (m_iconSpr == NULL||m_iconSpr->getReferenceCount()<=0) {
        return ;
    }
    Action* action = createAnimation();
    if(action)
    {
        //CCCallFuncO* fun1 = CCCallFuncO::create(this, callfuncO_selector(Walker::continueMove), NULL);
        //Action *  result = CCSequence::create(action, fun1, NULL);
        //CCSequence *  result = CCSequence::create(action, fun1, NULL);
        m_iconSpr->runAction(action);
    }
    
   // myArray->release();
    this->unschedule(schedule_selector(Walker::playAttack));
    if (m_armType==1||m_armType==4||m_armType==3||m_armType==7) {//
        this->schedule(schedule_selector(Walker::playAttack), 2);
        playAttack(0);
    }
}

void Walker::setSprScale(float sc)
{
    m_iconSpr->setScale(sc);
}

void Walker::update(float delta)
{
    if (m_shadow && m_iconSpr)
    {
        m_shadow->setPosition(m_iconSpr->getPosition() + Vec2(-5, -14));
    }
}

void Walker::startMove()
{
    int totalPointInGraph = mPointsInPathGraph.size();
    int randSeq = 1 + rand() % (totalPointInGraph - 1);
    
    nextMove(randSeq);
}

void Walker::onMoveFinished()
{
    auto iter = mPointsInPathGraph.find(mTargetSeq);
    int type = iter->second.type;
    if (type != POINT_TYPE_MOVE)
    {
        nextWork(type);
    }
    else
    {
        nextMove(mTargetSeq);
    }
    /*
    if (false)
    {

        CCCallFunc * fun1 = CCCallFunc::create(this, callfunc_selector(Walker::onWorkFinished));
       
        playAnimation(WalkerActionStatus::WALKER_ACTION_MOVE, 0.1, fun1, 10);
        
    }
    */
}

void Walker::onWorkFinished()
{
    if(true)
    {
        nextMove(mTargetSeq);
    }
}

void Walker::initPath()
{
    vector<CCPoint> path;
    /*
    //第一条路
    path.push_back(Vec2(1521, 2146)); // p1
    path.push_back(Vec2(1729, 2246)); // p2
    path.push_back(Vec2(1837, 2102)); // p3
    path.push_back(Vec2(1901, 2030)); // p4
    path.push_back(Vec2(2109, 2121)); // p5
    path.push_back(Vec2(2189, 2206)); // p6
    path.push_back(Vec2(2325, 2271)); // p7
    path.push_back(Vec2(2514, 2206)); // p8
    path.push_back(Vec2(2806, 2370)); // p9
    
    
    //第二条路，前面两个点跟上一路线后面两个点是相同的
    //path.push_back(Vec2(2806, 2370)); //p9
    //path.push_back(Vec2(2514, 2206)); //p8
    path.push_back(Vec2(2784, 1876)); //p10
    path.push_back(Vec2(2293, 1511));//p11
    path.push_back(Vec2(3431, 909)); //p12
     
     addEdgeToPathGraph(10, 8);
     addEdgeToPathGraph(11, 10);
     addEdgeToPathGraph(12, 11);
    */
    /* new path
     (1849, 1353)
     (1742, 1503)//建筑角
     (1802, 1543)
     (1571, 1689)
     (1434, 1871)//建筑角
     （1521， 1926）
     （1606， 1896）
     （1914， 2048） //挥手
     （2183，2192）
     （2123，2237）
     （2258，2197）
     （2340，2237）
     （2464，2202）
     （2519，2215）
     （2840，2399）
     */
//    path.push_back(Vec2(1859, 1361)); // p1
//    path.push_back(Vec2(1735, 1518)); // p2 //建筑角
//    path.push_back(Vec2(1790, 1568)); // p3
//    path.push_back(Vec2(1576, 1689)); // p4
//    path.push_back(Vec2(1426, 1886)); // p5 //建筑角
//    path.push_back(Vec2(1548, 1968)); // p6
//    path.push_back(Vec2(1625, 1913)); // p7
//    path.push_back(Vec2(1687, 1988)); // p8 //建筑面前
//    path.push_back(Vec2(1902, 2050)); // p9 //挥手
//    path.push_back(Vec2(2166, 2174)); // p10
//    
//    path.push_back(Vec2(2098,2244)); // p11, 开始下楼梯
//    path.push_back(Vec2(2258,2210)); // p12
//    path.push_back(Vec2(2320,2247)); // p13 开始下楼梯
//    path.push_back(Vec2(2459,2222)); // p14
//    path.push_back(Vec2(2519,2215)); // p15 开始过右边桥
//    path.push_back(Vec2(2840,2399)); // p16
//
//    path.push_back(Vec2(2722,2065)); // p17
//    path.push_back(Vec2(2784, 1876)); //p18
//    path.push_back(Vec2(2293, 1511));//p19，开始过中间桥
//    path.push_back(Vec2(3431, 909)); //p20
//    
//    path.push_back(Vec2(1956, 1269)); //p21，河边呼吸
    
    int path_ox = 0;
    int path_oy = 0;
    path.push_back(Vec2(1786 + path_ox, 1258 + path_oy)); // p1
    path.push_back(Vec2(1645 + path_ox, 1294 + path_oy)); // p2
    path.push_back(Vec2(1778 + path_ox, 1366 + path_oy)); // p3
    path.push_back(Vec2(1850 + path_ox, 1332 + path_oy)); // p4
    path.push_back(Vec2(1943 + path_ox, 1352 + path_oy)); // p5
    path.push_back(Vec2(2056 + path_ox, 1387 + path_oy)); // p6
    path.push_back(Vec2(2187 + path_ox, 1446 + path_oy)); // p7
    path.push_back(Vec2(2102 + path_ox, 1503 + path_oy)); // p8
    path.push_back(Vec2(2141 + path_ox, 1576 + path_oy)); // p9
    path.push_back(Vec2(2286 + path_ox, 1491 + path_oy)); // p10
    path.push_back(Vec2(2079 + path_ox, 1655 + path_oy)); // p11
    path.push_back(Vec2(2011 + path_ox, 1728 + path_oy)); // p12
    path.push_back(Vec2(1913 + path_ox, 1826 + path_oy)); // p13
    path.push_back(Vec2(1804 + path_ox, 1898 + path_oy)); // p14
    path.push_back(Vec2(1963 + path_ox, 1932 + path_oy)); // p15
    path.push_back(Vec2(2029 + path_ox, 2012 + path_oy)); // p16
    path.push_back(Vec2(2011 + path_ox, 1851 + path_oy)); // p17
    path.push_back(Vec2(2091 + path_ox, 1723 + path_oy)); // p18
    path.push_back(Vec2(2172 + path_ox, 1654 + path_oy)); // p19
    path.push_back(Vec2(2286 + path_ox, 1617 + path_oy)); // p20
    path.push_back(Vec2(2341 + path_ox, 1553 + path_oy)); // p21

    
    int i = 1;
    for(auto iter = path.begin(); iter != path.end(); ++iter)
    {
        WalkerPathPoint p((*iter).x, (*iter).y, i, POINT_TYPE_MOVE);
        if(i == 9)
        {
            p.type = POINT_TYPE_WAVE;
        }
        if(i == 8)
        {
            p.type = POINT_TYPE_WORK;
        }
        if(i == 5)
        {
            //p.type = POINT_TYPE_WORK;
        }
        if(i == 21)
        {
            p.type = POINT_TYPE_REST;
        }
        addPointToPathGraph(p);
        //if(i > 1)
        //{
            //addEdgeToPathGraph(i, i - 1);
        //}
        i++;
    }
    
    for(int j = 2; j <= 16; j++)
    {
        addEdgeToPathGraph(j, j - 1);
    }
    
    for(int j = 18; j <= 20; j++)
    {
        addEdgeToPathGraph(j, j - 1);
    }
    addEdgeToPathGraph(15, 17);
    addEdgeToPathGraph(1, 21);
    addEdgeToPathGraph(21, 19);
}

void Walker::addPointToPathGraph(WalkerPathPoint & pathPoint)
{
    if(mPointsInPathGraph.find(pathPoint.seq) == mPointsInPathGraph.end())
    {
        mPointsInPathGraph.insert(pair<int, WalkerPathPoint>(pathPoint.seq, pathPoint));
    }
}

void Walker::addEdgeToPathGraph(int seq1, int seq2)
{
    auto iter1 = mPointsInPathGraph.find(seq1);
    auto iter2 = mPointsInPathGraph.find(seq2);
    
    if(iter1 == mPointsInPathGraph.end())
    {
        return;
    }
    if(iter2 == mPointsInPathGraph.end())
    {
        return;
    }
    auto pathPoint1 = &(iter1->second);
    auto pathPoint2 = &(iter2->second);
    bool find = false;
    for(auto iter = pathPoint1->connectedPointSeqs.begin(); iter != pathPoint1->connectedPointSeqs.end(); ++iter)
    {
        if(seq2 == *iter)
        {
            find = true;
            break;
        }
    }
    if(!find)
    {
        pathPoint1->connectedPointSeqs.push_back(seq2);
        pathPoint2->connectedPointSeqs.push_back(seq1);
    }
}

void Walker::nextMove(int currentSeq)
{
    int startSeq = currentSeq;
    //int targetSeq = 0;
    int prevSeq = mCurrentSeq;
    vector<CCPoint> path;
    vector<int> seqs;
    float spd = 40;
    if (m_icon == "b020")
    {
        spd = 50;
    }
    auto iter = mPointsInPathGraph.find(startSeq);
    
    setSoldierPosition(iter->second.point);
    
    for(int i = 0; i < 2; i++)
    {
        iter = mPointsInPathGraph.find(startSeq);
        
        
        if(iter != mPointsInPathGraph.end())
        {
            path.push_back(iter->second.point);
            seqs.push_back(startSeq);
            
            int availableTargetNum = iter->second.connectedPointSeqs.size();
            int randomTargetIndex = 0;
            if (availableTargetNum > 1)
            {
                randomTargetIndex = rand() % (availableTargetNum);
                //if(iter->second.connectedPointSeqs[randomTargetIndex] == mCurrentSeq)
                if(iter->second.connectedPointSeqs[randomTargetIndex] == prevSeq)
                {
                    if (randomTargetIndex == 0)
                    {
                        randomTargetIndex = randomTargetIndex + 1;
                    }
                    else if(randomTargetIndex == (availableTargetNum -1))
                    {
                        randomTargetIndex = randomTargetIndex - 1;
                    }
                    else
                    {
                        int secondRandom = rand() % (2);
                        if(secondRandom == 0)
                            randomTargetIndex = randomTargetIndex - 1;
                        else
                            randomTargetIndex = randomTargetIndex + 1;
                    }
                    
                }
                
            }
            prevSeq = startSeq;
            startSeq = iter->second.connectedPointSeqs[randomTargetIndex];
        }
        
        
    }
    //moveToPosition(currentSeq, prevSeq, path, 0, spd);
    moveToPosition(seqs, path, 0, spd);
}

void Walker::nextWork(int pointType)
{
    CCCallFunc * fun1 = CCCallFunc::create(this, callfunc_selector(Walker::onWorkFinished));
    WalkerActionStatus s = WALKER_ACTION_MOVE;
    if(pointType == POINT_TYPE_REST)
        s = WALKER_ACTION_REST;
    if(pointType == POINT_TYPE_WAVE)
        s = WALKER_ACTION_WAVE;
    if(pointType == POINT_TYPE_WORK)
        s = WALKER_ACTION_WORK;
    playAnimation(s, 0.1, fun1, 3);
}