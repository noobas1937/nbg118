//
//  AnniversaryInfoView.cpp
//
//  Created by zhangjun on 2015-07-16
//

#include "AnniversaryInfoView.h"
#include "PopupViewController.h"
#include "FBUtilies.h"
#include "WorldActivityCommand.h"
#include <spine/Json.h>
#include "../guiExt/md5.h"
#include "ImperialScene.h"

#define startX 10

AnniversaryInfoView::~AnniversaryInfoView()
{
    CCLOGFUNC();
}
AnniversaryInfoView* AnniversaryInfoView::create()
{
    AnniversaryInfoView *pRet = new AnniversaryInfoView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}
bool AnniversaryInfoView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(200, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(200, true);
    });
    /*Load CCB*/
    auto bg = CCBLoadFile("AnniversaryInfoView", this, this);
    
    m_headTxt->setString("m_headTxt");
    m_titleTxt->setString(_lang("150013"));
    m_scroll=CCScrollView::create(m_scrollNode->getContentSize());
    m_scroll->setDirection(kCCScrollViewDirectionVertical);
    m_scrollNode->addChild(m_scroll);
    
    
    
    return true;
}

void AnniversaryInfoView::setScrollData(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        return;
    }
    shareString[4]="";
    shareLang=LocalController::shared()->getLanguageFileName();
    
    playerName = GlobalData::shared()->playerInfo.name;
    itemPic ="";
    timeArr = CCArray::create();
    dialogArr = CCArray::create();

    timeArr->addObject(CCDouble::create(1433952000));
    dialogArr->addObject(CCString::create("150006"));//2015.6.11
    timeArr->addObject(CCDouble::create(1425916800));
    dialogArr->addObject(CCString::create("150005"));//2015.3.10
    timeArr->addObject(CCDouble::create(1413993600));
    dialogArr->addObject(CCString::create("150004"));//2014.10.23
    timeArr->addObject(CCDouble::create(1409760000));
    dialogArr->addObject(CCString::create("150003"));//2014.9.4
    timeArr->addObject(CCDouble::create(1404144000));
    dialogArr->addObject(CCString::create("150002"));//2014.7.1
    
    auto params = _dict(ret->getData());
    double regTime = params->valueForKey("regTime")->doubleValue();
    otherName = params->valueForKey("atkName")->getCString();
    double atkTime = params->valueForKey("atkTime")->doubleValue();
    double defTime = params->valueForKey("defTime")->doubleValue();
    double teamAtkTime = params->valueForKey("teamAtkTime")->doubleValue();
    insertIntoArr(regTime,1);
    insertIntoArr(atkTime,2);
    insertIntoArr(defTime,3);
    insertIntoArr(teamAtkTime,4);
    
