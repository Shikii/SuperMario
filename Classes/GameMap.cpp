#include "GameMap.h"
#include "AnimationManager.h"
#include "Item.h"
#include "Enemy.h"
#include "GameLayer.h"
#include "Hero.h"
#include "Bullet.h"
#include "Global.h"
#include "SimpleAudioEngine.h"

CCGameMap* CCGameMap::_gameMap;
CCGameMap* CCGameMap::getGameMap()
{
	return _gameMap;
}

CCGameMap::CCGameMap()
{
	CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("superMarioMap.png");
	_brokenCoin = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(1, 18, 16, 16));
	_brokenCoin->retain();

	pItemCoordArray = CCPointArray::create(100);
	pItemCoordArray->retain();

	pSpriteArray = CCArray::createWithCapacity(4);
	pSpriteArray->retain();

	pMushroomPointArray = CCPointArray::create(100);
	pMushroomPointArray->retain();

	pEnemyArray = CCArray::create();
	pEnemyArray->retain();

	pBulletArray = CCArray::create();
	pBulletArray->retain();

	pGadgetArray = CCArray::create();
	pGadgetArray->retain();

	pMushSprite = NULL;
	pAddLifeMushroom = NULL;
	pItem = NULL;
	heroInGadget = NULL;

	isBossMap = false;

	_gameMap = this;

	enemyTilePos = CCPointZero;
	pRandomEnemy = NULL;

	// רΪBoss��ͼ׼����
	bridgeTileStartPos = CCPointZero;
	bridgeTileNums = 0;
	pBossEnemy = NULL;
	pPrincess = NULL;
}

CCGameMap::~CCGameMap()
{
	this->unscheduleAllSelectors();

	//CC_SAFE_DELETE(pItemCoordArray);
	//CC_SAFE_DELETE(pEnemyArray);
	//CC_SAFE_DELETE(pBulletArray);
	//CC_SAFE_DELETE(pMushroomPointArray);

	//CC_SAFE_DELETE(landLayer);
	//CC_SAFE_DELETE(blockLayer);
	//CC_SAFE_DELETE(pipeLayer);
	//CC_SAFE_DELETE(coinLayer);
	//CC_SAFE_DELETE(trapLayer);
	//CC_SAFE_DELETE(cloudLayer);
	//CC_SAFE_DELETE(objectLayer);

	//pItemCoordArray->release();
	//pEnemyArray->release();
}

CCGameMap* CCGameMap::create(const char *tmxFile)
{
	CCGameMap *pGameMap = new CCGameMap();
	if (pGameMap && pGameMap->initWithTMXFile(tmxFile))
	{
		pGameMap->extraInit();
		pGameMap->autorelease();
		return pGameMap;
	}
	CC_SAFE_DELETE(pGameMap);
	return NULL;
}

void CCGameMap::extraInit()
{
	tileSize = this->getTileSize();
	mapSize = this->getMapSize();

	cloudLayer = this->layerNamed("cloud");
	blockLayer = this->layerNamed("block");
	pipeLayer = this->layerNamed("pipe");
	landLayer = this->layerNamed("land");
	trapLayer = this->layerNamed("trap");
	objectLayer = this->objectGroupNamed("objects");
	coinLayer = this->layerNamed("coin");
	flagpoleLayer = this->layerNamed("flagpole");

	this->initObjects();

	// ������Щ��������Ӧ���ڳ�ʼ��������֮���������
	// �ڳ�ʼ�������Ӱڷŵ�󣬽�����ڵ�ͼ��
	// ����Ҫ�жϸõ�ͼ�ǲ���BossMap
	if (isBossMap)
	{
		pFlag = CCSprite::create("axe.png");
		pFlag->retain();
		pPrincess = CCSprite::create("princess.png");
		pPrincess->setAnchorPoint(ccp(0.5f, 0.0f));
		pPrincess->setPosition(ccp(finalPoint.x + 16, finalPoint.y));
		this->addChild(pPrincess, this->getChildrenCount());
	} 
	else
	{
		pFlag = CCSprite::create("flag.png");
		pFlag->retain();
	}

	pFlag->setAnchorPoint(ccp(0.5f, 0));
	pFlag->setPosition(flagPoint);
	this->addChild(pFlag, this->getChildrenCount());

	this->launchEnemyInMap();

	this->launchGadgetInMap();

	this->scheduleUpdate();
}

void CCGameMap::showFlagMove()
{
	if (isBossMap)
	{
		// ��Boss�أ�˳��ͨ�ض���Ӧ�����ŵ�����ȥ��Boss����ȥ

		this->initBridgeArray();
	} 
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("QiZiLuoXia.ogg");
		CCMoveBy *pMoveBy = CCMoveBy::create(2.0f, ccp(0, -8*16));
		pFlag->runAction(pMoveBy);
	}
}

