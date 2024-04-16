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
	float start_pos;
	bool display_flg;
	eItemType item_type;
	eItemAnim now_anim;

public:
	Item();
	~Item();

	virtual void Initialize() override;
	virtual void Update(Vector2D diff) override;
	virtual void Draw() override;
	virtual int Finalize() override;
	virtual int GetPreset() override;

private:
	virtual void Movement() override;
	virtual void OnHit(ObjectBase* obj) override;
	virtual void SetType(int handle) override;
	void ChangeType(eItemType type);
};