//    long time0 = dynamic_cast<CCLong*>(timeArr->objectAtIndex(0))->getValue();
//    string timeStr = CCCommonUtils::timeStampToYMD(time0);
    string labStr = "";
    float py = 50;
    float lineH = 30;
    //分享按钮
    if(GlobalData::shared()->isAnniSharePhoto){
        string analyticID=GlobalData::shared()->analyticID;
        if(analyticID == "market_global"||CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            auto btnPic = CCLoadSprite::createScale9Sprite("btn_green3.png");
            auto controlBtn = CCControlButton::create(btnPic);
            float btnW=300;
            controlBtn->setPreferredSize(CCSizeMake(btnW, 64));
            CCCommonUtils::setButtonTitle(controlBtn, _lang("107098").c_str());
            controlBtn->setPosition(ccp(((m_scrollNode->getContentSize().width/2)-(btnW/2)), py));
            controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(AnniversaryInfoView::fbShare), CCControlEventTouchUpInside);
            m_scroll->addChild(controlBtn);
            py+=controlBtn->getContentSize().height;
            py+=10;
        }
    }else{
        int k_vers = GlobalData::shared()->anniSharePhoto;
        int versionStr = atoi(cocos2d::extension::CCDevice::getVersionCode().c_str());
        CCLOG("k_vers:%d,versionStr:%d",k_vers,versionStr);
        string analyticID=GlobalData::shared()->analyticID;
        if(analyticID == "market_global"||CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
            if (versionStr>k_vers&&k_vers>=0) {
                auto btnPic = CCLoadSprite::createScale9Sprite("btn_green3.png");
                auto controlBtn = CCControlButton::create(btnPic);
                float btnW=300;
                controlBtn->setPreferredSize(CCSizeMake(btnW, 64));
                CCCommonUtils::setButtonTitle(controlBtn, _lang("107098").c_str());
                controlBtn->setPosition(ccp(((m_scrollNode->getContentSize().width/2)-(btnW/2)), py));
                controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(AnniversaryInfoView::fbShare), CCControlEventTouchUpInside);
                m_scroll->addChild(controlBtn);
                py+=controlBtn->getContentSize().height;
                py+=10;
            }
        }
    }
    
    //写在最底下的话
    labStr=_lang_1("150012", playerName.c_str());
    CCLabelTTF* lab = CCLabelTTF::create(labStr.c_str(), "Helvetica", 20, CCSizeMake(450,0), kCCTextAlignmentLeft);
    lab->setPosition(CCPoint(startX, py));
    lab->setColor({81,48,13});
    m_scroll->addChild(lab);
    py+=lab->getContentSize().height+20;
    //最下时间节点，bar右边的话
    float dy=15;
    labStr=getStringByDialog(0);
    CCSprite* picSpr = CCLoadSprite::createSprite(itemPic.c_str());
    CCCommonUtils::setSpriteMaxSize(picSpr,360);
    picSpr->setPosition(CCPoint(startX+50, py));
    m_scroll->addChild(picSpr);
    py+=((picSpr->getContentSize().height)*(picSpr->getScaleY())+5);
    
    lab = CCLabelTTF::create(labStr.c_str(), "Helvetica", 22, CCSizeMake(400,0), kCCTextAlignmentLeft);
    lab->setPosition(CCPoint(startX+50, py));
    lab->setColor({81,48,13});
    m_scroll->addChild(lab);
    py+=lab->getContentSize().height;
    float downHeight=py;
    
    CCScale9Sprite* leftbar = CCLoadSprite::createScale9Sprite("anniversay_zhou.png");
    leftbar->setAnchorPoint(CCPoint(0.5, 1.0));
    leftbar->setContentSize(CCSize(27.0, 500.0));
    leftbar->setPosition(startX, py-lineH);
    m_scroll->addChild(leftbar);
    
    CCSprite* leftSpr = CCLoadSprite::createSprite("anniversay_dian.png");
    leftSpr->setPosition(CCPoint(startX, py-lineH));
    m_scroll->addChild(leftSpr);
    py+=dy;
    //时间节点，从第二个开始
    for(int i=1;i<dialogArr->count();i++){
        labStr=getStringByDialog(i);
        picSpr = CCLoadSprite::createSprite(itemPic.c_str());
        CCCommonUtils::setSpriteMaxSize(picSpr,360);
        picSpr->setPosition(CCPoint(startX+50, py));
        m_scroll->addChild(picSpr);
        py+=((picSpr->getContentSize().height)*(picSpr->getScaleY())+5);
        
        lab = CCLabelTTF::create(labStr.c_str(), "Helvetica", 22, CCSizeMake(400,0), kCCTextAlignmentLeft);
        lab->setPosition(CCPoint(startX+50, py));
        lab->setColor({81,48,13});
        m_scroll->addChild(lab);
        py+=lab->getContentSize().height;
        leftSpr = CCLoadSprite::createSprite("anniversay_dian.png");
        leftSpr->setPosition(CCPoint(startX, py-lineH));
        m_scroll->addChild(leftSpr);
        py+=dy;
    }
    ////设置bar长度
    leftbar->setContentSize(CCSize(27.0, py-downHeight+15));
    ////
    //写在最顶上的话
    labStr = _lang_3("150000", playerName.c_str(), playerName.c_str(), playerName.c_str());
    labStr =labStr + "\n" + _lang("150001");
    lab = CCLabelTTF::create(labStr.c_str(), "Helvetica", 20, CCSizeMake(450,0), kCCTextAlignmentLeft);
    lab->setPosition(CCPoint(startX, py));
    lab->setColor({81,48,13});
    m_scroll->addChild(lab);
    py+=lab->getContentSize().height;
    py+=dy;
    
    m_scroll->setContentSize(CCSize(m_scroll->getContentSize().width, py));
    m_scroll->setContentOffset(CCPoint(0,m_scrollNode->getContentSize().height-py));
}