void CCGameMap::initObjects()
{
	CCArray *tempArray = objectLayer->getObjects();
	CCDictionary *pDic = NULL;
	for (unsigned int idx = 0; idx < tempArray->count(); ++idx)
	{
		pDic = (CCDictionary *)tempArray->objectAtIndex(idx);
		int posX = ((CCString*)pDic->objectForKey("x"))->intValue();
		int posY = ((CCString*)pDic->objectForKey("y"))->intValue();
		posY -= this->getTileSize().height;
		CCPoint tileXY = this->positionToTileCoord(ccp(posX, posY));

		CCString *name = (CCString*)pDic->objectForKey("name");
		CCString *type = (CCString*)pDic->objectForKey("type");

		// ���й���ĳ�ʼ�����ȸ����������ж��ǲ���enemy����ϸ��enemy����		
		if (name->m_sString == "enemy")
		{
			CCEnemy *pEnemy = NULL;
			if (type->m_sString == "mushroom")
			{
				pEnemy = new CCEnemyMushroom();
			}
			else if (type->m_sString == "flower")
			{
				pEnemy = new CCEnemyFlower();
			}
			else if (type->m_sString == "tortoise")
			{
				// ��ͼ��Ԥ�ȱ�Ǻõ��ڹ궼�������
				pEnemy = new CCEnemyTortoise(0);				
			}
			else if (type->m_sString == "tortoise_round")
			{
				CCString *dis = (CCString *)pDic->objectForKey("roundDis");
				int roundDis = dis->floatValue();
				pEnemy = new CCEnemyTortoiseRound(roundDis);
			}
			else if (type->m_sString == "tortoise_fly")
			{
				CCString *dis = (CCString*)pDic->objectForKey("flyDis");
				int flyDis = dis->floatValue();
				pEnemy = new CCEnemyTortoiseFly(flyDis);
			}
			else if (type->m_sString == "fire_string")
			{
				CCString *str = (CCString *)pDic->objectForKey("begAngle");
				float begAngle = str->floatValue();
				str = (CCString *)pDic->objectForKey("time");
				float time = str->floatValue();
				pEnemy = new CCEnemyFireString(begAngle, time);
			}
			else if (type->m_sString == "flyfish")
			{
				CCString *str = (CCString*)pDic->objectForKey("offsetH");
				float offsetH = str->floatValue();
				str = (CCString *)pDic->objectForKey("offsetV");
				float offsetV = str->floatValue();
				str = (CCString *)pDic->objectForKey("duration");
				float duration = str->floatValue();

				pEnemy = new CCEnemyFlyFish(offsetH, offsetV, duration);
			}
			else if (type->m_sString == "boss")
			{
				isBossMap = true;
				pEnemy = new CCEnemyBoss();
				pBossEnemy = pEnemy;
			}
			else if (type->m_sString == "addmushroom")
			{
				CCString *str = (CCString *)pDic->objectForKey("nums");
				int nums = str->intValue();
				pEnemy = new CCEnemyAddMushroom(nums);
			}
			else if (type->m_sString == "battery")
			{
				CCString *str = (CCString *)pDic->objectForKey("delay");
				float delay = str->floatValue();
				pEnemy = new CCEnemyBattery(delay);
			}
			else if (type->m_sString == "darkcloud")
			{
				CCString *str = (CCString*)pDic->objectForKey("delay");
				float delay = str->floatValue();
				str = (CCString *)pDic->objectForKey("style");
				int style = str->intValue();
				pEnemy = new CCEnemyDarkCloud(delay, style);
			}

			if (pEnemy != NULL)
			{
				pEnemy->setTileCoord(tileXY);
				pEnemy->setEnemyPos(ccp(posX, posY));
				pEnemyArray->addObject(pEnemy);
			}
		}
		else if (name->m_sString == "gadget")
		{
			CCString *str = (CCString*)pDic->objectForKey("ladderDis");
			float dis = str->floatValue();
			int val;
			CCGadget *pGadget = NULL;
			if (type->m_sString == "ladderLR")
			{
				// �����ƶ�������
				pGadget = new CCGadgetLadderLR(dis);
				str = (CCString *)pDic->objectForKey("LorR");
				val = str->intValue();
				pGadget->setStartFace(val);
			}
			else if (type->m_sString == "ladderUD")
			{
				// �����ƶ�������
				pGadget = new CCGadgetLadderUD(dis);
				str = (CCString *)pDic->objectForKey("UorD");
				val = str->intValue();
				pGadget->setStartFace(val);
			}

			if (pGadget != NULL)
			{
				pGadget->setStartPos(ccp(posX, posY));
				pGadgetArray->addObject(pGadget);
			}
		}
		else if (name->m_sString == "mushroom")
		{
			if (type->m_sString == "MushroomReward")
			{
				// ���˱������ӵ�Ģ����
				pMushroomPointArray->addControlPoint(tileXY);
			}
			else if (type->m_sString == "MushroomAddLife")
			{
				// ��ͼ�н��е�һ������Ģ����
				addLifePoint = tileXY;
			}
		}
		else if (name->m_sString == "others")
		{
			if (type->m_sString == "BirthPoint")
			{
				// ������ڵ�ǰ��ͼ�ϵĳ�����
				marioBirthPos = this->tilecoordToPosition(tileXY);
				marioBirthPos.x += this->getTileSize().width/2;
			}
			else if (type->m_sString == "flagpoint")
			{
				flagPoint = ccp(posX, posY);
			}
			else if (type->m_sString == "finalpoint")
			{
				finalPoint = ccp(posX, posY);
			}
			else if (type->m_sString == "bridgestartpos")
			{
				bridgeTileStartPos = tileXY;
			}
		}
	}
}

