#include"GameSceneDemo.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;
using namespace cocos2d::experimental;

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

	/*----------------- 3d 支持 ------------------- */
	FileUtils::getInstance()->addSearchPath("res/Particle3D/materials");
	FileUtils::getInstance()->addSearchPath("res/Particle3D/scripts");
	m_gl_size = Director::getInstance()->getWinSize();
	Camera * _camera = Camera::createPerspective(30.0f, m_gl_size.width / m_gl_size.height, 1.0f, 1000.0f);
	_camera->setPosition3D(Vec3(0.0f, 0.0f, 100.0f));
	_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	_camera->setCameraFlag(CameraFlag::USER1);
	this->addChild(_camera);
	
	/*
	auto shader = GLProgram::createWithFilenames("shader/cube_map.vert",
		"shader/cube_map.frag");
	auto state = GLProgramState::create(shader);
	TextureCube *  _textureCube = TextureCube::create("skybox/4/Left.jpg",
		"skybox/4/Right.jpg",
		"skybox/4/Top.jpg",
		"skybox/4/Bottom.jpg",
		"skybox/4/Front.jpg",
		"skybox/4/Back.jpg");
	Texture2D::TexParams tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	tRepeatParams.wrapS = GL_MIRRORED_REPEAT;
	tRepeatParams.wrapT = GL_MIRRORED_REPEAT;
	_textureCube->setTexParameters(tRepeatParams);
	state->setUniformTexture("u_cubeTex", _textureCube);
	// add skybox
	Skybox * _skyBox = Skybox::create();
	_skyBox->setCameraMask((unsigned short)CameraFlag::USER1);
	_skyBox->setTexture(_textureCube);
	this->addChild(_skyBox);
	*/

	// add particle
	auto particle = PUParticleSystem3D::create("explosionSystem.pu");
	particle->setCameraMask((unsigned short)CameraFlag::USER1);
	particle->setScale(1.0f);
	particle->setPositionZ(10.0); // 设置z坐标大于0平面
	particle->setPosition(0.0f, 0.0f);
	// 这个特效的位置是在3维空间透视后的位置...
	// 范围大概是 : width [-40,40] height [-20,20] ...
	this->addChild(particle, 1.0 , 101);
	// add bg music
	AudioEngine::preload("audio/bg.mp3", [&](bool isSuccess) {
		if (isSuccess) {
			CCLOG("success\n");
		}
		else {
			CCLOG("unsuccess\n");
		}
	});

	m_current_rounds = 1;

	Sprite * bg = Sprite::create("skybox/bg.png");  
	this->addChild(bg);

	//初始化玩家
	m_player = Player::create("character/player.png", 80, 0, 0, 0, 0, 3);
	m_player->setPosition( m_visibleSize.width * 1 / 4 , m_visibleSize.height / 2 );
	m_player->setScale(0.07);
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
	float volume = 1.0f;
	AudioEngine::play2d("audio/bg.mp3", loop, volume);
}

// TODO: 有待修改...
void GameSceneDemo::explode_on_enemy(Target target) {
	auto particle = (PUParticleSystem3D *)this->getChildByTag(101);
	for (BaseEnemy * e : target.enemys) {
		Vec2 position = e->getPosition();
		float k1 = (40.0 - (-40.0)) / (m_gl_size.width);
		float k2 = (20.0 - (-20.0)) / (m_gl_size.height);
		position.x = k1 * position.x + (-40.0) ;
		position.y = k2 * position.y + (-20.0);
		particle->setPosition(position);
		particle->startParticleSystem();
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

	int current_cost = m_player->getCost();
	if ( card->getCardCost() > current_cost) {
		target.isReady = false;
		CCLOG("I do not have enough cost");
	}
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
		m_player->changeCost( -card->getCardCost());
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
}

void GameSceneDemo::updateCostLabel() {
	__String *cache = __String::create(Value(m_player->getCost()).asString() + " /" + Value(m_player->getMaxCost()).asString());
	m_cost_label->setString(cache->getCString());
}