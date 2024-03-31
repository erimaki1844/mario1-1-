#pragma once
#include"ObjectBase.h"

enum eBlockType
{
	E_NONE,
	E_NOMAL,
	E_BRICK,
	E_ITEMBLOCK,
	E_HIDEBLOCK,
	E_ITEMBRICK,
	E_FLOORBLOCK
};

class Block : public ObjectBase
{
private:
	float pos;
	bool block_state;
	bool anim_flg;
	eBlockType block_type;
public:
	Block();
	~Block();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw(Vector2D diff) override;
	virtual int Finalize() override;
	virtual void SetLocation(Vector2D location) override;
	virtual void SetType(int handle) override;
	bool GetState();
private:
	virtual void OnHit(ObjectBase* obj) override;
};