void CCGameMap::launchEnemyInMap()
{
	CCEnemy *tempEnemy = NULL;
	unsigned int enemyCount = pEnemyArray->count();
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		tempEnemy = (CCEnemy *)pEnemyArray->objectAtIndex(idx);
		tempEnemy->setPosition(tempEnemy->getEnemyPos());
		switch (tempEnemy->getEnemyType())
		{
		case eEnemy_flower:
		case eEnemy_AddMushroom:
			this->addChild(tempEnemy, 3);
			break;
		default:
			this->addChild(tempEnemy, 7);
			break;
		}
		tempEnemy->launchEnemy();
	}
}

void CCGameMap::launchGadgetInMap()
{
	CCGadget *tempGadget = NULL;
	unsigned int gadgetCount = pGadgetArray->count();
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		tempGadget = (CCGadget *)pGadgetArray->objectAtIndex(idx);
		tempGadget->setPosition(tempGadget->getStartPos());
		this->addChild(tempGadget, 3);
		tempGadget->launchGadget();
	}
}

void CCGameMap::enemyVSHero()
{
	CCEnemy *tempEnemy = NULL;
	EnemyVSHero vsRet;
	unsigned int enemyCount = pEnemyArray->count();
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		tempEnemy = (CCEnemy *)pEnemyArray->objectAtIndex(idx);
		if (tempEnemy->getEnemyState() == eEnemyState_active)
		{
			vsRet = tempEnemy->checkCollisionWithHero();
			switch (vsRet)
			{
			case eVS_heroKilled:
				{
					if (! CCHero::getHeroInstance()->getIsSafeTime())
					{
						CCHero::getHeroInstance()->changeForGotEnemy();
					}
					break;
				}
			case eVS_enemyKilled:
				{
					tempEnemy->forKilledByHero();
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("CaiSiGuaiWu.ogg");
					break;
				}
			default:
				break;
			}
		}
	}
}

void CCGameMap::update(float dt)
{
	this->enemyVSHero();
	this->bulletVSEnemy();
}

void CCGameMap::stopUpdateForHeroDie()
{
	// ֹͣ����ĸ���
	unsigned int enemyCount = pEnemyArray->count();
	CCEnemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (CCEnemy *)pEnemyArray->objectAtIndex(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->stopEnemyUpdate();
			switch (pEnemy->getEnemyType())
			{
			case eEnemy_mushroom:
			case eEnemy_tortoise:
				break;
			case eEnemy_flower:
			case eEnemy_flyFish:
			case eEnemy_tortoiseRound:
			case eEnemy_tortoiseFly:
			case eEnemy_fireString:
			//case eEnemy_Boss:
				pEnemy->stopAllActions();
				break;
			}
			
			pEnemy->unscheduleUpdate();
		}
	}

	// ֹͣ�ӵ��ĸ���
	unsigned int bulletCount = pBulletArray->count();
	CCBullet *pBullet = NULL;	
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (CCBullet *)pBulletArray->objectAtIndex(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->unscheduleUpdate();
		}
	}

	// ֹͣС����ĸ���
	unsigned int gadgetCount = pGadgetArray->count();
	CCGadget *pGadget = NULL;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		pGadget = (CCGadget *)pGadgetArray->objectAtIndex(idx);
		pGadget->stopAllActions();
		pGadget->unscheduleUpdate();
	}

	this->unscheduleUpdate();
}

