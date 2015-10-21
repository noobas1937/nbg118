//
//  CCGallery.cpp
//  CrazyBirdTD

//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CCGallery.h"

//#include "CCLuaEngine.h"

#include <math.h>

USING_NS_CC;

CCGallery::CCGallery(void)
: m_pDelegate(NULL)
, m_nGalleryScriptHandler(0)
, m_bEnabled(true)
, m_bIsDragging(false)
, m_nNumItems(0)
, m_nDirection(kCCGalleryDirectionHorizontal)
, m_nDragDirection(kDragDirCenter)
, m_fLeftTopDis(0.0f)
, m_fRightDownDis(0.0f)
, m_pItemsArray(NULL)
, m_tMoveOffset(CCPointZero)
, m_tMoveDistance(CCPointZero)
, m_fFrontScale(1.0f)
, m_fBackScale(1.0f)
, m_bAutoMoveDest(false)
, m_nSelectedItemIdx(0)
{
}

CCGallery::~CCGallery(void)
{
	CC_SAFE_RELEASE(m_pItemsArray);
}

CCGallery* CCGallery::create(CCSize itemSize, CCSize viewSize)
{
	CCGallery *pRet = new CCGallery();
	if (pRet && pRet->init(itemSize, viewSize)) {
		pRet->autorelease();
		return pRet;
	}
	
	CC_SAFE_DELETE(pRet);
	return pRet;
}

bool CCGallery::init(CCSize itemSize, CCSize viewSize)
{
	m_pItemsArray = CCArray::createWithCapacity(32);
	CC_SAFE_RETAIN(m_pItemsArray);
	
	m_tItemSize = itemSize;
	m_tViewSize = viewSize;
	m_fInterspace = itemSize.width;
	m_tCenter = ccp(m_tViewSize.width * 0.5f, m_tViewSize.height * 0.5f);
	
	setDirection(kCCGalleryDirectionHorizontal);
    setCycleMode(kCCGalleryCycleModeCircular);
	return true;
}

void CCGallery::onEnter()
{
	CCLayer::onEnter();
	
	scheduleUpdate();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
//	Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void CCGallery::onExit()
{
	unscheduleUpdate();
    setTouchEnabled(false);
//	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	
	CCLayer::onExit();
}

bool CCGallery::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if (!m_bEnabled || !isVisible()) {
		return false;
	}
	
	for (CCNode *c = this->getParent(); c != NULL; c = c->getParent()) {
		if (c->isVisible() == false) {
			return false;
		}
	}
	
	m_tPreTouchPoint = convertTouchToNodeSpace(pTouch);
	CCPoint screenPos = convertToWorldSpace(m_tPreTouchPoint);
	CCRect viewRect = getViewRect();
	if (viewRect.containsPoint(screenPos)) {
		return true;
	}
	
	return false;
}

void CCGallery::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_nNumItems <= 1) {
		return;
	}
	
	CCPoint touchPoint = convertTouchToNodeSpace(pTouch);
		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// 用户体验，android没有做触摸颤动处理
	if (ccpLengthSQ(ccpSub(touchPoint, m_tPreTouchPoint)) <= 100.0f) {
		return;
	}
#endif
	
	m_bIsDragging = true;
		
//	if (!m_bIsDragging && m_pDelegate) {
//		m_pDelegate->slideBegan(this);
//	}
	
	m_tMoveDistance = ccpSub(touchPoint, m_tPreTouchPoint);
	m_tPreTouchPoint = touchPoint;
	
	if (m_nDirection == kCCGalleryDirectionHorizontal) {
		m_tMoveDistance.y = 0.0f;
		if (m_tMoveDistance.x < 0.0f) {
			m_nDragDirection = kDragDirLeft;
		} else {
			m_nDragDirection = kDragDirRight;
		}
	}
	
	updateItemsPos();
}

void CCGallery::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if (m_bIsDragging) {
		int moveNum = 0;
		if (m_tMoveOffset.x > 0) {
			moveNum = roundf(m_tMoveOffset.x / m_fInterspace + 0.4f);
		} else {
			moveNum = roundf(m_tMoveOffset.x / m_fInterspace - 0.4f);
		}
		m_fAutoMoveDis = moveNum * m_fInterspace - m_tMoveOffset.x;
		m_fAutoMoveOffset = 0.0f;
		if (m_fAutoMoveDis < 0.0f) {
			m_nDragDirection = kDragDirLeft;
		} else {
			m_nDragDirection = kDragDirRight;
		}
		m_bIsDragging = false;
		m_bAutoMoveDest = true;
	} else {
		CCPoint touchPoint = convertTouchToNodeSpace(pTouch);
		CCGalleryItem *pGItem = touchGalleryItems(touchPoint);
		if (pGItem) {
			if (m_pDelegate) {
				m_pDelegate->selectionDecided(this, pGItem);
			}
			
//			CCLuaEngine::defaultEngine()->executeGalleryEvent(this, pGItem, kCCGalleryEventDecided);
		}
	}
}

