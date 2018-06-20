#include"GameSceneDemo.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
#include "audio/include/AudioEngine.h"
#include "spine/spine-cocos2dx.h"
#include "spine/spine.h"

using namespace cocos2d;
using namespace cocos2d::experimental;
using namespace spine;

Scene* GameSceneDemo::createScene() {
	auto scene = Scene::create();
	auto layer = GameSceneDemo::create();
	scene->addChild(layer);
	return scene;
}

bool GameSceneDemo::init() {
	if (!Layer::init()) {
		return false;
	}
	m_visibleSize = _director->getVisibleSize();

	auto emitter = CCParticleFire::create();
	emitter->setDuration(0.5);
	emitter->stopSystem();
	this->addChild(emitter,1,101);

	m_current_rounds = 1;

	Sprite * bg = Sprite::create("skybox/bg.png");
	bg->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
	bg->setOpacity(155);
	this->addChild(bg);

	//初始化玩家
	m_player = Player::create("character/player.png", 80, 0, 0, 0, 0, 3);
	m_player->setPosition( m_visibleSize.width * 1 / 4 , m_visibleSize.height / 2 );
	m_player->setScale(0.06);
	this->addChild(m_player , 0 , "player");

	//初始化敌人1
	auto enemy_1 = BaseEnemy::create("character/enemy.png", 100, 0, 0, 0 , EnemyType::Demo_Enemy);
	enemy_1->setPosition(m_visibleSize.width * 3 / 4, m_visibleSize.height / 2);
	this->addChild(enemy_1);
	m_enemys.pushBack(enemy_1);

	auto enemy_2 = BaseEnemy::create("character/enemy.png", 100, 0, 0, 0 , EnemyType::Demo_Enemy);
	enemy_2->setPosition(m_visibleSize.width * 2 / 4, m_visibleSize.height / 2);
	this->addChild(enemy_2);
	m_enemys.pushBack(enemy_2);

	//初始化回合Label
	m_cost_label = Label::create( "", "Arical", 24);
	m_cost_label->setPosition(
		m_visibleSize.width * (1 - touchableSize.width) / 2, 
		m_visibleSize.height * touchableSize.height + m_cost_label->getContentSize().height
	);
	this->addChild(m_cost_label);

	//模拟回合结束的按钮
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameSceneDemo::endRound, this));

	closeItem->setPosition(Vec2( m_visibleSize.width - closeItem->getContentSize().width / 2,
		closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//初始化卡牌
	// 如果后面用杀戮尖塔的卡牌,可以考虑用spine直接对卡牌图片文件通过位置切割... 
	std::vector<CardID> cardIDs;
	cardIDs.push_back(CardID::Strike);
	cardIDs.push_back(CardID::Strike);
	cardIDs.push_back(CardID::Strike);
	cardIDs.push_back(CardID::Strike);
	cardIDs.push_back(CardID::Defend);
	cardIDs.push_back(CardID::Defend);
	cardIDs.push_back(CardID::Defend);
	cardIDs.push_back(CardID::Inner_Strength);
	cardIDs.push_back(CardID::Armor_Breaking);
	cardIDs.push_back(CardID::Blessing_Of_Prince_Daughter);
	cardIDs.push_back(CardID::Blessing_Of_Prince_Daughter);
	cardIDs.push_back(CardID::Health_Potion);
	cardIDs.push_back(CardID::Mortal_Strike);
	cardIDs.push_back(CardID::Preparation);
	cardIDs.push_back(CardID::Cleave);
	cardIDs.push_back(CardID::Split);
	cardIDs.push_back(CardID::Pot_of_Avarice);
	cardIDs.push_back(CardID::Pot_of_Avarice);
	cardIDs.push_back(CardID::Burst_Limit);
	cardIDs.push_back(CardID::Tie);
	cardIDs.push_back(CardID::Magic_Missile);
	cardIDs.push_back(CardID::Magic_Missile);
	cardIDs.push_back(CardID::Magic_Book);
	cardIDs.push_back(CardID::Magic_Book);
	cardIDs.push_back(CardID::Magic_Book);
	cardIDs.push_back(CardID::Enchanted_Weapon);
	cardIDs.push_back(CardID::Arcane_Breath);

	m_card_layer = CardLayer_::create( cardIDs );
	this->addChild(m_card_layer);

	for (int i = 0; i < INIT_HAND_CARD_NUM; i++) {
		m_card_layer->drawCard();
	}

	m_ai_manager = AIManager::create(m_player, m_enemys);
	this->addChild(m_ai_manager);

	//鼠标指针
	auto cursor = Sprite::create("ui/cursor.png");
	this->addChild(cursor, 1, 255);

	// add animation
	FileUtils::getInstance()->addSearchPath("res/character/monsters");
	FileUtils::getInstance()->addSearchPath("res/character/characters");
	auto skeletonNode = spine::SkeletonAnimation::createWithFile
	("theBottom/boss/guardian/skeleton.json", "theBottom/boss/guardian/skeleton.atlas", 0.5);//0.2是设置图片的缩放比例
	skeletonNode->setPosition(Point(400, 200));
	skeletonNode->setAnimation(0, "idle", true);//true是指循环播放walk动作
	this->addChild(skeletonNode);

	//添加触摸监听器
	auto listener = EventListenerTouchOneByOne::create();
	auto listener1 = EventListenerMouse::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameSceneDemo::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameSceneDemo::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameSceneDemo::onTouchEnded, this);
	listener1->onMouseMove = CC_CALLBACK_1(GameSceneDemo::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
	
	this->scheduleUpdate();

	return true;
}

void GameSceneDemo::onEnterTransitionDidFinish() {
	bool loop = true;
	float volume = 0.5f;
	AudioEngine::preload("audio/boss1.mp3");
	AudioEngine::play2d("audio/boss1.mp3", loop, volume);
}

void GameSceneDemo::play_music(std::string name) {
	AudioEngine::play2d(name, false, 1.0);
}
void GameSceneDemo::explode_on_enemy(Target target) {
	auto particle = (CCParticleFire *)this->getChildByTag(101);
	for (BaseEnemy * e : target.enemys) {
		particle->setPosition(e->getPosition());
		particle->resetSystem();
	}
}

Target GameSceneDemo::getTarget(BaseBehaviour* behaviour) {
	Target target;
	target.isReady = true;
	switch (behaviour->m_target_need.enemy_need) {
		case 0:
			break;
		case 1:target.enemys.pushBack((BaseEnemy*)behaviour->getParent());
			break;
		case -1:
			target.enemys = m_enemys;
			break;
	}
	if (behaviour->m_target_need.player_need) {
		target.players.pushBack(m_player);
	}
	if (behaviour->m_target_need.card_layer_need) {
		target.cardLayers.pushBack(m_card_layer);
	}
	return target;
}
Target GameSceneDemo::getTarget(BaseCard* card) {
	Target target;
	target.isReady = true;
	if (card == NULL) {
		target.isReady = false;
		CCLOG("card is a null pointer!!!\n");
	} else {
		int current_cost = m_player->getCost();
		if (card->getCardCost() > current_cost) {
			target.isReady = false;
			CCLOG("I do not have enough cost");
		} else {
			switch (card->m_target_need.enemy_need) {
			case 0:
				break;

			case 1:
				if (m_current_enemy == NULL) {
					target.isReady = false;
				}
				else {
					target.enemys.pushBack(m_current_enemy);
				}
				break;

			case -1:
				target.enemys = m_enemys;
				break;
			}

			if (card->m_target_need.player_need) {
				target.players.pushBack(m_player);
			}
			if (card->m_target_need.card_layer_need) {
				target.cardLayers.pushBack(m_card_layer);
			}

			if (target.isReady) {
				m_player->changeCost(-card->getCardCost());
			}
		}
	}
	return target;
}

void GameSceneDemo::update(float dt) {
	updateCostLabel();
}

void GameSceneDemo::endRound(Ref* pRef) {
	m_current_rounds++;

	m_ai_manager->effect_when_round_end(m_current_rounds);	//结束回合更新AI、同时更新敌人
	m_player->effect_when_round_end();	//结束回合更新玩家
	m_card_layer->effect_when_round_end();	//结束回合使卡牌Layer抽牌
}

bool GameSceneDemo::onTouchBegan(Touch* pTouch, Event* pEvent) {
	CCLOG("GAMESCENE-TOUCH");
	//m_current_enemy = NULL;
	/*if (m_card_layer->getIsSelect()) {
		return true;
	}
	return false;*/
	return true;
}
void GameSceneDemo::onTouchMoved(Touch* pTouch, Event* pEvent) {
	//CCLOG("GAMESCENE-TOUCH-MOVED");

	/*
	// 下面这一步要加一个特效: 当某个敌人被选中要标出...
	// (但是只对那些针对特定敌人的卡牌需要做... -> 这一块应该需要CardLayer_提供接口来判断...)
	for (BaseEnemy* enemy : m_enemys) {
		if (enemy->getBoundingBox().containsPoint( pTouch->getLocation() )) {
			m_current_enemy = enemy;
		}
	}
	*/
}
void GameSceneDemo::onTouchEnded(Touch* pTouch, Event* pEvent) {
	/*
	CCLOG("GAMESCENE-TOUCH-END");
	for (BaseEnemy* enemy : m_enemys) {
		if (enemy->getBoundingBox().containsPoint( pTouch->getLocation() )) {
			m_current_enemy = enemy;
			return;
		}
	}
	m_current_enemy = NULL;
	*/
}


void GameSceneDemo::onMouseMove(EventMouse* pEvent) {
	//for (BaseEnemy* enemy : m_enemys) {
	//	if (enemy->getBoundingBox().containsPoint(Vec2(pEvent->getCursorX(), pEvent->getCursorY()))) {
	//		m_current_enemy = enemy;
	//		return;
	//	}
	//}
	//m_current_enemy = NULL;
	auto cursor = (Sprite*)this->getChildByTag(255);
	if (pEvent->getCursorX() <= 0 || pEvent->getCursorX() >= m_visibleSize.width ||
		pEvent->getCursorY() <= 0 || pEvent->getCursorY() >= m_visibleSize.height) {
		cursor->setVisible(false);
	} else {
		cursor->setVisible(true);
		cursor->setPosition(pEvent->getCursorX(), pEvent->getCursorY());
	}
}

void GameSceneDemo::updateCostLabel() {
	__String *cache = __String::create(Value(m_player->getCost()).asString() + " /" + Value(m_player->getMaxCost()).asString());
	m_cost_label->setString(cache->getCString());
}