CCTMXLayer* CCGameMap::getCloudLayer()
{
	return cloudLayer;
}

CCTMXLayer* CCGameMap::getLandLayer()
{
	return landLayer;
}

CCTMXLayer* CCGameMap::getPipeLayer()
{
	return pipeLayer;
}

CCTMXLayer* CCGameMap::getBlockLayer()
{
	return blockLayer;
}

CCTMXLayer* CCGameMap::getTrapLayer()
{
	return trapLayer;
}

CCTMXObjectGroup* CCGameMap::getObjectLayer()
{
	return objectLayer;
}

CCTMXLayer* CCGameMap::getCoinLayer()
{
	return coinLayer;
}

CCTMXLayer* CCGameMap::getFlagpoleLayer()
{
	return flagpoleLayer;
}

CCPoint CCGameMap::getMarioBirthPos()
{
	return marioBirthPos;
}

// ����һ��Tile����ϵ�µĵ㣬�����õ��Ӧ��ͼ�е�ͼ��Ԫ������
TileType CCGameMap::tileTypeforPos(CCPoint tileCoord)
{
	int GID = pipeLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Pipe;
	}
	GID = blockLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Block;
	}
	GID = landLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Land;
	}
	GID = trapLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Trap;
	}
	GID = coinLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Coin;
	}
	GID = flagpoleLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Flagpole;
	}
	return eTile_NoneH;
}

// ��block���� ��Щblockש���ǿ��б�����ģ�
// ��Щש������˸���ʺţ�ͷ��һ�»ᵯ��һ�������
// �ڵ�ͼ��ͼ�������У���blockType����ʾ
// 1: ��˸���ʺ�
// 2: ��ͨ��һ�������ש��
void CCGameMap::breakBlock(CCPoint tileCoord, BodyType _bodyType)
{
	int gID = blockLayer->tileGIDAt(tileCoord);
	CCDictionary *pD = NULL;
	pD = this->propertiesForGID(gID);
	
	if (pD)
	{
		CCString *str = NULL;
		str = (CCString*)pD->objectForKey("blockType");
		if (str)
		{
			int blockType = str->intValue();
			if (blockType == 2)
			{
				switch (_bodyType)
				{
				case eBody_Normal:
					{
						// ��������:ש�鱻������Ŀ飬Ȼ����ʧ��
						this->showBlockBroken(tileCoord);
						blockLayer->removeTileAt(tileCoord);
					}
					break;
				case eBody_Small:
					{
						// ������С������£�ש��ֻ�������ƶ�һ��Ȼ���ٻָ�λ��
						this->showBlockJump(tileCoord);
						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingYingZhuanKuai.ogg");
					}
					break;
				default:
					break;
				}
			}
			else if (blockType == 1)  // �������ש��
			{
				if (itemCoordArrayContains(tileCoord) == false)
				{
					// ���統ǰ�����������ʺ�û�б�����							
					pItemCoordArray->addControlPoint(tileCoord);

					// ��֮��Ҫ�ѽ���Ģ���������˸���ʺ�ש����
					// Ӧ�÷�����һ�����εĵط�����Ȼ��blockͼ�㣬�����õ�ȷʵ��ձ�����ͼ��
					//if (tileCoord.x == addLifePoint.x && tileCoord.y == addLifePoint.y)
					//{
					//	// �����������Ģ�����Ǹ���ͼ����
					//	this->showAddLifeMushroom(tileCoord);
					//}
					if (mushroomPointContains(tileCoord))
					{
						// �����Ģ����ô��ҾͲ�չʾ��Ծ����
						resetCoinPoint = tileCoord;
						this->resetCoinBlockTexture();
						this->showNewMushroom(tileCoord, _bodyType);
						this->deleteOneMushPointFromArray(tileCoord);
					} 
					else
					{
						// ��ש��������������Ҳ�����
						if (CCRANDOM_0_1() > 0.4f)
						{
							CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
							this->showJumpUpBlinkCoin(tileCoord);
							// �������ͨ�Ľ�ң���ôչʾһ����Ծ����
							this->showCoinJump(tileCoord);
						}
						else
						{
							this->showCoinJump(tileCoord);
							// ͣ��0.2��
							enemyTilePos = tileCoord;
							this->runAction(CCSequence::create(CCDelayTime::create(0.2f),
								CCCallFunc::create(this, callfunc_selector(CCGameMap::randomShowEnemy)), NULL));
						}

					}
				}
				else
				{
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingYingZhuanKuai.ogg");
				}
			}
			else if (blockType == 3)
			{
				if (itemCoordArrayContains(tileCoord) == false)
				{
					pItemCoordArray->addControlPoint(tileCoord);
					// ���ص�ש�飬һ�������н���Ģ��
					CCSprite *pSprite = blockLayer->tileAt(tileCoord);
					pSprite->setDisplayFrame(_brokenCoin);
					this->showAddLifeMushroom(tileCoord);
				}
			}
		}
	}
	//else
	//{
	//	// Ҫ�����еı������ӵ�Ģ������˸��ש����Ļ�������жϴ���Ҳ����ִ����
	//	//if (mushroomPointContains(tileCoord))
	//	//{
	//	//	this->showNewMushroom(tileCoord, _bodyType);
	//	//	this->deleteOneMushPointFromArray(tileCoord);
	//	//}
	//	// ��������������˵�Ļ�����ô���еĽ���Ģ�������ж϶���ִ�������
	//	if (tileCoord.x == addLifePoint.x && tileCoord.y == addLifePoint.y)
	//	{
	//		// �����������Ģ�����Ǹ���ͼ����
	//		this->showAddLifeMushroom(tileCoord);
	//	}
	//}
}

