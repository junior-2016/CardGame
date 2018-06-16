#pragma once

#ifndef _GAME_SCENE_DEMO_H_
#define _GAME_SCENE_DEMO_H_

#include"GameConfig.h"

#include"BaseCard.h"

#include"Player.h"
#include"BaseEnemy.h"
#include"Target.h"

#include"CardLayer_.h"

#include"BaseBehaviour.h"

#include"AIManager.h"


class GameSceneDemo :public Layer {
public:
	static Scene* createScene();
	CREATE_FUNC(GameSceneDemo);

	Target getTarget(BaseCard* card);	//���ݿ��Ʒ���Target
	Target getTarget(BaseBehaviour* behaviour);	//����TargetNeed����Target

	Vector<BaseEnemy*> m_enemys;
	BaseEnemy* m_current_enemy;	//���浱ǰĿ�����

	int getCurrentRound() { return m_current_rounds; }

	void explode_on_enemy(Target target);
private:
	void endRound( Ref* pRef );
	void updateCostLabel();

	virtual bool init();
	virtual void update(float dt);

	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);

	virtual void onMouseMove(EventMouse* pEvent);	//���ڼ�����ͣ���ڵ�����ͼ��ʱ�ĺ���

	virtual void onEnterTransitionDidFinish();
private:
	int m_current_rounds;

	Size m_visibleSize;

	Size m_gl_size; // GL����ϵ�´��ڴ�С

	CardLayer_* m_card_layer;

	Player * m_player;

	AIManager* m_ai_manager;

	Label* m_cost_label;
};

#endif // !_GAME_SCENE_DEMO_H_
