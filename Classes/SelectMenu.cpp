#include "SelectMenu.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameLayer.h"
#include <string>
using namespace std;


bool CCSelectMenu::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !CCLayer::init() );

		m_nCurPage = 1;
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		pLayer = CCLayer::create();
		CCString *seleteName = NULL;
		for (int i = 1; i <= CCGlobal::getGlobalInstance()->getTotalLevels(); ++i)
		{
			seleteName = CCString::createWithFormat("select%d.jpg", i);
			CCSprite *pSprite = CCSprite::create(seleteName->m_sString.c_str());
			pSprite->setPosition(ccp(visibleSize.width * (i-0.5f), visibleSize.height/2 + 10));
			pLayer->addChild(pSprite, 0, i);
		}

		pLayer->setContentSize(CCSizeMake(480*8, 320));
		this->addChild(pLayer, 1);

		CCSprite *pBg = CCSprite::create("bg.png");
		pBg->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
		this->addChild(pBg, 0);

		pNewGame = CCMenuItemImage::create("newgameA.png", "newgameB.png",
			this, menu_selector(CCSelectMenu::menuBegin));
		pNewGame->setPosition(ccp(origin.x + visibleSize.width/2, 30));

		CCMenuItemImage *pBack = CCMenuItemImage::create("backA.png", "backB.png",
			this, menu_selector(CCSelectMenu::menuBack));
		pBack->setPosition(ccp(origin.x + 20, origin.y + visibleSize.height - 20));

		CCMenu *pMenu = CCMenu::create(pNewGame, pBack, NULL);
		pMenu->setAnchorPoint(CCPointZero);
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu, 2);

		m_pLevel = CCLabelTTF::create("Level: 1", "Arial", 20);
		m_pLevel->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 20));
		this->addChild(m_pLevel, 3);

		bRet = true;
	}while(0);

	return bRet;

}

void CCSelectMenu::menuCloseCallback(CCObject* pSender)
{

}

void CCSelectMenu::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);

	CCString *ccStr = NULL;
	string str;
	CCSprite *pSp = NULL;
	for (int i = 2; i <= 8; ++i)
	{
		ccStr = CCString::createWithFormat("Level%d", i);
		str = CCUserDefault::sharedUserDefault()->getStringForKey(ccStr->m_sString.c_str());
		if (str == "no")
		{
			CCLayerColor *pColor = CCLayerColor::create(ccc4(0, 0, 0, 200));
			pSp = (CCSprite*)pLayer->getChildByTag(i);
			pColor->ignoreAnchorPointForPosition(false);
			pColor->setAnchorPoint(ccp(0.5, 0.5));
			pColor->setPosition(pSp->getPosition());
			pColor->setContentSize(pSp->getContentSize());
			pLayer->addChild(pColor, pSp->getZOrder() + 1);
		}
	}
}

void CCSelectMenu::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}


bool CCSelectMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	m_touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocation());
	return true;
}

void CCSelectMenu::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{

}

void CCSelectMenu::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocation());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 5)
	{
		adjustView(distance);
	}
}

void CCSelectMenu::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint endPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocation());
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 5)
	{
		adjustView(distance);
	}
}

void CCSelectMenu::adjustView(float offset)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	if (offset<0)
	{
		++m_nCurPage;
	}else
	{
		--m_nCurPage;
	}

	if (m_nCurPage <1)
	{
		m_nCurPage = 1;
	}
	if (m_nCurPage > 8)
	{
		m_nCurPage = 8;
	}

	CCPoint  adjustPos = ccp(origin.x - visibleSize.width * (m_nCurPage-1), 0);
	pLayer->runAction(CCMoveTo::create(0.2f, adjustPos));


	CCString *ccStr = CCString::createWithFormat("Level: %d", m_nCurPage);
	m_pLevel->setString(ccStr->m_sString.c_str());
	ccStr = CCString::createWithFormat("Level%d", m_nCurPage);
	string str = CCUserDefault::sharedUserDefault()->getStringForKey(ccStr->m_sString.c_str());
	if (str == "no")
	{
		pNewGame->setEnabled(false);
	}else
	{
		pNewGame->setEnabled(true);
	}
	    
}

void CCSelectMenu::menuBegin( CCObject *pSender )
{
	CCGlobal::getGlobalInstance()->setCurrentLevel(m_nCurPage);
	CCGameLayer *pGameLayer = CCGameLayer::create();

	CCScene *pScene = CCScene::create();
	pScene->addChild(pGameLayer);
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void CCSelectMenu::menuBack( CCObject *pSender )
{
	CCMainMenu *pMainMenu = CCMainMenu::create();
	CCScene *pScene = CCScene::create();
	pScene->addChild(pMainMenu);
	CCDirector::sharedDirector()->replaceScene(pScene);
}
