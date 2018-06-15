#include "TestUIScene.h"

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

	DrawNode * line = DrawNode::create();
	line->drawQuadBezier(Vec2(0, 0), Vec2(100, 100), Vec2(200, 200), 3, Color4F(255, 75, 96, 255));
	this->addChild(line);
	return true;
}

void TestUIScene::update(float dt) {

}