void AnniversaryInfoView::insertIntoArr(double time,int type){
    if (time==0) {
        return;
    }
    time_t insertTime = time/1000;
    for (int i=0; i<timeArr->count(); i++) {
        time_t timeI = dynamic_cast<CCDouble*>(timeArr->objectAtIndex(i))->getValue();
        if (insertTime>timeI) {
            timeArr->insertObject(CCDouble::create(insertTime), i);
            switch (type) {
                case 1:
                    dialogArr->insertObject(CCString::create("150007"),i);
                    break;
                case 2:
                    dialogArr->insertObject(CCString::create("150008"),i);
                    break;
                case 3:
                    dialogArr->insertObject(CCString::create("150010"),i);
                    break;
                case 4:
                    dialogArr->insertObject(CCString::create("150011"),i);
                    break;
                    
                default:
                    break;
            }
            break;
        }
        if (i==timeArr->count()-1) {
            timeArr->insertObject(CCDouble::create(insertTime), i+1);
            switch (type) {
                case 1:
                    dialogArr->insertObject(CCString::create("150007"),i+1);
                    break;
                case 2:
                    dialogArr->insertObject(CCString::create("150008"),i+1);
                    break;
                case 3:
                    dialogArr->insertObject(CCString::create("150010"),i+1);
                    break;
                case 4:
                    dialogArr->insertObject(CCString::create("150011"),i+1);
                    break;
                    
                default:
                    break;
            }
            break;
        }
    }
}

std::string AnniversaryInfoView::getStringByDialog(int index){
    std::string str="";
    string dialogId = dynamic_cast<CCString*>(dialogArr->objectAtIndex(index))->getCString();
    time_t time = dynamic_cast<CCDouble*>(timeArr->objectAtIndex(index))->getValue();
    string timeStr = CCCommonUtils::timeStampToYMD(time);
    if (dialogId=="150002") {
        str=_lang_1("150002", timeStr.c_str());
        itemPic="ico107000.png";//成人礼 士兵
    }else if (dialogId=="150003"){
        str=_lang_1("150003", timeStr.c_str());
        itemPic="ann_pic_xianshihuodong.PNG";//活动
    }else if (dialogId=="150004"){
        str=_lang_1("150004", timeStr.c_str());
        itemPic="ann_pic_shijie.PNG";//世界
    }else if (dialogId=="150005"){
        str=_lang_1("150005", timeStr.c_str());
        itemPic="ann_pic_zhuanpan.PNG";//转盘
    }else if (dialogId=="150006"){
        str=_lang_1("150006", timeStr.c_str());
        itemPic="ann_pic_boss.png";//boss
    }else if (dialogId=="150007"){
        str=_lang_2("150007", timeStr.c_str(), playerName.c_str());
        itemPic="ann_pic_xincheng.PNG";//第一次登录时间
        shareString[0]=str;
    }else if (dialogId=="150008"){
        str=_lang_3("150008", timeStr.c_str(), playerName.c_str(), otherName.c_str());
        itemPic="ann_pic_gongji.PNG";//攻击
        shareString[1]=str;
//    }else if (dialogId=="150009"){
//        str=_lang_1("150009", timeStr.c_str());
//        itemPic="";//
    }else if (dialogId=="150010"){
        str=_lang_2("150010", timeStr.c_str(), playerName.c_str());
        itemPic="ann_pic_beigongji.PNG";//被攻击
        shareString[2]=str;
    }else if (dialogId=="150011"){
        str=_lang_2("150011", timeStr.c_str(), playerName.c_str());
        itemPic="ann_pic_jijie.PNG";//集结
        shareString[3]=str;
    }
    return str;
}


