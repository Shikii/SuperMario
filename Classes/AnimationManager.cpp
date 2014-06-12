#include "AnimationManager.h"
#include "SimpleAudioEngine.h"
#include <string>
using namespace std;

AnimationManager* AnimationManager::_instance = NULL;

CCAnimation* AnimationManager::createAnimationByAnimationType(AnimationType key)
{
	CCArray* pArray = CCArray::create();
	CCSpriteFrame *frame = NULL;
	CCTexture2D *pTexture;
	CCAnimation *pAnimation = NULL;

	switch (key)
	{
	case eAniRight:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("walkRight.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniLeft:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("walkLeft.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniLeftSmall:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("smallWalkLeft.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(14*i, 0, 14, 16));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniRightSmall:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("smallWalkRight.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(14*i, 0, 14, 16));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniLeftFire:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("WalkLeft_fire.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniRightFire:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("WalkRight_fire.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniBlinkCoin:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("coinani.png");
			for (int i = 0; i < 4; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(8*i, 0, 8, 14));
				pArray->addObject(frame);
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniMushroom:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("Mushroom0.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(16*i, 0, 16, 16));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniflower:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("flower0.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(16*i, 0, 16, 24));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniTortoiseLeft:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("tortoise0.png");
			for (int i = 2; i < 4; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 24));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniTortoiseRight:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("tortoise0.png");
			for (int i = 4; i < 6; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 24));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniTortoiseFly:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("tortoise0.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 24));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.3f);
			break;
		}
	case eAniRotatedFireBall:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("fireRight.png");
			for (int i = 0; i < 8; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(10*i, 3, 10, 10));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.01f);
			break;
		}
	case eAniBoomedFireBall:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("fireRight.png");

			frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(80, 3, 10, 10));
			pArray->addObject(frame);
			frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(90, 0, 14, 16));
			pArray->addObject(frame);
			frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(105, 0, 16, 16));
			pArray->addObject(frame);

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniFireActionR:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("fireActionR.png");
			for (int i = 0; i < 6; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(25*i, 0, 25, 32));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowActionR:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("arrowActionR.png");
			for (int i = 0; i < 6; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(25*i, 0, 25, 32));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniFireActionL:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("fireActionL.png");
			for (int i = 5; i >= 0; --i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(25*i, 0, 25, 32));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowActionL:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("arrowActionL.png");
			for (int i = 5; i >= 0; --i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(25*i, 0, 25, 32));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniFlyFishR:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("flyFishRight.png");
			frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(16*4, 0, 16, 16));
			pArray->addObject(frame);
			frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(16*5, 0, 16, 16));
			pArray->addObject(frame);

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniFlyFishL:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("flyFishLeft.png");
			frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(16*4, 0, 16, 16));
			pArray->addObject(frame);
			frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(16*5, 0, 16, 16));
			pArray->addObject(frame);

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniArrowBroken:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("arrowBroken.png");
			for (int i = 0; i < 3; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(16*i, 0, 16, 16));
				pArray->addObject(frame);
			}

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.05f);
			break;
		}
	case eAniSmallDie:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("small_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(16*i, 0, 16, 18));
				pArray->addObject(frame);
				pArray->addObject(frame);
			}

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniNormalDie:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("normal_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(24*i, 0, 24, 34));
				pArray->addObject(frame);
				pArray->addObject(frame);
			}

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniFireDie:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("fire_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(24*i, 0, 24, 34));
				pArray->addObject(frame);
				pArray->addObject(frame);
			}

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniBossMoveLeft:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("boss.png");
			for (int i = 3; i >=0; --i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(32*i, 0, 32, 32));
				pArray->addObject(frame);
			}

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.2f);
			break;
		}
	case eAniBossMoveRight:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("boss.png");
			for (int i = 4; i < 8; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(32*i, 0, 32, 32));
				pArray->addObject(frame);
			}

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.2f);
			break;
		}
	case eAniBossFireLeft:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("bossBullet.png");
			for (int i = 0; i < 2; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(24*i, 0, 24, 8));
				pArray->addObject(frame);
			}

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniBossFireRight:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("bossBullet.png");
			for (int i = 3; i >= 2; --i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(24*i, 0, 24, 8));
				pArray->addObject(frame);
			}

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniBatteryBoom:
		{
			frame = CCSpriteFrame::create("batteryBoom1.png", CCRectMake(0, 0, 32, 27));
			pArray->addObject(frame);
			frame = CCSpriteFrame::create("batteryBoom2.png", CCRectMake(0, 0, 32, 18));
			pArray->addObject(frame);
			frame = CCSpriteFrame::create("batteryBoom3.png", CCRectMake(0, 0, 32, 20));
			pArray->addObject(frame);

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniLighting:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("lighting.png");

			frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 16, 32));
			pArray->addObject(frame);
			frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(16, 0, 16, 32));
			pArray->addObject(frame);

			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	case eAniArrowLeft:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("allow_walkLeft.png");
			for (int i = 9; i >= 0; --i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowRight:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("allow_walkRight.png");
			for (int i = 0; i < 10; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(18*i, 0, 18, 32));
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.02f);
			break;
		}
	case eAniArrowDie:
		{
			pTexture = CCTextureCache::sharedTextureCache()->addImage("arrow_die.png");
			for (int i = 0; i < 7; ++i)
			{
				frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(24*i, 0, 24, 34));
				pArray->addObject(frame);
				pArray->addObject(frame);
			}
			pAnimation = CCAnimation::createWithSpriteFrames(pArray, 0.1f);
			break;
		}
	default:
		break;
	}
	
	pArray->release();
	
	return pAnimation;
}

