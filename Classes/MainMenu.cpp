#include "MainMenu.h"
#include "GameLayer.h"
#include "Global.h"
#include "SelectMenu.h"
#include "AboutMenu.h"
#include "SetMusic.h"

CCMainMenu::CCMainMenu()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite *pBackground = CCSprite::create("background.png");
	pBackground->setPosition(ccp(winSize.width/2, winSize.height/2));
	this->addChild(pBackground);
	
	pStartGame = CCMenuItemImage::create("startgame_normal.png", "startgame_select.png",
		this, menu_selector(CCMainMenu::menuCallBackForStartGame));
	pStartGame->setPosition(ccp(winSize.width/2, winSize.height/2));

	pQuit = CCMenuItemImage::create("quitgame_normal.png", "quitgame_select.png",
		this, menu_selector(CCMainMenu::menuQuit));
	pQuit->setPosition(ccp(winSize.width/2, winSize.height/2 - 40));

	pAbout = CCMenuItemImage::create("about_normal.png", "about_select.png",
		this, menu_selector(CCMainMenu::menuCallBackForAbout));
	pAbout->setPosition(ccp(winSize.width - 50, 20));

	pSetting = CCMenuItemImage::create("Setting_n.png", "setting_s.png",
		this, menu_selector(CCMainMenu::menuSetting));
	pSetting->setPosition(ccp(winSize.width/2, winSize.height/2 - 80));

	pMenu = CCMenu::create(pStartGame, pAbout, pQuit, pSetting, NULL);
	pMenu->setPosition(ccp(0, 0));
	this->addChild(pMenu, 0);
}

CCMainMenu::~CCMainMenu()
{
	this->unscheduleAllSelectors();
}

bool CCMainMenu::init()
{
	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	return true;
}


void CCMainMenu::menuCallBackForStartGame(CCObject *pSender)
{
	CCGlobal::getGlobalInstance()->setCurrentLifeNum(3);
	//CCGlobal::getGlobalInstance()->reSetLevel();  // 关卡重置为第一关

	//CCGameLayer *pGameLayer = CCGameLayer::create();
	CCSelectMenu *pSelectMenu = CCSelectMenu::create();
	CCScene *pScene = CCScene::create();
	pScene->addChild(pSelectMenu);
	CCDirector::sharedDirector()->replaceScene(pScene);
	//CCGameScene *pGameScene =  CCGameScene::getGameSceneInstance();
	//pGameScene->removeAllChildrenWithCleanup(true);

	//pGameScene->addChild(pSelectMenu);
}

void CCMainMenu::menuCallBackForAbout(CCObject *pSender)
{
	CCAbout *pAbout = CCAbout::create();
	CCScene *pScene = CCScene::create();
	pScene->addChild(pAbout);
	CCDirector::sharedDirector()->replaceScene(pScene);
}

bool CCMainMenu::ccTouchBegan(CCTouch *touch, CCEvent * pEvent)
{
	return true;
}

void CCMainMenu::ccTouchEnded(CCTouch *touch, CCEvent * pEvent)
{

}

void CCMainMenu::ccTouchCancelled(CCTouch *touch, CCEvent *pEvent)
{

}

void CCMainMenu::ccTouchMoved(CCTouch *touch, CCEvent *pEvent)
{

}

void CCMainMenu::menuQuit( CCObject *pSender )
{
	CCDirector::sharedDirector()->end();
}

void CCMainMenu::menuSetting( CCObject *pSender )
{
	CCScene *pScene = CCScene::create();
	pScene->addChild(CCSetMusic::create());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