void AnniversaryInfoView::fbShare(CCObject* pSender, CCControlEvent event){
//    if(FBUtilies::fbIsLogin()){
//        CCLOG("login");
//        FBUtilies::fbShareMessage("share123");
//        FBUtilies::fbSharePhotosByUrl("http://cok.eleximg.com/cok/img/fb_share_icon1.png",_lang("150015"));
//    }else{
//        CCLOG("not login");
//    }

    getImgUrl(NULL);
}

void AnniversaryInfoView::AnimationCallback(){
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    string name = getAnimationManager()->getLastCompletedSequenceName();
    if(name == "FadeIn") {
//        setScrollData(NULL);
        m_sprSubBg->setOpacity(150);
        AnniversaryGetCommand* cmd = new AnniversaryGetCommand();
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AnniversaryInfoView::setScrollData), NULL));
        cmd->sendAndRelease();
    }
    if (name == "FadeOut") {
        this->removeFromParentAndCleanup(true);
    }
}

void AnniversaryInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(AnniversaryInfoView::AnimationCallback));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
}
void AnniversaryInfoView::onExit()
{
    setTouchEnabled(false);
//    m_dataArr->release();
    CCNode::onExit();
}

SEL_CCControlHandler AnniversaryInfoView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_MenuHandler AnniversaryInfoView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CallFuncN AnniversaryInfoView::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool AnniversaryInfoView::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeBG", CCNode*,this->m_nodeBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollNode", CCNode*,this->m_scrollNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*,this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headTxt", CCLabelIF*,this->m_headTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprSubBg", CCSprite*,this->m_sprSubBg);
    return false;
}
//ccTouch
bool AnniversaryInfoView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_nodeBG, pTouch)) {
        return false;
    }
    m_scroll->setVisible(!m_scroll->isVisible());
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(AnniversaryInfoView::AnimationCallback));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//    this->removeFromParentAndCleanup(true);
    return true;
}

void AnniversaryInfoView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void AnniversaryInfoView::getImgUrl(cocos2d::CCObject *pObj)
{
    GameController::getInstance()->showWaitInterface();
    string sString=shareString[0]+"|"+shareString[1]+"|"+shareString[2]+"|"+shareString[3];
    CCCommonUtils::StringReplace(sString, "\n", "");
    sString=UrlEncode(sString).c_str();
    HFHttpRequest* pHttpRequest = new HFHttpRequest();
    pHttpRequest->setUrl(ANNIVERSARY_IMAGE_URL);
    pHttpRequest->setRequestType(network::HttpRequest::Type::POST);
    pHttpRequest->setResponseCallback(this, httpresponse_selector_hf(AnniversaryInfoView::onHttpRequestCompleted));
    
    string uid = GlobalData::shared()->playerInfo.uid;
    string strAuthKey = "";
    {
        string kvstr= "";
        kvstr.append("x645rGA7rnG5yOZkGijd");
        kvstr.append("data0commandNameuser/CreateShareImage");
        kvstr.append("params0");
        kvstr.append("gameuid").append(uid);
        kvstr.append("lang");
        kvstr.append(shareLang);
        kvstr.append("text");
        kvstr.append(sString);
        
        MD5 md5(kvstr);
        strAuthKey = md5.toString();
    }
    ::transform(strAuthKey.begin(),strAuthKey.end(), strAuthKey.begin(), ::toupper);

    string data = "{\"data\":[{\"commandName\":\"user/CreateShareImage\",";
    data.append("\"params\":[{");
    data.append("\"lang\":\"").append(shareLang).append("\",");
    data.append("\"text\":\"").append(sString).append("\",");
    data.append("\"gameuid\":").append(uid);
    data.append("}]}],");
    data.append("\"authkey\":\"");
    data.append(strAuthKey).append("\"");
    data.append("}");
    CCLOG("data:%s",data.c_str());
    pHttpRequest->setRequestData(data);
    
    pHttpRequest->setTag("CreateShareImage");
    
    HFHttpClient::getInstance()->send(pHttpRequest);
    pHttpRequest->release();
}

