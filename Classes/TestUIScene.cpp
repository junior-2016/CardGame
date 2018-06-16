#include "TestUIScene.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
#include "SimpleAudioEngine.h"

#include "platform/CCPlatformConfig.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;
using namespace CocosDenshion;

Scene* TestUIScene::createScene() {
	auto scene = Scene::create();
	auto layer = TestUIScene::create();
	scene->addChild(layer);
	return scene;
}

bool TestUIScene::init() {
	if (!Layer::init()) {
		return false;
	}

	FileUtils::getInstance()->addSearchPath("res/Particle3D/materials");
	FileUtils::getInstance()->addSearchPath("res/Particle3D/scripts");

	size = Director::getInstance()->getWinSize();
	Camera* _camera = Camera::createPerspective(30.0f, size.width / size.height, 1.0f, 1000.0f);
	_camera->setPosition3D(Vec3(0.0f, 0.0f, 100.0f));
	_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	_camera->setCameraFlag(CameraFlag::USER1);
	this->addChild(_camera);

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
	//_skyBox->setScale(700.f);

	this->addChild(_skyBox);

	auto rootps = PUParticleSystem3D::create("lineStreak.pu", "pu_mediapack_01.material");
	rootps->setCameraMask((unsigned short)CameraFlag::USER1);
	rootps->setScale(0.3f);
	rootps->setPosition(-25.0f, 0.0f); 
	// 这个特效的位置是在3维空间透视后的位置...
	// 范围大概是 : width [-40,40] height [-20,20] ...
	rootps->startParticleSystem();
	this->addChild(rootps,1,101);

	auto roopts1 = PUParticleSystem3D::create("explosionSystem.pu");
	roopts1->setCameraMask((unsigned short)CameraFlag::USER1);
	roopts1->setScale(1.0f);
	roopts1->startParticleSystem();
	this->addChild(roopts1, 1, 102);

	AudioEngine::preload("audio/bg.mp3", [&](bool isSuccess) {
		if (isSuccess) {
			CCLOG("success\n");
		}
		else {
			CCLOG("unsuccess\n");
		}
	});
	

	//添加触摸监听器
	auto listener = EventListenerTouchOneByOne::create();
	auto listener1 = EventListenerMouse::create();
	listener->onTouchBegan = CC_CALLBACK_2(TestUIScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TestUIScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TestUIScene::onTouchEnded, this);
	listener1->onMouseMove = CC_CALLBACK_1(TestUIScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
	this->scheduleUpdate();
	return true;
}

void TestUIScene::update(float dt) {
	
}
void TestUIScene::onEnterTransitionDidFinish() {
	bool loop = true;
	float volume = 1.0f;
	AudioEngine::play2d("audio/bg.mp3",loop,volume);
}

bool  TestUIScene::onTouchBegan(Touch* pTouch, Event* pEvent) {
	
	return true;
}
void TestUIScene::onTouchMoved(Touch* pTouch, Event* pEvent) {

}
void TestUIScene::onTouchEnded(Touch* pTouch, Event* pEvent) {

}

void TestUIScene::onMouseMove(EventMouse* pEvent) {
	auto r = (PUParticleSystem3D *)this->getChildByTag(101);
	auto r1 = (PUParticleSystem3D *)this->getChildByTag(102);
	Vec2 position = Vec2(pEvent->getCursorX(), pEvent->getCursorY());
	// 做 [0,size.width] -> [-40,40]的映射... 线性映射..
	// [0,size.h] -> [-20,20]
	float k1 = (40.0 -(-40.0)) / (size.width);
	float k2 = (20.0-(-20.0))/(size.height);
	position.x = k1*position.x + (-40.0);
	position.y = k2*position.y + (-20.0);
	r->setPosition(position);
}
