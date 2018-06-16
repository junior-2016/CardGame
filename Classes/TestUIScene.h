#pragma once
#ifndef TEST_UI_SCENE_H
#define TEST_UI_SCENE_H
#include"GameConfig.h"

#include"BaseCard.h"

#include"Player.h"
#include"BaseEnemy.h"
#include"Target.h"

#include"CardLayer_.h"

#include"BaseBehaviour.h"

#include"AIManager.h"


class TestUIScene : public cocos2d::Layer {
public:
	static Scene* createScene();
	CREATE_FUNC(TestUIScene);
private:

	virtual bool init();
	virtual void update(float dt);

	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);

	virtual void onMouseMove(EventMouse* pEvent);	//���ڼ�����ͣ���ڵ�����ͼ��ʱ�ĺ���

	virtual void onEnterTransitionDidFinish();

	Size size;
private:
};
#endif // TEST_UI_SCENE_H
