#include "AboutMenu.h"
#include "MainMenu.h"

CCAbout::CCAbout()
{

}

CCAbout::~CCAbout()
{

}

bool CCAbout::init()
{
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCSprite *pBg = CCSprite::create("bg.png");
	pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	this->addChild(pBg, 0);

	CCSprite *pZhy = CCSprite::create("zhy.jpg");
	pZhy->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 + 50));
	this->addChild(pZhy, 2);

	CCSprite *pWxb = CCSprite::create("wxb.jpg");
	pWxb->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 - 50));
	this->addChild(pWxb, 2);

	CCMenuItemImage *pBack = CCMenuItemImage::create("backA.png", "backB.png",
		this, menu_selector(CCAbout::menuBack));
	pBack->setPosition(ccp(origin.x + 20, origin.y + visibleSize.height - 20));
	CCMenu *pMenu = CCMenu::create(pBack, NULL);
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	return true;
}

void CCAbout::menuBack( CCObject *pSender )
{
	CCMainMenu *pMainMenu = CCMainMenu::create();
	CCScene *pScene = CCScene::create();
	pScene->addChild(pMainMenu);
	CCDirector::sharedDirector()->replaceScene(pScene);
}