void CCGallery::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	CCRect frame = getViewRect();
	glEnable(GL_SCISSOR_TEST);
	Director::getInstance()->getOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
	
	Layer::visit(renderer,parentTransform,parentFlags);

	glDisable(GL_SCISSOR_TEST);
}

void CCGallery::addChild(CCNode *child)
{
	CCGalleryItem *pGItem = CCGalleryItem::create(m_nNumItems);
	m_nNumItems++;
	child->setTag(1);
	pGItem->addChild(child);
	
	CCPoint itemPos = ccpAdd(m_tCenter, m_tMoveOffset);
	if (m_nDirection == kCCGalleryDirectionHorizontal) {
		if (m_nDragDirection == kDragDirCenter) {
			pGItem->setPosition(m_tCenter);
			
			m_fLeftTopDis = m_fLeftTopDis + m_fInterspace;
			m_fRightDownDis = m_fRightDownDis + m_fInterspace;
			
			m_nDragDirection = kDragDirRight;
		} else if (m_nDragDirection == kDragDirLeft) {
			itemPos = ccpSub(itemPos, ccp(m_fLeftTopDis, 0.0f));
			
			m_fLeftTopDis = m_fLeftTopDis + m_fInterspace;
			m_nDragDirection = kDragDirRight;
		} else if (m_nDragDirection == kDragDirRight) {
			itemPos = ccpAdd(itemPos, ccp(m_fRightDownDis, 0.0f));
			
			m_fRightDownDis = m_fRightDownDis + m_fInterspace;
//			m_nDragDirection = kDragDirLeft;
		}
	}
	pGItem->setPosition(itemPos);
	
	CCLayer::addChild(pGItem);
	
	m_pItemsArray->addObject(pGItem);
}

void CCGallery::addChildFinish()
{
	updateItemsPos();
}

void CCGallery::setTargetIndexItem(int idx, bool animated/* = false*/)
{
	int subIdx = m_nSelectedItemIdx - idx;
	
	for (int i = 0; i < abs(subIdx); i++) {
		if (subIdx > 0) {
			m_nDragDirection = kDragDirRight;
			m_tMoveDistance.x = m_fInterspace;
		} else {
			m_nDragDirection = kDragDirLeft;
			m_tMoveDistance.x = -m_fInterspace;
		}
		
		updateItemsPos();
	}
	
	m_nSelectedItemIdx = idx;
    if (m_pDelegate) {
        m_pDelegate->selectionChanged(this, (CCGalleryItem *)getChildByTag(m_nSelectedItemIdx));
    }
}

CCGalleryItem* CCGallery::itemAtIndex(int idx)
{
	CCObject *pItemObj = NULL;
	CCARRAY_FOREACH(m_pItemsArray, pItemObj) {
		CCGalleryItem *pGItem = (CCGalleryItem *) pItemObj;
		if (pGItem->getIdx() == idx) {
			return pGItem;
		}
	}
	
	return NULL;
}

void CCGallery::setDelegate(CCGalleryDelegate *delegate)
{
	m_pDelegate = delegate;
}

void CCGallery::setDirection(int dir)
{
	m_nDirection = dir;
}

void CCGallery::setCycleMode(int mode)
{
    m_nCycleMode = mode;
}

void CCGallery::setItemSpace(float space)
{
	m_fInterspace = space;
}

void CCGallery::setFrontScale(float scale)
{
	m_fFrontScale = scale;
}

void CCGallery::setBackScale(float scale)
{
	m_fBackScale = scale;
}

void CCGallery::registerGalleryScriptHandler(int nFunID)
{
	m_nGalleryScriptHandler = nFunID;
}

void CCGallery::unregisterGalleryScriptHandler()
{
	m_nGalleryScriptHandler = 0;
}

int CCGallery::getGalleryScriptHandler()
{
	return m_nGalleryScriptHandler;
}

