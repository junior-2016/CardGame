#pragma once

#ifndef _CARD_LAYER__H_
#define _CARD_LAYER__H_

#include"GameConfig.h"
#include"BaseCard.h"
#include"CardSystem.h"

#define LINE_SPRITE_NUMBER 10 // ���������ϵ�ͼ�����

class CardLayer_ :public Layer {
public:
	static CardLayer_* create(std::vector<CardID>& cardIDs);

	BaseCard* drawFromCardLibrary();	//�ӳ��ƿ��ȡ����
	BaseCard* drawFromCardLibraryAtID(int index);	//���±�Ϊindex�ĳ��ƿ��ȡ����
	BaseCard* drawFromDisLibrary();	//�����ƿ��ȡ����
	BaseCard* drawFromDisLibraryAtID( int index );	//���±�Ϊindex�����ƿ��ȡ����

	bool drawCard();	//���ƿ��г�ȡ����
	bool drawCard(int num);	//���ƿ��г�ȡnum�ſ���
	bool drawCardAtID(int index);
	void drawFull();

	bool discard( bool isDestory );	//�����������
	bool discardAtID(int index , bool isDestory);	//��������
	void discardAll( bool isDestory );	//������������

	bool drawFromDisLibraryToHand();	//������������ƿ��г�ȡ���Ƶ�����
	bool drawFromDisLibraryToHand( int num );	//������������ƿ��г�ȡnum�ſ��Ƶ�����
	bool drawFromDisLibraryToCardLibrary();	//������������ƿ��г�ȡ���Ƶ����ƿ�
	bool drawFromDisLibraryToHandAtID( int index );	//������������ƿ��г�ȡ���Ƶ�����
	bool drawFromDisLibraryToCardLibraryatID( int index );	//������������ƿ��г�ȡ���Ƶ����ƿ�

	bool addCardToHandCard(BaseCard*& card);	//�������ӿ��Ƶĺ���
	void addCardToCardLibrary(BaseCard*& card);
	void addCardToDisLibrary(BaseCard*& card);

	int getSizeOfCardLibrary() { return m_card_library.size(); }
	int getSizeOfHandCard() { return m_hand_cards.size(); }
	int getSizeOfDisLibrary() { return m_discard_library.size(); }

	bool getIsSelect() { return m_isSelect; }
	BaseCard* getCurrentCard() { return m_current_card;  }

	void effect_when_round_end();
private:
	void updateLayout();	//���²���
	void checkCardLibrary(Ref* pSender);
	//void useCard();

	virtual bool init(std::vector<CardID> cardIDs);
	virtual void update(float dt);

	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);

	virtual void onMouseMove(EventMouse* pEvent);	//���ڼ�����ͣ���ڿ�����ͼ��ʱ�ĺ���

	void callfunc(BaseCard * card, Target target);

	void enemy_shake_action_callback();

	Vec2* getCubicBezierPoints(const Vec2 &origin, const Vec2 &control1, 
		const Vec2 &control2, const Vec2 &destination, unsigned int segments);
	void clearLine();

private:
	Size m_visibleSize;

	MenuItemImage* m_card_library_button;	//���ƿ⾫��
	Label* m_card_library_label;	//���ƿ�ʣ�࿨����
	MenuItemImage* m_dis_library_button;	//���ƿ⾫��
	Label* m_discard_library_label;	//���ƿ�ʣ�࿨����

	Vector<BaseCard*> m_card_library;	//���ƿ�
	Vector<BaseCard*> m_discard_library;	//���ƿ�
	Vector<BaseCard*> m_hand_cards;	//���ƿ�

	Vector<BaseCard*> m_destory_library;	//�ݻٵ��ƿ�

	BaseCard* m_current_card;	//��ǰѡ����

	int m_draw_when_round_end = 2;	//����ʱ��ȡ�����������ƿ����õ�Effect���Զ��������Ӱ�죬�Ӷ��ı�ÿ�غϳ�����

	bool m_isHovor;	    //�Ƿ�����
	bool m_isSelect;	//�Ƿ�ѡ����һ�ſ���

	bool m_isCurrentCanMove = true; // ��ѡ���ƿɷ��ƶ�
	bool m_isSendCard = false;  // ���Ϳ��ƽ׶β��ܶԿ��������κα䶯...

	BaseCard * m_oldSelectCard = NULL; // ֮ǰѡ��Ŀ���,ʹ�������Ϊ�˷�ֹ�����ͬһ�ſ����ƶ�ʱ��������������...

	Vec2 endPoint;
	Vec2 control1;
	Vec2 control2;
	bool m_isEnemyCanShake = true;

	Sprite ** line = new Sprite*[LINE_SPRITE_NUMBER];
};

#include"GameSceneDemo.h"
#endif // !_CARD_LAYER__H_