//
//  cocosextV3.h
//  MyCppGame
//
//  Created by 舒 圣林 on 14-4-2.
//
//

#ifndef MyCppGame_cocosextV3_h
#define MyCppGame_cocosextV3_h

#include "extensions/cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"
#include <editor-support/spine/spine-cocos2dx.h>
#include <network/HttpClient.h>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocosbuilder;

typedef ControlButton CCControlButton;
typedef Control::EventType CCControlEvent;

typedef spine::SkeletonAnimation CCSkeletonAnimation;
typedef NotificationObserver CCNotificationObserver;
typedef SAXDelegator CCSAXDelegator;
typedef NodeLoaderLibrary CCNodeLoaderLibrary;
typedef NodeLoader CCNodeLoader;
typedef LayerLoader CCLayerLoader;
typedef NodeLoaderListener CCNodeLoaderListener;
typedef cocos2d::extension::Control::Handler SEL_CCControlHandler;
typedef DictElement CCDictElement;
typedef Scale9Sprite CCScale9Sprite;
typedef TableViewDelegate CCTableViewDelegate;
typedef Device CCDevice;
typedef TableViewDataSource CCTableViewDataSource;
typedef ScrollView CCScrollView;
typedef ScrollView::Direction CCScrollViewDirection;
typedef TableViewCell CCTableViewCell;
typedef TableView CCTableView;
typedef EditBox CCEditBox;
typedef EditBoxDelegate CCEditBoxDelegate;
typedef EditBox::InputMode EditBoxInputMode;
typedef ScrollViewDelegate CCScrollViewDelegate;

//typedef EditBox::KeyboardReturnType KeyboardReturnType;
typedef cocos2d::network::HttpResponse CCHttpResponse;
typedef cocos2d::network::HttpClient CCHttpClient;
typedef cocos2d::network::HttpRequest CCHttpRequest;
const Control::EventType CCControlEventTouchUpInside = Control::EventType::TOUCH_UP_INSIDE;
const Control::EventType CCControlEventTouchDown = Control::EventType::TOUCH_DOWN;

const Control::State CCControlStateNormal       = Control::State::NORMAL;
const Control::State CCControlStateHighlighted  = Control::State::HIGH_LIGHTED;
const Control::State CCControlStateDisabled     = Control::State::DISABLED;
const Control::State CCControlStateSelected     = Control::State::SELECTED;

const ScrollView::Direction kCCScrollViewDirectionNone = ScrollView::Direction::NONE ;
const ScrollView::Direction kCCScrollViewDirectionHorizontal = ScrollView::Direction::HORIZONTAL;
const ScrollView::Direction kCCScrollViewDirectionVertical = ScrollView::Direction::VERTICAL ;
const ScrollView::Direction kCCScrollViewDirectionBoth = ScrollView::Direction::BOTH ;
const TableView::VerticalFillOrder kCCTableViewFillTopDown = TableView::VerticalFillOrder::TOP_DOWN;
const TableView::VerticalFillOrder  kCCTableViewFillBottomUp = TableView::VerticalFillOrder::BOTTOM_UP;
const EditBox::InputMode kEditBoxInputModeNumeric = EditBox::InputMode::NUMERIC;
const EditBox::KeyboardReturnType kKeyboardReturnTypeDefault = EditBox::KeyboardReturnType::DEFAULT;
const EditBox::KeyboardReturnType kKeyboardReturnTypeSend = EditBox::KeyboardReturnType::SEND;
const EditBox::KeyboardReturnType kKeyboardReturnTypeDone = EditBox::KeyboardReturnType::DONE;
const EditBox::InputFlag kEditBoxInputFlagInitialCapsSentence = EditBox::InputFlag::INITIAL_CAPS_SENTENCE;

const Control::EventType CCControlEventValueChanged = Control::EventType::VALUE_CHANGED;
#define TOUCH_EVENT_BEGIN onTouchBegan
#define TOUCH_EVENT_MOVED onTouchMoved
#define TOUCH_EVENT_ENDED onTouchEnded
#define TOUCH_EVENT_CANCELLED onTouchCancelled
#define TOUCHES_EVENT_BEGIN onTouchesBegan
#define TOUCHES_EVENT_MOVED onTouchesMoved
#define TOUCHES_EVENT_END onTouchesEnded
#define TOUCHES_EVENT_CANCELLED onTouchesCancelled
#define VISIT_DECL virtual void visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
#define VISIT_IMPL(CLASS) void CLASS::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
#define DRAW_DECL virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
#define DRAW_IMPL(CLASS) void CLASS::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
#define CCStringMake StringMake
#define kCCRepeatForever kRepeatForever

#define VISIT_DECL virtual void visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
#define VISIT_IMPL(CLASS) void CLASS::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
#define DRAW_DECL virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
#define DRAW_IMPL(CLASS) void CLASS::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)

#define HFNODEVECTOR_FOREACH(vector, _iterator)                                                                         \
for (Vector<Node*>::iterator _iterator = vector.begin(); _iterator != vector.end(); _iterator ++)
#endif
