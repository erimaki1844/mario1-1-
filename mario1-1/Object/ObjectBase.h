#pragma once
#include"DxLib.h"
#include"../Utility/Vector2D.h"

#define GRAVITY 0.9f

enum eObjectType
{
	E_PLAYER,
	E_ENEMY,
	E_BLOCK,
	E_ITEM,
	E_POLE,
	E_CLAYPIPE,
	E_BAKUHA
};

enum eDirection
{
	E_RIGHT,
	E_LEFT
};

class ObjectBase
{
protected:
	Vector2D box_size;
	Vector2D location;
	int image[10];
	int se[10];
	int num_img[15];
	int score;
	float angle;
	float g_speed;
	bool state;
	bool is_active;
	bool end_flg;
	int anim_count;
	int anim;
	Vector2D overlap;
	eObjectType obj_type;
	eObjectType hit_type{};
	eDirection direction;

public:
	ObjectBase() {};
	~ObjectBase() {};

	virtual void Initialize() {};
	virtual void Update(Vector2D diff) {};
	virtual void Draw() {};
	virtual int Finalize() { return 0; };
	virtual void OnHit(ObjectBase* obj) {};
	virtual void Movement() {};
	virtual int GetPreset() { return 0; };
	virtual void SetPreset(int preset) {};
	void SetLocation(Vector2D location) 
	{
		this->location = location;
	};
	virtual void SetType(int handle) {};
	eObjectType GetObjectType()
	{ 
		return obj_type; 
	}
	Vector2D GetLocation()
	{
		return location;
	}
	Vector2D GetSize()
	{
		return box_size;
	}
	virtual Vector2D GetOffSet()
	{
		return 0;
	}
	bool GetIsActive()
	{
		return this->is_active;
	}
	bool GetState()
	{
		return this->state;
	}
	bool GetEndFlg()
	{
		return this->end_flg;
	}
	int GetScore()
	{
		return this->score;
	}
	float GetG_SPEED()
	{
		return this->g_speed;
	}
};