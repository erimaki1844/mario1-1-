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
	E_FLOORBLOCK,
	E_KAIBLOCK,
	E_BRICKBREAK
};

class Block : public ObjectBase
{
private:
	float pos;
	float g_speed;
	bool block_state;
	bool anim_flg;
	Vector2D move;
	eBlockType block_type;
public:
	Block();
	~Block();

	virtual void Initialize() override;
	virtual void Update(Vector2D diff) override;
	virtual void Draw() override;
	virtual int Finalize() override;
	virtual void SetType(int handle) override;
	virtual int GetPreset() override;
	
private:
	virtual void OnHit(ObjectBase* obj) override;
	void BlockAnim();
};