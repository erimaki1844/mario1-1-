#pragma once
#include"ObjectBase.h"

enum ePlayerType 
{
	SUPER,
	NOMAL,
	POWER_DOWN,
	POWER_UP
};

enum ePlayerAnim
{
	E_IDOL,
	E_HIT,
	E_RUN,
	E_JUMP,
	E_BRAKE,
	E_SQUAT,
	E_GAMEOVER,
	E_CLING,
	E_GOAL
};

class Player : public ObjectBase
{
private:
	int bgm;
	int count;
	int flash_count;
	int anim_count2;
	float speed;
	float jump_power;
	float start_pos;
	Vector2D move;
	ePlayerAnim now_anim;
	ePlayerType player_type;
	ePlayerType fall;

public:
	Player();
	~Player();

	virtual void Initialize() override;
	virtual void Update(Vector2D diff) override;
	virtual void Draw() override;
	virtual int Finalize() override;
	virtual Vector2D GetOffSet() override;
	virtual int GetPreset() override;

private:
	virtual void Movement() override;
	virtual void OnHit(ObjectBase* obj) override;
	void ChangeAnim(ePlayerAnim anim);
	void ChangeType(ePlayerType type);
	void PlayerAnim();
};