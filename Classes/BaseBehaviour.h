#pragma once

#ifndef _BASE_BEHAVIOUR_H_
#define _BASE_BEHAVIOUR_H_

#include"GameConfig.h"
#include"BaseEffect.h"
#include"Target.h"

struct IntentType {
	bool isAttack;
	bool isDefend;
	bool isStengthen;
	bool isCurse;
};
struct BehaviourDigital {
	int damage_digital = 0;
	int armor_digital = 0;
	BehaviourDigital(int d, int a) :damage_digital(d), armor_digital(a){}
	BehaviourDigital(){
		damage_digital = armor_digital = 0;
	}
};

//��������һ��ͼ�꣫˵����Ҳ���ǵ���ͷ�ϵ���ͼ
//����Vector<BaseEffect>����AIManager������ÿ�����˳��е�BaseBehaviour
class BaseBehaviour :public Node {
public:
	static BaseBehaviour* creaet();

	void updateIntent( std::string picPath );	//���Ը�����ͼ

	void addEffect(BaseEffect* effect) { m_effects.pushBack(effect); }	//���Effect
	void effect( Target& target );	//����Behaviour�ĺ���


	IntentType m_intent_tpye;	//���ʱ���������������ͼ��Ϣ
	TargetNeed m_target_need;	//���Բ���Target
	BehaviourDigital m_behaviour_digital;	//���Դ洢����Ϊ����ɵ�digital
	BehaviourDigital m_pre_digital;	//��������digital
private:
	virtual bool init();
	virtual void update(float dt);

	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
private:
	Sprite * m_icon_sprite;	//��ͼͼ��
	Label* m_tip_label;	//����/������ֵLabel

	std::string info;	//��ͼ˵��

	Vector<BaseEffect*> m_effects;
};

#endif // !_BASE_BEHAVIOUR_H_