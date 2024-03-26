#pragma once
#include"ObjectBase.h"

enum eItemAnim
{
	E_IDOL_ITEM,
	E_HIT_ITEM
};

enum eItemType
{
	E_1UP,
	E_SUPER,
	E_COIN
};

class Item : public ObjectBase
{
private:
	float g_speed;
	float start_pos;
	eItemType item_type;
	eItemAnim now_anim;

public:
	Item();
	~Item();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw(float diff) override;
	virtual void Finalize() override;
	virtual int GetPreset() override;

private:
	virtual void Movement() override;
	virtual void OnHit(ObjectBase* obj) override;
	void ChangeType(eItemType type);
};