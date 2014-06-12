#include "SetMusic.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include <string>
using namespace std;


CCSetMusic::CCSetMusic()
{
	// 默认的音乐和音效都是开着
	bMusic = true;
	bSoundEffect = true;
}

CCSetMusic::~CCSetMusic()
{

}

bool CCSetMusic::init()
{
	CCSprite *pBg = CCSprite::create("bg.png");
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	
	pBg->setPosition(ccp(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	this->addChild(pBg, 0);

	CCSprite *pSetBg = CCSprite::create("Set_Music.png");
	pSetBg->setAnchorPoint(ccp(129.0/248, 71.0/132));
	pSetBg->setPosition(ccp(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	this->addChild(pSetBg, 1);

	CCMenuItemImage *pBack = CCMenuItemImage::create("backA.png", "backB.png",
		this, menu_selector(CCSetMusic::menuBackMainMenu));
	pBack->setPosition(ccp(origin.x+20, origin.y+visibleSize.height-20));
	CCMenu *pMenu = CCMenu::create(pBack, NULL);
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 2);

	this->initSwitch();

	return true;
}

void CCSetMusic::menuBackMainMenu( CCObject *pSender )
{
	CCScene *pScene = CCScene::create();
	pScene->addChild(CCMainMenu::create());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void CCSetMusic::initSwitch()
{
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	pMusicOn = CCSpriteFrame::create("music_on.png", CCRectMake(0, 0, 70, 63));
	pMusicOn->retain();
	pMusicOff = CCSpriteFrame::create("music_off.png", CCRectMake(0, 0, 70, 63));
	pMusicOff->retain();
	pEffectOn = CCSpriteFrame::create("sound_effect_on.png", CCRectMake(0, 0, 70, 63));
	pEffectOn->retain();
	pEffectOff = CCSpriteFrame::create("sound_effect_off.png", CCRectMake(0, 0, 70, 63));
	pEffectOff->retain();

	pMusic = CCSprite::createWithSpriteFrame(pMusicOn);
	pMusic->setPosition(ccp(origin.x+visibleSize.width/2+80, origin.y+visibleSize.height/2-40));
	this->addChild(pMusic, 3);

	pSoundEffect = CCSprite::createWithSpriteFrame(pEffectOn);
	pSoundEffect->setPosition(ccp(origin.x+visibleSize.width/2+80, origin.y+visibleSize.height/2+40));
	this->addChild(pSoundEffect, 3);

	CCMenuItemImage *pMusicMenu = CCMenuItemImage::create("switchBg.png", "switchBg.png",
		this, menu_selector(CCSetMusic::menuMusic));
	pMusicMenu->setPosition(ccp(origin.x+visibleSize.width/2+80, origin.y+visibleSize.height/2-40));
	CCMenuItemImage *pEffectMenu = CCMenuItemImage::create("switchBg.png", "switchBg.png",
		this, menu_selector(CCSetMusic::menuEffect));
	pEffectMenu->setPosition(ccp(origin.x+visibleSize.width/2+80, origin.y+visibleSize.height/2+40));

	CCMenu *pMenu = CCMenu::create(pMusicMenu, pEffectMenu, NULL);
	pMenu->setAnchorPoint(CCPointZero);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);
}

void CCSetMusic::menuMusic( CCObject *pSender )
{
	if (bMusic)
	{
		bMusic = false;
		pMusic->setDisplayFrame(pMusicOff);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
		CCUserDefault::sharedUserDefault()->setStringForKey("Music", "off");
	}else
	{
		bMusic = true;
		pMusic->setDisplayFrame(pMusicOn);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
		CCUserDefault::sharedUserDefault()->setStringForKey("Music", "on");
	}
}

void CCSetMusic::menuEffect( CCObject *pSender )
{
	if (bSoundEffect)
	{
		bSoundEffect = false;
		pSoundEffect->setDisplayFrame(pEffectOff);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
		CCUserDefault::sharedUserDefault()->setStringForKey("SoundEffect", "off");
	}else
	{
		bSoundEffect = true;
		pSoundEffect->setDisplayFrame(pEffectOn);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
		CCUserDefault::sharedUserDefault()->setStringForKey("SoundEffect", "on");
	}
}

void CCSetMusic::onEnter()
{
	CCLayer::onEnter();

	string str = CCUserDefault::sharedUserDefault()->getStringForKey("Music");
	if (str == "on")
	{
		bMusic = true;
		pMusic->setDisplayFrame(pMusicOn);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
	}else if (str == "off")
	{
		bMusic = false;
		pMusic->setDisplayFrame(pMusicOff);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
	}

	str = CCUserDefault::sharedUserDefault()->getStringForKey("SoundEffect");
	if (str == "on")
	{
		bSoundEffect = true;
		pSoundEffect->setDisplayFrame(pEffectOn);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
	}else if (str == "off")
	{
		bSoundEffect = false;
		pSoundEffect->setDisplayFrame(pEffectOff);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
	}
}

void CCSetMusic::onExit()
{


	CCLayer::onExit();
}

