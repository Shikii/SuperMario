#ifndef _SELECTMENU_H_
#define _SELECTMENU_H_

#include "cocos2d.h"
using namespace cocos2d;

class CCSelectMenu : public cocos2d::CCLayer
{
public:
	virtual bool init();

	void menuCloseCallback(CCObject* pSender);

	CREATE_FUNC(CCSelectMenu);

public:
	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	void menuBegin(CCObject *pSender);
	void menuBack(CCObject *pSender);
private:
	//根据手势滑动的距离和方向滚动图层
	void adjustView(float offset);
	CCPoint m_touchPoint;
	int m_nCurPage;
	CCLayer *pLayer;
	CCLabelTTF *m_pLevel;
	CCMenuItemImage *pNewGame;
};


#endif