void AnimationManager::preLoadEffectAndMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("OnLand.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DingChuMoGuHuoHua.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("AddLife.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("EatMushroomOrFlower.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("SuoXiao.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("BossDiaoLuoQiaoXia.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("BossDie.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("BossHuoQiu.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("CaiSiGuaiWu.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DiaoRuXianJingSi.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DingPoZhuan.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("DingYingZhuanKuai.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("EatCoin.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("GameOver.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("HuoQiuDaDaoGuaiWu.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("Jump.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("QiZiLuoXia.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("YuDaoGuaiWuSi.ogg");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("RengHuoQiu.ogg");
}

void AnimationManager::initAnimationMap()
{
	this->preLoadEffectAndMusic();

	this->setSelectLevel();

	this->setMusicSwitch();

	char temp[20] = { 0 };

	sprintf(temp, "%d", eAniRight);	
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniRight), temp);

	sprintf(temp, "%d", eAniLeft);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniLeft), temp);

	sprintf(temp, "%d", eAniLeftSmall);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniLeftSmall), temp);

	sprintf(temp, "%d", eAniRightSmall);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniRightSmall), temp);

	sprintf(temp, "%d", eAniBlinkCoin);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniBlinkCoin), temp);

	sprintf(temp, "%d", eAniMushroom);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniMushroom), temp);

	sprintf(temp, "%d", eAniflower);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniflower), temp);

	sprintf(temp, "%d", eAniTortoiseLeft);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniTortoiseLeft), temp);

	sprintf(temp, "%d", eAniTortoiseRight);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniTortoiseRight), temp);

	sprintf(temp, "%d", eAniTortoiseFly);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniTortoiseFly), temp);

	sprintf(temp, "%d", eAniRightFire);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniRightFire),  temp);

	sprintf(temp, "%d", eAniLeftFire);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniLeftFire), temp);

	sprintf(temp, "%d", eAniRotatedFireBall);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniRotatedFireBall), temp);

	sprintf(temp, "%d", eAniBoomedFireBall);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniBoomedFireBall), temp);

	sprintf(temp, "%d", eAniFireActionR);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniFireActionR), temp);

	sprintf(temp, "%d", eAniFireActionL);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniFireActionL), temp);

	sprintf(temp, "%d", eAniFlyFishR);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniFlyFishR), temp);

	sprintf(temp, "%d", eAniFlyFishL);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniFlyFishL), temp);

	sprintf(temp, "%d", eAniArrowBroken);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniArrowBroken), temp);

	sprintf(temp, "%d", eAniSmallDie);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniSmallDie), temp);

	sprintf(temp, "%d", eAniNormalDie);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniNormalDie), temp);

	sprintf(temp, "%d", eAniFireDie);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniFireDie), temp);

	sprintf(temp, "%d", eAniBossMoveLeft);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniBossMoveLeft), temp);

	sprintf(temp, "%d", eAniBossMoveRight);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniBossMoveRight), temp);

	sprintf(temp, "%d", eAniBossFireLeft);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniBossFireLeft), temp);

	sprintf(temp, "%d", eAniBossFireRight);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniBossFireRight), temp);

	sprintf(temp, "%d", eAniBatteryBoom);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniBatteryBoom), temp);

	sprintf(temp, "%d", eAniLighting);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniLighting), temp);

	sprintf(temp, "%d", eAniArrowLeft);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniArrowLeft), temp);

	sprintf(temp, "%d", eAniArrowRight);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniArrowRight), temp);

	sprintf(temp, "%d", eAniArrowDie);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniArrowDie), temp);

	sprintf(temp, "%d", eAniArrowActionL);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniArrowActionL), temp);

	sprintf(temp, "%d", eAniArrowActionR);
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		createAnimationByAnimationType(eAniArrowActionR), temp);
}

CCAnimation* AnimationManager::getAnimation(AnimationType key)
{
	char temp[20] = { 0 };
	sprintf(temp, "%d", key);
	
	return CCAnimationCache::sharedAnimationCache()->animationByName(temp);
}

CCAnimate* AnimationManager::createAnimate(const char *key)
{
	CCAnimation *anim = CCAnimationCache::sharedAnimationCache()->animationByName(key);
	if (anim)
	{
		return CCAnimate::create(anim);
	}
	return NULL;
}

CCAnimate* AnimationManager::createAnimate(AnimationType key)
{
	CCAnimation *anim = getAnimation(key);
	if (anim)
	{
		return CCAnimate::create(anim);
	}
	return NULL;
}

AnimationManager* AnimationManager::getInstance()
{
	if (!_instance)
	{
		_instance = new AnimationManager();
	}
	return _instance;
}

void AnimationManager::setSelectLevel()
{
	CCString *str = NULL;
	for (int i = 2; i <= 8; ++i)
	{
		 str= CCString::createWithFormat("Level%d", i);
		CCUserDefault::sharedUserDefault()->setStringForKey(str->m_sString.c_str(), "no");
	}
}

void AnimationManager::setMusicSwitch()
{
	CCUserDefault::sharedUserDefault()->setStringForKey("Music", "on");
	CCUserDefault::sharedUserDefault()->setStringForKey("SoundEffect", "on");
}