void CCGameMap::randomShowEnemy()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHua.wma");

	// �����Ĺ�����Ҳ��һ�����
	if (CCRANDOM_0_1() > 0.5f)
	{
		pRandomEnemy = new CCEnemyMushroom();
		if (CCRANDOM_0_1() > 0.5f)
		{
			// ���ó�ʼ��������
			pRandomEnemy->setMoveOffset(pRandomEnemy->getccMoveOffset());
		}
	} 
	else
	{
		int val = 1;
		if (CCRANDOM_0_1() < 0.5f)
		{
			val = 1;
		}
		pRandomEnemy = new CCEnemyTortoise(val);
	}

	CCPoint pos = this->tilecoordToPosition(enemyTilePos);
	pos.x += tileSize.width/2;

	pRandomEnemy->setPosition(pos);
	this->addChild(pRandomEnemy, blockLayer->getZOrder()-1);

	pRandomEnemy->runAction(CCSequence::create(CCJumpBy::create(0.2f, ccp(0, 16), 1, 20),
		CCCallFunc::create(this, callfunc_selector(CCGameMap::randomLaunchEnemy)), NULL));
	

}

void CCGameMap::randomLaunchEnemy()
{
	pEnemyArray->addObject(pRandomEnemy);
	//pRandomEnemy->setEnemyPos(pos);
	pRandomEnemy->_setZOrder(7);
	pRandomEnemy->launchEnemy();
}

bool CCGameMap::itemCoordArrayContains(CCPoint tileCoord)
{
	CCPoint temp;
	bool flag = false;
	for (unsigned int idx = 0; idx < pItemCoordArray->count(); ++idx)
	{
		temp = pItemCoordArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

bool CCGameMap::mushroomPointContains(CCPoint tileCoord)
{
	CCPoint temp;
	bool flag = false;
	for (unsigned int idx = 0; idx < pMushroomPointArray->count(); ++idx)
	{
		temp = pMushroomPointArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

void CCGameMap::deleteOneMushPointFromArray(CCPoint tileCoord)
{
	CCPoint temp ;
	for (unsigned int idx = 0; idx < pMushroomPointArray->count(); ++idx)
	{
		temp = pMushroomPointArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			pMushroomPointArray->removeControlPointAtIndex(idx);
			break;
		}
	}
}

void CCGameMap::showBlockBroken(CCPoint tileCoord)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingPoZhuan.ogg");

	CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("singleblock.png");
	CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 8, 8));
	CCPoint pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width/2;
	pos.y += this->getTileSize().height/2;

	CCSprite *pSprite1 = CCSprite::createWithSpriteFrame(frame);	
	pSprite1->setPosition(pos);
	pSpriteArray->addObject(pSprite1);
	CCSprite *pSprite2 = CCSprite::createWithSpriteFrame(frame);
	pSprite2->setPosition(pos);
	pSpriteArray->addObject(pSprite2);
	CCSprite *pSprite3 = CCSprite::createWithSpriteFrame(frame);
	pSprite3->setPosition(pos);
	pSpriteArray->addObject(pSprite3);
	CCSprite *pSprite4 = CCSprite::createWithSpriteFrame(frame);
	pSprite4->setPosition(pos);
	pSpriteArray->addObject(pSprite4);

	this->addChild(pSprite1);
	this->addChild(pSprite2);
	this->addChild(pSprite3);
	this->addChild(pSprite4);

	CCActionInterval *pLeftUp = CCJumpBy::create(0.2f,
		ccp(-this->getTileSize().width*2, this->getTileSize().height), 10, 1);
	CCActionInterval *pRightUp = CCJumpBy::create(0.2f, 
		ccp(this->getTileSize().width*2, this->getTileSize().height), 10, 1);
	CCActionInterval *pLeftDown = CCJumpBy::create(0.2f, 
		ccp(-this->getTileSize().width*3, -this->getTileSize().height), 5, 1);
	CCActionInterval *pRightDown = CCJumpBy::create(0.2f, 
		ccp(this->getTileSize().width*3, -this->getTileSize().height), 5, 1);

	pSprite1->runAction(pLeftUp);
	pSprite2->runAction(pRightUp);
	pSprite3->runAction(pLeftDown);
	pSprite4->runAction(CCSequence::create(pRightDown,
		CCCallFunc::create(this, callfunc_selector(CCGameMap::clearSpriteArray)), NULL));
}