void AnniversaryInfoView::onHttpRequestCompleted(HFHttpClient *pSender, HFHttpResponse *pResponse)
{
    GameController::getInstance()->removeWaitInterface();
    do
    {
        CCAssert(pResponse != NULL, "http response NULL");
        
#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG > 0
        std::vector<char>* headers = pResponse->getResponseHeader();
        string strHeader (headers->begin(),headers->end());
        CCLOGFUNCF("resp headers: %s",strHeader.c_str());
#endif
        CCDictionary* pDictContent = NULL;
        if (pResponse->isSucceed())
        {
            pDictContent=parseResponseData(pResponse->getResponseData());
            string imgUrl = pDictContent->valueForKey("img_url")->getCString();
            CCLOG("imgUrl:%s", imgUrl.c_str());
            FBUtilies::fbSharePhotosByUrl(imgUrl,_lang("150015"));
        }
        else
        {
//            CCCommonUtils::flyHint("", "", _lang("4100004"));头像上传失败，请重试！
        }
        
    } while (0);
}

CCDictionary* AnniversaryInfoView::parseResponseData(vector<char>* response)
{
    CCDictionary* pDictContent = NULL;
    do {
        CC_BREAK_IF(response == NULL);
        string m_sResponseData;
        m_sResponseData.clear();
        m_sResponseData.assign(response->begin(),response->end());
        CCLOG("m_sResponseData:%s", m_sResponseData.c_str());
        if (0 == m_sResponseData.size())
        {
            CCLOG("通信成功，但未返回任何数据。");
            //  通信失败；
            pDictContent = CCDictionary::create();
            CC_BREAK_IF(NULL == pDictContent);
            
            pDictContent->setObject(ccs("-1"), "errorCode");
            pDictContent->setObject(CCString::create("Invalid Response data!"), "errorMsg");
        }
        else
        {
            //  将应答包数据序列化为json对象；
            try
            {
                //todocfb
                Json *jsonObj = Json_create(m_sResponseData.c_str());
                Json *jsonResponse = NULL;
                Json *jsonData = NULL;
                int status = -1;
                if (jsonObj)
                {
                    jsonResponse = Json_getItem(jsonObj, "response");
                }
                if (jsonResponse)
                {
                    jsonData = Json_getItemAt(jsonResponse, 0);
                }
                if (jsonData)
                {
                    status = Json_getInt(jsonData, "status", -1);
                }
                pDictContent = CCDictionary::create();
                if (status == 1)
                {
                    Json *jsonD = Json_getItem(jsonData, "data");
                    string img_url=Json_getString(jsonD, "img_url", "");
                    pDictContent->setObject(CCString::create(img_url.c_str()), "img_url");
                }
                else
                {
                    pDictContent->setObject(ccs("-1"), "errorCode");
                    pDictContent->setObject(CCString::create("Invalid Response data!"), "errorMsg");
                    
                }
                
                Json_dispose(jsonObj);
            } catch (std::invalid_argument &ia)
            {
                CCLOGERROR("parse json error: %s data: %s",ia.what(),m_sResponseData.c_str());
                
                pDictContent = CCDictionary::create();
                CC_BREAK_IF(NULL == pDictContent);
                
                pDictContent->setObject(ccs("-1"), "errorCode");
                pDictContent->setObject(CCString::create("Invalid Response data!"), "errorMsg");
            }
        }
    } while (0);
    return pDictContent;
}

std::string AnniversaryInfoView::UrlEncode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            unsigned char upper = (unsigned char)str[i] >> 4;
            strTemp += upper > 9 ? upper + 55 : upper + 48;
            unsigned char lower = (unsigned char)str[i] % 16;
            strTemp += lower > 9 ? lower + 55 : lower + 48;
        }
    }
    return strTemp;
}
