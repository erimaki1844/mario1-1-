#include"ObjectBase.h"
#pragma once

enum eEnemyType
{
	E_GOOMBA,
	E_KOOPTROOPA,
	E_KOOPTROOPA_HIDE
};

enum eEnemyAnim
{
	E_IDOL_ENEMY,
	E_HIT_ENEMY
};

class Enemy : public ObjectBase
{
private:
	float speed;
	float g_speed; //—Ž‰º‘¬“x
	int count;
	eEnemyType enemy_type;
	eEnemyAnim now_anim;

public:
	Enemy();
	~Enemy();

	virtual void Initialize() override;
	virtual void Update(Vector2D diff) override;
	virtual void Draw() override;
	virtual int Finalize() override;
	virtual int GetPreset() override;
	virtual void SetType(int handle) override;

private:
	virtual void OnHit(ObjectBase* obj) override;
	virtual void Movement() override;
	void ChangeAnim(eEnemyAnim anim);
	void ChangeType(eEnemyType type);
};