//
//  NBProgressBar.h
//  clipTest
//
//  Created by fusheng on 15-10-8.
//
//

#ifndef __clipTest__NBProgressBar__
#define __clipTest__NBProgressBar__

#include <stdio.h>
#include "cocos2d.h"

class NBProgressBar : public cocos2d::Node
{
public:
    virtual bool init(cocos2d::Node* pProgressBar,cocos2d::Node* pMaskNode,float percent,float offsetX ,float alphaThreshold,float minMNValue ,float maxMNValue);
    
    NBProgressBar():m_pProgressBar(nullptr),m_pMaskNode(nullptr),m_pClippingNode(nullptr),m_percent(1),m_minMNValue(-10086),m_maxMNValue(-10086)
    {}
    
    /*************************************************
     Description:    生成一个进度条，主要是为了显示非矩形，扇形进度。
     Input:         pProgressBar 进度条的显示部分，
                    pMaskNode 负责剪裁的Node ，
                    percent 百分比（0-1），
                    minMNValue 剪裁Node的位置最小值 使用默认值时 值为pProgressBar->getPositionX() ，
                    maxMNValue 剪裁Node的位置最大值 使用默认值时 值为pProgressBar->getPositionX()+pProgressBar->getContentSize().width:maxMNValue
    *************************************************/
    static NBProgressBar* create(cocos2d::Node* pProgressBar,cocos2d::Node* pMaskNode,float percent = 1,float offsetX = 0,float alphaThreshold = 0.34 ,float minMNValue = -10086,float maxMNValue = -10086);

    void setPercent(float percent);
    
    float getPercent();
    
    void refresh();
    
    cocos2d::ClippingNode* m_pClippingNode;
private:
    cocos2d::Node* m_pProgressBar;
    cocos2d::Node* m_pMaskNode;
    
    float m_percent;
    float m_minMNValue;
    float m_maxMNValue;
    float offsetX;
    
    // implement the "static create()" method manually
//    CREATE_FUNC(NBProgressBar);
};
#endif /* defined(__clipTest__NBProgressBar__) */
