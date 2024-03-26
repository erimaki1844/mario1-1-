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
	E_CLAYPIPE
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
	int score;
	float angle;
	bool state;
	bool is_active;
	int anim_count;
	int anim;
	eObjectType obj_type;
	eObjectType hit_type{};
	eDirection direction;

public:
	ObjectBase() {};
	~ObjectBase() {};

	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw(float diff) {};
	virtual void Finalize() {};
	virtual void OnHit(ObjectBase* obj) {};
	virtual void Movement() {};
	virtual int GetPreset() { return 0; };
	virtual void SetLocation(Vector2D location) {};
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
	float GetOffSet()
	{
		return this->location.x;
	}
	bool GetIsActive()
	{
		return this->is_active;
	}
	bool GetState()
	{
		return this->state;
	}
	int GetScore() {};
};