#pragma once
#include"ObjectBase.h"

class Pole : public ObjectBase
{
private:
	Vector2D move;  //ˆÚ“®—Ê

public:
	Pole();
	~Pole();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw(Vector2D diff) override;
	virtual void Finalize() override;

private:
	virtual void Movement() override;
	virtual void OnHit(ObjectBase* obj) override;
};