void CCGameMap::showJumpUpBlinkCoin(CCPoint tileCoord)
{
	pItem = CCItem::create(eBlinkCoin);
	CCPoint pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width/2;
	pos.y += this->getTileSize().height;
	pItem->setPosition(pos);
	pItem->setVisible(true);
	this->addChild(pItem);

	CCActionInterval *pJump = CCJumpBy::create(0.16f, ccp(0, this->getTileSize().height),
		this->getTileSize().height*1.5, 1);

	pItem->itemBody->runAction(sAnimationMgr->createAnimate(eAniBlinkCoin));
	pItem->runAction(CCSequence::create(pJump, CCCallFunc::create(
		this, callfunc_selector(CCGameMap::clearItem)), NULL));
}

void CCGameMap::showBlockJump(CCPoint tileCorrd)
{
	CCSprite *tempSprite = blockLayer->tileAt(tileCorrd);
	CCActionInterval *pJumpBy = CCJumpBy::create(0.2f, CCPointZero, 
		this->getTileSize().height*0.5, 1);
	tempSprite->runAction(pJumpBy);
}

void CCGameMap::showCoinJump(CCPoint tileCoord)
{
	CCSprite *tempSprite = blockLayer->tileAt(tileCoord);
	CCActionInterval *pJumpBy = CCJumpBy::create(0.2f, CCPointZero, 
		this->getTileSize().height*0.5, 1);

	// ͨ�����еĳ�Ա���������ݲ��������Լ򻯺������йش��εĴ���
	resetCoinPoint = tileCoord;

	tempSprite->runAction(CCSequence::create(pJumpBy, 
		CCCallFunc::create(this, callfunc_selector(CCGameMap::resetCoinBlockTexture)), NULL));
}

void CCGameMap::resetCoinBlockTexture()
{
	CCSprite *coinTile = blockLayer->tileAt(resetCoinPoint);
	coinTile->setDisplayFrame(_brokenCoin);
}



void CCGameMap::showNewMushroom(CCPoint tileCoord, BodyType _bodyType)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHuoHua.ogg");
	// ���õ�ǰĢ��������
	mushTileCoord = ccp(tileCoord.x, tileCoord.y - 1);

	CCPoint pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width/2;
	pos.y += this->getTileSize().height/2;

	switch (_bodyType)
	{
	case eBody_Small:
		{
			pMushSprite = CCSprite::create("rewardMushroomSet.png", CCRectMake(0, 0, 16, 16));
		}
		break;
	case eBody_Normal:
		{
			pMushSprite = CCSprite::create("Tools.png", CCRectMake(0, 0, 18, 18));
		}
		break;
	default:
		break;
	}

	pMushSprite->setPosition(pos);
	this->addChild(pMushSprite);

	CCMoveBy *pMushJump = CCMoveBy::create(0.4f, ccp(0, this->getTileSize().height));
	pMushSprite->runAction(pMushJump);
}

void CCGameMap::showAddLifeMushroom(CCPoint tileCoord)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHua.wma");

	// ��һ�ε����ý���Ģ��������
	// ��һ�ν�y��������һ����λ����Ϊ�˺�����ж��Ƿ�ٵ�����Ģ������
	addLifePoint = ccp(tileCoord.x, tileCoord.y - 1);

	CCPoint pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width/2;
	pos.y += this->getTileSize().height/2;

	pAddLifeMushroom = CCSprite::create("rewardMushroomSet.png", CCRectMake(16, 0, 16, 16));

	pAddLifeMushroom->setPosition(pos);
	this->addChild(pAddLifeMushroom);

	CCMoveBy *pMushJump = CCMoveBy::create(0.4f, ccp(0, this->getTileSize().height));
	pAddLifeMushroom->runAction(pMushJump);
}


