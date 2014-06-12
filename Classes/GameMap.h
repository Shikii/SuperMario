#ifndef _GAMEMAP_H_
#define _GAMEMAP_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class CCItem;
class CCEnemy;
class CCGadget;

class CCGameMap : public CCTMXTiledMap
{
	// ֻ���������ĸ���Ϸ��Ҫ�õ���ͼ��
	// �Ʋ������ƶ�
	CC_PROPERTY_READONLY(CCTMXLayer*, cloudLayer, CloudLayer);
	// ʣ����ĸ�ͼ����Ϊ����ײ���
	CC_PROPERTY_READONLY(CCTMXLayer*, blockLayer, BlockLayer);
	CC_PROPERTY_READONLY(CCTMXLayer*, pipeLayer, PipeLayer);
	CC_PROPERTY_READONLY(CCTMXLayer*, landLayer, LandLayer);
	CC_PROPERTY_READONLY(CCTMXLayer*, trapLayer, TrapLayer);
	// �����
	CC_PROPERTY_READONLY(CCTMXObjectGroup*, objectLayer, ObjectLayer);
	// ��Ҳ�
	CC_PROPERTY_READONLY(CCTMXLayer*, coinLayer, CoinLayer);
	// ��˲�
	CC_PROPERTY_READONLY(CCTMXLayer*, flagpoleLayer, FlagpoleLayer);
public:
	CCGameMap();
	~CCGameMap();

	//// ���ͼ��Ϣ���
	CCSize tileSize; // ��ͼ�е�ͼ��ߴ�
	CCSize mapSize;  // ��ͼ��ͼ��ĸ���

	// �����������ڹ��سɹ���չʾ�����õ���
	CCPoint flagPoint;  // С���ӵĵ�
	CCSprite *pFlag;
	void showFlagMove();  // չʾ���Ӵ���������µĶ���
	CCPoint finalPoint;  // ��������Ҫ�ƶ��������ſڵĵ�
	bool isBossMap;  // ��ʶ�Ƿ�ΪBoss��

	void createNewBulletForBoss(CCPoint pos, EnemyType _enemyType); // Boss������һ���ӵ�



	// ��ͨ�ĳ��˻����Ģ��������
	// ����Ģ�������ݵ�ǰ����µ����ʹ�С���ж϶���������Ģ�������ӵ�
	// ��¼��Ģ�����ǻ���ͼ������ϵ�µ�
	CCPointArray *pMushroomPointArray;

	CCSprite *pAddLifeMushroom;  // ����Ģ������
	CCPoint addLifePoint;  // ÿһ���ж���һ�����˵�Ģ����

	// ��ͼ�ϱ�������й������Ϣ����
	CCArray *pEnemyArray;

	// ��ͼ�����е��ӵ�����Ϣ
	CCArray *pBulletArray;

	// ��ͼ�����е�С���������Ϣ
	CCArray *pGadgetArray;
	// ��ǰ�������Ǹ������
	CCGadget *heroInGadget;

	// ��¼��ǰ��Ļ�еĹ�������
	//CCArray *pEnemyInScreen;
	bool isHeroInGadget(CCPoint heroPos, float &gadgetLevel);

	CCSprite *pMushSprite;  //��ǰ��������Ģ������
	CCPoint mushTileCoord;  // ��ǰĢ�����ڵ�λ��

	CCItem *pItem; // ��ǰ��������˸���ʺ�

	CCSpriteFrame *_brokenCoin;  // ���ʺŵĽ�ұ�������Ҫ���滻����������ʽ

	// ��ǰ��Ҫ������Ľ��ש��λ��
	CCPoint resetCoinPoint;

	// ÿ�ŵ�ͼ������µĳ����㣬GL����ϵ�µ����꣬CCGameLayer��ȡ��ֱ�ӿ��õ�
	CCPoint marioBirthPos;
	CCPoint getMarioBirthPos();

	static CCGameMap* create(const char *tmxFile);

	// ���ص�ͼָ��λ���ϵ�ͼ�����ͣ�������ײ���
	TileType tileTypeforPos(CCPoint tileCoord);

	// ��ͼ�ϵ�blockͼ���ϵ�ĳЩש���ǿ��Ա����Ƕ���
	void breakBlock(CCPoint tileCoord, BodyType _bodyType);

	// չʾblockͼ���ϵ�ĳЩש�鱻������¶�����Ŀ�Ķ���
	void showBlockBroken(CCPoint tileCoord);
	// չʾ��˸���ʺű�������ҵĶ���
	void showJumpUpBlinkCoin(CCPoint tileCoord);
	// չʾש�鱻С����¶�һ�µ�Ч��
	void showBlockJump(CCPoint tileCoord);
	// ����������´����С����ұ��������Ĺ����л�����Ž�ҵ�ש�����ƶ�����������showJumpBlock
	void showCoinJump(CCPoint tileCoord);
	// ��չʾ���˽��ש��/Ģ��ש������ƶ�����Ҫ�������ý��ש���Ǳ�������״̬����
	void resetCoinBlockTexture();
	// ����һֻĢ�������ݵ�ǰ�������״̬���ж�Ҫ���ɵ��Ǳ��Ģ�������ӵ�Ģ��
	void showNewMushroom(CCPoint tileCoord, BodyType _bodyType);
	// ����һֻ����Ģ��
	void showAddLifeMushroom(CCPoint tileCoord);

	// �ж�������Ƿ�Ե�Ģ��
	bool isMarioEatMushroom(CCPoint tileCoord);
	// �ж��Ƿ�ٵ�����Ģ��
	bool isMarioEatAddLifeMushroom(CCPoint tileCoord);

	// ������ͼ�ϵ����й����������
	void launchEnemyInMap();

	// ������ͼ�����е����
	void launchGadgetInMap();

	void update(float dt);
	// ֹͣ������ӵ��ĸ��£�Ȼ����ֹͣ�Լ��ĸ���
	void stopUpdateForHeroDie();

	void pauseGameMap();  // ��ͣ��ͼ�ϵĸ��ֻᶯ��
	void resumeGameMap();  // ���¶�����

	// �����������active״̬�еĹ������������⣬Ȼ����ֳ������߼�
	void enemyVSHero();

	// ��Ϸͼ������ϵ��tilemapͼ������ϵ֮���ת��
	CCPoint positionToTileCoord(CCPoint pos);
	CCPoint tilecoordToPosition(CCPoint tileCoord);

	// ����һ���µ��ӵ�������������
	void createNewBullet();

	// �ӵ���������ײ���
	void bulletVSEnemy();

	static CCGameMap *_gameMap;
	static CCGameMap* getGameMap();
private:
	void deleteOneMushPointFromArray(CCPoint tileCoord);
	void clearItem();
	void clearSpriteArray();
	void extraInit();
	void initObjects();
	bool itemCoordArrayContains(CCPoint tileCoord);
	bool mushroomPointContains(CCPoint tileCoord);
	CCPointArray *pItemCoordArray;
	CCArray *pSpriteArray;

	// ���Boss��׼���ı���
	CCPoint bridgeTileStartPos;
	int bridgeTileNums;
	CCEnemy *pBossEnemy;
	void initBridgeArray();
	CCSprite *pPrincess;  // ��������

	// ���ڶ�ש��������ɹ������ʱ����
	CCPoint enemyTilePos;
	CCEnemy *pRandomEnemy;
	void randomShowEnemy();
	void randomLaunchEnemy();
};

#endif