void CCGallery::updateItemsPos(void)
{
    if (m_nCycleMode == kCCGalleryCycleModeNotCircular) {
        if (m_nDragDirection == kDragDirLeft || m_nDragDirection == kDragDirUp) {
            if (m_fRightDownDis <= m_fInterspace && (m_tMoveOffset.x <= 0 && m_tMoveDistance.x <= 0)) {
                return;
            }
        } else if (m_nDragDirection == kDragDirRight || m_nDragDirection == kDragDirDown) {
            if (m_fLeftTopDis <= m_fInterspace && (m_tMoveOffset.x >= 0 && m_tMoveDistance.x >= 0)) {
                return;
            }
        }
    }
    m_tMoveOffset = ccpAdd(m_tMoveOffset, m_tMoveDistance);
    
	if (m_nDragDirection == kDragDirLeft && m_tMoveOffset.x <= -m_fInterspace) {
		m_tMoveOffset.x = m_tMoveOffset.x + m_fInterspace;
		
		m_fLeftTopDis = m_fLeftTopDis + m_fInterspace;
		m_fRightDownDis = m_fRightDownDis - m_fInterspace;
	} else if (m_nDragDirection == kDragDirRight && m_tMoveOffset.x >= m_fInterspace) {
		m_tMoveOffset.x = m_tMoveOffset.x - m_fInterspace;
		
		m_fLeftTopDis = m_fLeftTopDis - m_fInterspace;
		m_fRightDownDis = m_fRightDownDis + m_fInterspace;
	}

    Vector<Node*>& children = getChildren();
    for (auto child : children)
    {
        CCGalleryItem *pGItem = (CCGalleryItem *) child;
        CCPoint itemPos = ccpAdd(pGItem->getPosition(), m_tMoveDistance);
        pGItem->setPosition(itemPos);
        
        float distance = m_fInterspace - fabsf(pGItem->getPosition().x - m_tCenter.x);
        if (distance < 0.0f) {
            distance = 0.0f;
        }
        float scale = (distance / m_fInterspace) * (m_fFrontScale - m_fBackScale) + m_fBackScale;
        pGItem->setScale(scale);
    }

    if (m_nCycleMode == kCCGalleryCycleModeCircular) {
        if (m_nDragDirection == kDragDirLeft || m_nDragDirection == kDragDirUp) {
            CCGalleryItem *pGItem = (CCGalleryItem *) m_pItemsArray->objectAtIndex(0);
            if (updateItemStatus(pGItem)) {
                m_pItemsArray->removeObjectAtIndex(0);
                m_pItemsArray->addObject(pGItem);
            }
        } else if (m_nDragDirection == kDragDirRight || m_nDragDirection == kDragDirDown) {
            CCGalleryItem *pGItem = (CCGalleryItem *) m_pItemsArray->lastObject();
            if (updateItemStatus(pGItem)) {
                m_pItemsArray->removeLastObject();
                m_pItemsArray->insertObject(pGItem, 0);
            }
        }
    }


    for (auto child : children)
    {
        CCGalleryItem *pGItem = (CCGalleryItem *) child;
        
        float scale = pGItem->getScale();
        CCPoint itemPos = pGItem->getPosition();
        
        float width = m_fInterspace * 0.5f * scale;
        if (m_nDirection == kCCGalleryDirectionHorizontal) {
            if ((itemPos.x + width) < 0.0f || (itemPos.x - width) > m_tViewSize.width) {
                pGItem->setVisible(false);
            } else {
                pGItem->setVisible(true);
            }
        }
    }
	
//	if (m_pDelegate) {
//		m_pDelegate->selectionChanged(this, m_nSelectIdx);
//	}
}

bool CCGallery::updateItemStatus(CCGalleryItem *pGItem)
{
	bool isNeedChange = false;
	
	float scale = pGItem->getScale();
	CCPoint itemPos = pGItem->getPosition();
	
	float width = m_fInterspace * 0.5f * scale;
	if (m_nDirection == kCCGalleryDirectionHorizontal) {
		if (m_nDragDirection == kDragDirLeft && (itemPos.x + width) < 0.0f) {
			isNeedChange = true;
			
			itemPos = ccpAdd(m_tCenter, m_tMoveOffset);
			itemPos = ccpAdd(itemPos, ccp(m_fRightDownDis, 0.0f));
			pGItem->setPosition(itemPos);
			
			m_fRightDownDis = m_fRightDownDis + m_fInterspace;
			
			m_fLeftTopDis = m_fLeftTopDis - m_fInterspace;
		} else if (m_nDragDirection == kDragDirRight && (itemPos.x - width) > m_tViewSize.width) {
			isNeedChange = true;
			
			itemPos = ccpAdd(m_tCenter, m_tMoveOffset);
			itemPos = ccpSub(itemPos, ccp(m_fLeftTopDis, 0.0f));
			pGItem->setPosition(itemPos);
			
			m_fLeftTopDis = m_fLeftTopDis + m_fInterspace;
			
			m_fRightDownDis = m_fRightDownDis - m_fInterspace;
		}
	}
	
	return isNeedChange;
}

