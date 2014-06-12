#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "cocos2d.h"
using namespace cocos2d;

class CCMainMenu : public CCLayer
{
public:
	CCMainMenu();
	~CCMainMenu();

	CCMenuItem *pStartGame;
	CCMenuItem *pQuit;
	CCMenuItem *pAbout;
	CCMenuItem *pSetting;

	CCMenu *pMenu;

	void menuCallBackForStartGame(CCObject *pSender);
	void menuCallBackForAbout(CCObject *pSender);
	void menuQuit(CCObject *pSender);
	void menuSetting(CCObject *pSender);

	bool init();

    bool ccTouchBegan(CCTouch *touch, CCEvent * pEvent);
    void ccTouchEnded(CCTouch *touch, CCEvent * pEvent);
    void ccTouchCancelled(CCTouch *touch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *touch, CCEvent *pEvent);

	CREATE_FUNC(CCMainMenu);

private:

};


#endif