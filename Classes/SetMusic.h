#ifndef _SETMUSIC_H_
#define _SETMUSIC_H_

#include "cocos2d.h"
using namespace cocos2d;

class CCSetMusic : public CCLayer
{
public:
	CCSetMusic();
	~CCSetMusic();

	bool init();
	void initSwitch();
	
	CREATE_FUNC(CCSetMusic);

	void menuBackMainMenu(CCObject *pSender);
	void menuMusic(CCObject *pSender);
	void menuEffect(CCObject *pSender);

	virtual void onEnter();
	virtual void onExit();
private:
	bool bSoundEffect;
	bool bMusic;
	CCSprite *pMusic;
	CCSprite *pSoundEffect;
	CCSpriteFrame *pMusicOn;
	CCSpriteFrame *pMusicOff;
	CCSpriteFrame *pEffectOn;
	CCSpriteFrame *pEffectOff;
};

#endif