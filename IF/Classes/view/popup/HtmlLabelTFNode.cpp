//
//  HtmlLabelTFNode.cpp
//  IF
//
//  Created by lifangkai on 14-11-13.
//
//

#include "HtmlLabelTFNode.h"
#include "Utf8Utils.h"

HtmlLabelTFNode* HtmlLabelTFNode::create(string str,int fontSize,float wd){
    HtmlLabelTFNode* ret = new HtmlLabelTFNode(str,fontSize,wd);
    if(ret&&ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool HtmlLabelTFNode::init(){
    CCLayer::init();
    this->setAnchorPoint(CCPoint(0, 0));
    m_textNode= CCNode::create();
    this->addChild(m_textNode);
    return true;
}
void HtmlLabelTFNode::onEnter(){
    CCLayer::onEnter();
    
}

void HtmlLabelTFNode::onExit(){
    CCLayer::onExit();
}
bool HtmlLabelTFNode::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    if(isTouchInside(this, pTouch)){
        return true;
    }
    return false;
}
void HtmlLabelTFNode::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    if(isTouchInside(this, pTouch)){
        
    }
    
}
void HtmlLabelTFNode::setTextStr(string charStr){
    m_str = charStr;
    string str = charStr;
    this->m_textNode->removeAllChildren();
    m_textNode->setPositionY(0);
    m_VectorStr.clear();
    m_showVectorStr.clear();
    CCLabelTTF *label = CCLabelTTF::create( "sample", "Helvetica", mFontSize);
    label->setAnchorPoint(ccp(0, 0));
    int len = Utf8Utils::strlen(str.c_str());
    int i=1;
    float dh =label->getContentSize().height;
    float dy = -dh;
    bool isextend = false;
    bool isHtml = false;
    int posStart =-1;
    int posMiddle = -1;
    int posEnd = -1;
    string tempshow = "";
    string tempStr = "";
    while (i<=len) {
        tempStr = Utf8Utils::utf8sub(str.c_str(),0,i);
        string oneStr =  Utf8Utils::utf8sub(str.c_str(),i-1,1);
        if(oneStr=="\n"){
            
            tempStr=Utf8Utils::utf8sub(str.c_str(),0,i-1);
            m_VectorStr.push_back(tempStr);
            if(posEnd!=-1){
                posEnd= tempshow.find("</a>");
                string showTempStr = "";
                int totalLen = tempshow.length();
                showTempStr = tempshow.substr(0,posEnd);
                showTempStr+=tempshow.substr(posEnd+4,totalLen-(posEnd+3));
                tempshow =showTempStr;
                posEnd = -1;
            }
            m_showVectorStr.push_back(tempshow);
            str =Utf8Utils::utf8sub(str.c_str(),i,len-(i));
            len =Utf8Utils::strlen(str.c_str());
            isextend = true;
            tempshow="";
            isHtml = false;
            i=0;
        }else{
            //处理是否带超链接的文本
            if(oneStr=="<"&&(!isHtml)){
                string htmlFlagStr =  Utf8Utils::utf8sub(str.c_str(),i-1,9);
                if(htmlFlagStr=="<a href=\""){//
                    posStart = str.find("<a href=\"");
                    posMiddle = str.find("\">");
                    posEnd= str.find("</a>");
                    if(posStart!=-1&&posMiddle!=-1&&posEnd!=-1){
                        if(posMiddle>posStart&&posEnd>posMiddle){
                            isHtml = true;
                            isextend = true;
                            tempshow = Utf8Utils::utf8sub(str.c_str(),0,i-1);
                        }
                    }
                    
                }
            }
            if(isHtml){
                int realLen = tempStr.length();
                if(realLen>(posMiddle+1)&&realLen<posEnd){
                    tempshow+=Utf8Utils::utf8sub(str.c_str(),i,1);

                }else if(realLen>posEnd+3){
                    tempshow+=Utf8Utils::utf8sub(str.c_str(),i,1);
                    posEnd = -1;
                }
            }else{
                tempshow =tempStr;
            }
            
            label->setString(tempshow.c_str());
            if(label->getContentSize().width>m_width){
                isHtml = false;
                tempStr=Utf8Utils::utf8sub(str.c_str(),0,i);
//                label->setString(tempStr.c_str());
//                m_textNode->addChild(label);
//                label->setPositionY(dy);
//                dy-=dh;
                str =Utf8Utils::utf8sub(str.c_str(),i-1,len-(i-1));
                len =Utf8Utils::strlen(str.c_str());
                m_VectorStr.push_back(tempStr);
                int len1= Utf8Utils::strlen(tempshow.c_str());;
                string str1=Utf8Utils::utf8sub(tempshow.c_str(),0,len1-1);
                m_showVectorStr.push_back(str1);
                i=0;
            }
        }
        i++;
    }
    if(str!=""){
        m_VectorStr.push_back(str);
    }
    if(tempshow!=""){
        if(posEnd!=-1){
            posEnd= tempshow.find("</a>");
            string showTempStr = "";
            int totalLen = tempshow.length();
            showTempStr = tempshow.substr(0,posEnd);
            showTempStr+=tempshow.substr(posEnd+4,totalLen-(posEnd+3));
            tempshow =showTempStr;
        }
          m_showVectorStr.push_back(tempshow);
    }
    
    showText();
}
void HtmlLabelTFNode::showText(){
    int count = m_VectorStr.size();
    int i=0;
    float dy = 0;
    while (i<count) {
        CCLabelTTF *label = CCLabelTTF::create( "sample", "Helvetica", mFontSize);
        label->setAnchorPoint(ccp(0, 0));
        label->setString(m_showVectorStr[i].c_str());
        label->setColor(ccc3(56, 8, 0));
        float dh =label->getContentSize().height;
        m_textNode->addChild(label);
        label->setTag(i);
        dy-=dh;
        label->setPositionY(dy);
        i++;
    }
    m_textNode->setPositionY(-dy);
    m_height = -dy;
    showHtmlLine();

}
void HtmlLabelTFNode::showHtmlLine(){
    CCLabelTTF *label = CCLabelTTF::create( "sample", "Helvetica", mFontSize);
    label->setAnchorPoint(ccp(0, 0));
    float hintdh = label->getContentSize().height;
    int count = m_VectorStr.size();
    int i=0;
    float dy = 0;
    bool isMutiple = false;
    float width = 0;
    float dx = 0;
    while (i<count) {
        string tempStr = m_VectorStr[i];
        if(isMutiple){
            int posSecondEnd = tempStr.find("\"");
            if(posSecondEnd!=-1){
                string tempHtmlStr1 =tempStr.substr(0,posSecondEnd);
                label->setString(tempHtmlStr1.c_str());
                width=label->getContentSize().width;
            }else{
                width=m_width;
            }
            auto lineSpr = CCLoadSprite::createScale9Sprite("Mail_line_text.png");
            m_textNode->addChild(lineSpr);
            auto text = m_textNode->getChildByTag(i);
            lineSpr->setPosition(0,text->getPositionY());
            lineSpr->setAnchorPoint(ccp(0, 0.5));
            lineSpr->setContentSize(CCSize(width, lineSpr->getContentSize().height));
            
            auto hintBG = HtmlBGTouchLayer::create(CCSize(width, hintdh), m_url);
            m_textNode->addChild(hintBG);
            hintBG->setPosition(0,text->getPositionY());
        }
        width = 0;
        m_url="";
        int posStart = tempStr.find("<a href=\"");
        int posMiddle = tempStr.find("\">");
        int posEnd = tempStr.find("</a>",posStart+1);
        if(posStart!=-1&&posMiddle>posStart){
 
            string tempHtmlStr =tempStr.substr(0,posStart);
            label->setString(tempHtmlStr.c_str());
            dx =label->getContentSize().width;
            m_url=tempStr.substr(posStart+8+1,posMiddle-(posStart+8+1));
            if(posEnd!=-1){
                tempHtmlStr =tempStr.substr(posMiddle+1+1,posEnd-(posMiddle+1+1));
                
                label->setString(tempHtmlStr.c_str());
                width = label->getContentSize().width;
                isMutiple = false;
                addUnderLine(i,dx,width,hintdh);
            }else{
                width = m_width-dx;

                addUnderLine(i,dx,width,hintdh);
                int j=i+1;
                while (j<count) {
                    string nextTempStr = m_VectorStr[j];
                    int posnextEnd = nextTempStr.find("</a>");
                    if(posnextEnd!=-1){
                        string tempHtmlStr1 =tempStr.substr(0,posnextEnd);
                        label->setString(tempHtmlStr1.c_str());
                        width=label->getContentSize().width;
                        addUnderLine(j,0,width,hintdh);
                        i=j-1;
                        break;
                    }else{
                        addUnderLine(j,0,m_width,hintdh);
                    }
                    
                    j++;
                }
            }
            
            
        }
        i++;
    }

}
void HtmlLabelTFNode::addUnderLine(int index,float dx,float wd,float dh){

    auto lineSpr = CCLoadSprite::createScale9Sprite("Mail_line_text.png");
    m_textNode->addChild(lineSpr);
    auto text = m_textNode->getChildByTag(index);
    lineSpr->setPosition(dx,text->getPositionY());
    lineSpr->setAnchorPoint(ccp(0, 0.5));
    lineSpr->setContentSize(CCSize(wd, lineSpr->getContentSize().height));
    
    auto hintBG = HtmlBGTouchLayer::create(CCSize(wd, dh), m_url);
    m_textNode->addChild(hintBG);
    hintBG->setPosition(dx,text->getPositionY());
}
CCSize HtmlLabelTFNode::getContentSize(){

    return CCSize(m_width, m_height);
}
CCSize HtmlLabelTFNode::getTextDimension(const char *text)
{
    int len = strlen(text);
    if ((len==0)  ||  (len==1 && text[0] == '\n')) {
        CCLabelTTF *label = CCLabelTTF::create( "sample" , "Helvetica", mFontSize );
        CCSize size = label->getContentSize();
        size.width = 0;
        return size;
    }
    CCLabelTTF *label = CCLabelTTF::create( text, "Helvetica", mFontSize);
	return label->getContentSize();
}
//点击区域class

HtmlBGTouchLayer* HtmlBGTouchLayer::create(CCSize size,string url){
    HtmlBGTouchLayer* ret = new HtmlBGTouchLayer(size,url);
    if(ret&&ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool HtmlBGTouchLayer::init(){
    CCLayer::init();
    this->setAnchorPoint(CCPoint(0, 0));
    this->setContentSize(m_size);
    return true;
}
void HtmlBGTouchLayer::onEnter(){
    CCLayer::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -3, false);
    
}

void HtmlBGTouchLayer::onExit(){
    setTouchEnabled(false);
    CCLayer::onExit();
}
bool HtmlBGTouchLayer::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    if(isTouchInside(this, pTouch)){
        m_startPoint = pTouch->getStartLocation();
        return true;
    }
    return false;
}
void HtmlBGTouchLayer::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if((fabs(pTouch->getLocation().x-m_startPoint.x)>10)||
        (fabs(pTouch->getLocation().y-m_startPoint.y)>10)){
        return;
    }
    if(isTouchInside(this, pTouch)){
        if(m_url!=""){
            GameController::getInstance()->goTurntoUrl(m_url);
        }
    }
    
}