void CCGameMap::clearItem()
{
	pItem->removeFromParentAndCleanup(true);
	pItem = NULL;
}

void CCGameMap::clearSpriteArray()
{
	CCSprite *pS = NULL;
	for (unsigned int idx = 0; idx < pSpriteArray->count(); ++idx)
	{
		pS = (CCSprite *)pSpriteArray->objectAtIndex(idx);
		pS->removeFromParentAndCleanup(true);
	}
}

bool CCGameMap::isMarioEatMushroom(CCPoint tileCoord)
{
	if (pMushSprite == NULL)
	{
		return false;
	}
	if (tileCoord.x == mushTileCoord.x && tileCoord.y == mushTileCoord.y)
	{
		pMushSprite->removeFromParentAndCleanup(true);
		mushTileCoord = CCPointZero;
		pMushSprite = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

bool CCGameMap::isMarioEatAddLifeMushroom(CCPoint tileCoord)
{
	if (pAddLifeMushroom == NULL)
	{
		return false;
	}
	if (tileCoord.x == addLifePoint.x && tileCoord.y == addLifePoint.y)
	{
		pAddLifeMushroom->removeFromParentAndCleanup(true);
		mushTileCoord = CCPointZero;
		pAddLifeMushroom = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

CCPoint CCGameMap::positionToTileCoord(CCPoint pos)
{
	int x = pos.x / this->getTileSize().width;
	int y = (this->getMapSize().height - 1) - pos.y/this->getTileSize().height;
	return ccp(x, y);
}

CCPoint CCGameMap::tilecoordToPosition(CCPoint tileCoord)
{
	float x = tileCoord.x * this->getTileSize().width;
	float y = (this->getMapSize().height - 1 - tileCoord.y) * this->getTileSize().height;
	return ccp(x, y);
}

void CCGameMap::createNewBullet()
{
	CCBullet *pBullet = NULL;
	switch (CCGlobal::getGlobalInstance()->getCurrentBulletType())
	{
	case eBullet_common:
		pBullet = new CCBulletCommon();
		break;
	case eBullet_arrow:
		pBullet = new CCBulletArrow();
		break;
	default:
		break;
	}
	
	pBulletArray->addObject(pBullet);
	
	pBullet->setPosition(pBullet->getStartPos());
	this->addChild(pBullet, 7);
	pBullet->launchBullet();
}

void CCGameMap::createNewBulletForBoss(CCPoint pos, EnemyType _enemyType)
{
	CCEnemy *pEnemy = NULL;

	switch (_enemyType)
	{
	case eEnemy_BossFire:
		pEnemy = new CCEnemyBossFire();
		break;
	case eEnemy_mushroom:
		pEnemy = new CCEnemyMushroom();
		break;
	case eEnemy_BatteryBullet:
		pEnemy = new CCEnemyBatteryBullet();
		break;
	case eEnemy_Lighting:
		pEnemy = new CCEnemyLighting();
		break;
	default:
		break;
	}

	if (pEnemy != NULL)
	{
		pEnemyArray->addObject(pEnemy);
		pEnemy->setEnemyPos(pos);
		pEnemy->setPosition(pos);
		this->addChild(pEnemy, 7);
		pEnemy->launchEnemy();
	}
}

void CCGameMap::bulletVSEnemy()
{
	unsigned int bulletCount = pBulletArray->count();
	unsigned int enemyCount = pEnemyArray->count();
	CCBullet *pBullet = NULL;
	CCEnemy *pEnemy = NULL;
	CCArray *delBullet = CCArray::create();
	delBullet->retain();
	CCArray *delEnemy = NULL;
	CCRect bulletRect;
	CCRect enemyRect;

	for (unsigned int idxBullet = 0; idxBullet < bulletCount; ++idxBullet)
	{
		pBullet = (CCBullet *)pBulletArray->objectAtIndex(idxBullet);
		if (pBullet->getBulletState() == eBulletState_nonactive)
		{
			delBullet->addObject(pBullet);
			continue;
		}
		bulletRect = pBullet->getBulletRect();

		for (unsigned int idxEnemy = 0; idxEnemy < enemyCount; ++idxEnemy)
		{
			pEnemy = (CCEnemy *)pEnemyArray->objectAtIndex(idxEnemy);
			switch (pEnemy->getEnemyType())
			{
			// ���ǲ��ܱ��κ��ӵ������
			// �궷�޵���̨������ӵ����ᱻ�κ��ӵ����
			case eEnemy_BatteryBullet:
			case eEnemy_fireString:
			case eEnemy_Lighting:
			case eEnemy_DarkCloud:
				continue;
				break;
			}
			if (pBullet->getBulletType() == eBullet_common &&
				pEnemy->getEnemyType() == eEnemy_Boss)
			{
				// ��ͨ���ӵ���Bossû������
				continue;
			}
			if (pBullet->getBulletType() == eBullet_common &&
				pEnemy->getEnemyType() == eEnemy_BossFire)
			{
				// ��ͨ�ӵ���Boss�����Ļ���Ҳ��û�����õ�
				continue;
			}

			if (pEnemy->getEnemyState() == eEnemyState_active)
			{
				enemyRect = pEnemy->getEnemyRect();

				if (bulletRect.intersectsRect(enemyRect))
				{
					pBullet->forKilledEnemy();
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("HuoQiuDaDaoGuaiWu.ogg");
					pEnemy->forKilledByBullet();
				}
			}
		}
	}

	unsigned int delCount = delBullet->count();
	for (unsigned int idxDel = 0; idxDel < delCount; ++idxDel)
	{
		pBullet = (CCBullet *)delBullet->objectAtIndex(idxDel);
		pBulletArray->removeObject(pBullet, true);
		this->removeChild(pBullet, true);
	}
	delBullet->release();
}

bool CCGameMap::isHeroInGadget(CCPoint heroPos, float &gadgetLevel)
{
	bool ret = false;
	CCGadget *tempGadget = NULL;
	unsigned int gadgetCount = pGadgetArray->count();
	CCRect gadgetRect;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		tempGadget = (CCGadget *)pGadgetArray->objectAtIndex(idx);
		if (tempGadget->getGadgetState() == eGadgetState_active)
		{
			gadgetRect = tempGadget->getGadgetRect();
			if (gadgetRect.containsPoint(heroPos))
			{
				gadgetLevel = tempGadget->getPosition().y + tempGadget->getGadgetSize().height;
				ret = true;
				this->heroInGadget = tempGadget;
				CCHero::getHeroInstance()->setGadgetable(true);
				break;
			}
		}
	}
	return ret;
}

void CCGameMap::initBridgeArray()
{
	bridgeTileNums = 13;

	CCSprite *pS = NULL;
	CCPoint bossPos = pBossEnemy->getPosition();
	CCPoint pos;
	for (int i = 0; i < bridgeTileNums; ++i)
	{
		CCPoint tilePos = bridgeTileStartPos;
		tilePos.x += i;

		pS = landLayer->tileAt(tilePos);
		pS->runAction(CCMoveBy::create(1.0f, ccp(0, -60)));

		pos = this->tilecoordToPosition(tilePos);
		if (pos.x >= bossPos.x)
		{
			if (pBossEnemy->getEnemyState() == eEnemyState_active)
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("BossDiaoLuoQiaoXia.ogg");
				pBossEnemy->runAction(CCMoveBy::create(1.0f, ccp(0, -80)));
			}
		}
	}

	// ���ǰѸ�������Ϊ���ɼ�
	pFlag->setVisible(false);
}

void CCGameMap::pauseGameMap()
{
	// ֹͣ����ĸ���
	unsigned int enemyCount = pEnemyArray->count();
	CCEnemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (CCEnemy *)pEnemyArray->objectAtIndex(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->unscheduleUpdate();
		}
	}

	// ֹͣ�ӵ��ĸ���
	unsigned int bulletCount = pBulletArray->count();
	CCBullet *pBullet = NULL;	
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (CCBullet *)pBulletArray->objectAtIndex(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->unscheduleUpdate();
		}
	}

	// ֹͣС����ĸ���
	unsigned int gadgetCount = pGadgetArray->count();
	CCGadget *pGadget = NULL;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		pGadget = (CCGadget *)pGadgetArray->objectAtIndex(idx);
		pGadget->unscheduleUpdate();
	}

	this->unscheduleUpdate();
}

void CCGameMap::resumeGameMap()
{
	// ֹͣ����ĸ���
	unsigned int enemyCount = pEnemyArray->count();
	CCEnemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (CCEnemy *)pEnemyArray->objectAtIndex(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->scheduleUpdate();
		}
	}

	// ֹͣ�ӵ��ĸ���
	unsigned int bulletCount = pBulletArray->count();
	CCBullet *pBullet = NULL;	
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (CCBullet *)pBulletArray->objectAtIndex(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->scheduleUpdate();
		}
	}

	// ֹͣС����ĸ���
	unsigned int gadgetCount = pGadgetArray->count();
	CCGadget *pGadget = NULL;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		pGadget = (CCGadget *)pGadgetArray->objectAtIndex(idx);
		pGadget->scheduleUpdate();
	}

	this->scheduleUpdate();
}