void CCGallery::update(float delta)
{
	if (!m_bAutoMoveDest) {
		return;
	}
	
	m_tMoveDistance.x = m_fAutoMoveDis * delta * 4.0f;
	float offset = m_fAutoMoveOffset + m_tMoveDistance.x;
	if (fabsf(offset) >= fabsf(m_fAutoMoveDis)) {
		m_bAutoMoveDest = false;
		
		m_tMoveDistance.x = m_fAutoMoveDis - m_fAutoMoveOffset;
		
		if (m_pDelegate) {
//			m_pDelegate->slideEnded(this, m_nSelectIdx);
		}
		
//		CCObject *pItemObj = NULL;
//		CCArray *pItemsArray = getChildren();
        Vector<Node*>& children = getChildren();
        for (auto child : children)
        {
            CCGalleryItem *pGItem = (CCGalleryItem *) child;
            CCPoint itemPos = pGItem->getPosition();
            itemPos = ccpSub(itemPos, m_tCenter);
            if (fabsf(itemPos.x) < m_tItemSize.width * 0.5f) {
                m_nSelectedItemIdx = pGItem->getIdx();
                if (m_pDelegate) {
                    m_pDelegate->selectionChanged(this, pGItem);
                }
//                CCLuaEngine::defaultEngine()->executeGalleryEvent(this, pGItem, kCCGalleryEventEnded);
                break;
            }
        }
//		CCARRAY_FOREACH(pItemsArray, pItemObj) {
//			CCGalleryItem *pGItem = (CCGalleryItem *) pItemObj;
//			CCPoint itemPos = pGItem->getPosition();
//			itemPos = ccpSub(itemPos, m_tCenter);
//			if (fabsf(itemPos.x) < m_tItemSize.width * 0.5f) {
//				m_nSelectedItemIdx = pGItem->getIdx();
//				CCLuaEngine::defaultEngine()->executeGalleryEvent(this, pGItem, kCCGalleryEventEnded);
//				break;
//			}
//		}
	}
	
	m_fAutoMoveOffset = offset;
	
	updateItemsPos();
}

CCGalleryItem* CCGallery::touchGalleryItems(const CCPoint& touchPoint)
{
//	CCObject *pItemObj = NULL;
//	CCArray *pItemsArray = getChildren();
//	CCARRAY_FOREACH(pItemsArray, pItemObj) {
//		CCGalleryItem *pGItem = (CCGalleryItem *) pItemObj;
//		CCPoint itemPos = pGItem->getPosition();
//		CCRect itemRect = CCRectMake(itemPos.x - m_tItemSize.width * 0.5f, itemPos.y - m_tItemSize.height * 0.5f,
//									 m_tItemSize.width, m_tItemSize.height);
//		if (itemRect.containsPoint(touchPoint)) {
//			return pGItem;
//		}
//	}
    Vector<Node*>& children = getChildren();
    for (auto child : children)
    {
        CCGalleryItem *pGItem = (CCGalleryItem *) child;
        CCPoint itemPos = pGItem->getPosition();
        CCRect itemRect = CCRectMake(itemPos.x - m_tItemSize.width * 0.5f, itemPos.y - m_tItemSize.height * 0.5f,
                                     m_tItemSize.width, m_tItemSize.height);
        if (itemRect.containsPoint(touchPoint)) {
            return pGItem;
        }
    }
	
	return NULL;
}

CCRect CCGallery::getViewRect()
{
	CCPoint screenPos = convertToWorldSpace(CCPointZero);
	
	float scaleX = getScaleX();
	float scaleY = getScaleY();

	for (CCNode *p = getParent(); p != NULL; p = p->getParent()) {
		scaleX *= p->getScaleX();
		scaleY *= p->getScaleY();
	}
	
	if(scaleX < 0.f) {
		screenPos.x += m_tViewSize.width * scaleX;
		scaleX = -scaleX;
	}
	if(scaleY < 0.f) {
		screenPos.y += m_tViewSize.height * scaleY;
		scaleY = -scaleY;
	}

	return CCRectMake(screenPos.x, screenPos.y, m_tViewSize.width * scaleX, m_tViewSize.height * scaleY);
}




