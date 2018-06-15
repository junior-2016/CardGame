#pragma once

#ifndef _BASE_CARD_H_
#define _BASE_CARD_H_

#include"GameConfig.h"
#include"BaseEffect.h"
#include"Box2D\Box2D.h"
#include"CardID.h"
#include"Target.h"
#include<vector>

//ֻ����������ֵ�ǻ������Ҹı��
//��˿�����ΪBaseCard��һ���֣���¶���ⲿ�޸� 
struct CardBaseProperty{
	int damage_number = 0;
	int armor_number = 0;
	int magic_number = 0;	//�������
	CardBaseProperty(){
		damage_number = armor_number = magic_number = 0;
	}
	CardBaseProperty(int d, int a, int m) :damage_number(d), armor_number(a), magic_number(m){}
};
enum CardType {
	Attack_Card,//������
	Skill_Card,	//������
	Magic_Card	//ħ����
};

class BaseCard : public cocos2d::Node {
public:
	//ID�����֡�˵�������á����Ʊ���
	static BaseCard * create(CardID cardID, std::string name, std::string info, std::string cost, std::string picPath, b2World* world);


	void effect(Target& target);
	void addEffect(BaseEffect* effect);
	void updateCardInfo();

	//void setPlayerNeed(int opt) { m_player_need = opt; }
	//void setEnemyNeed(int opt) { m_enemy_need = opt; }
	//void setCardLayerNeed(int opt) { m_card_layer_need = opt; }

	//int getPlayerNeed() { return m_player_need; }
	//int getEnemyNeed() { return m_enemy_need; }
	//int getCardLayerNeed() { return m_card_layer_need; }

	//void setIsNeedPlayer(bool flag) { m_isNeedPlayer = flag; }
	//void setIsNeedTarget(bool flag) { m_isNeedTarget = flag;}
	//bool getIsNeedPlayer() { return m_isNeedPlayer; }
	//bool getIsNeedTarget() { return m_isNeedTarget; }

	CardID getCardID() { return m_card_ID; }
	int getCardCost() { return Value(m_cardCost).asInt(); }

	Sprite* getCardSprite() { return m_cardSprite; }
	b2Body* getBody() { return m_body; }

	CardBaseProperty m_Property;
	CardBaseProperty m_pre_property;
	CardType m_card_type;

	TargetNeed m_target_need;

	void moveTo(Vec2 position, float time);
	void rotateTo(float rotation, float time);

	virtual Size getContentSize() { return m_cardSprite->getContentSize(); }
private:
	BaseCard();
	~BaseCard();

	virtual bool init(CardID cardID, std::string name, std::string info , std::string cost, std::string picPath, b2World* world);
	virtual void update(float dt);

	void updateCardProperty(Ref* pSender);

	//���ڼ�����ͣ���ڿ�����ͼ��ʱ�ĺ���
	virtual void onMouseMove(EventMouse* pEvent);
private:
	Sprite * m_cardSprite;	//���ƾ���
	b2Body* m_body;

	CardID m_card_ID;

	//���á����֡�˵��
	std::string m_cardCost;
	int m_pre_cardCost;
	std::string m_cardName;
	std::string m_cardInfo;

	Label* m_cardCost_label;
	Label* m_cardName_label;
	Label* m_cardInfo_label;

	//���Ա�ʾ���ſ������е�Effect����ʹ�ÿ���ʱ������תΪ�������е�Effect��������effect����
	Vector<BaseEffect*> m_effect_vector;
};
#endif // !_BASE_CARD_H_
