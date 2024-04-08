#pragma once
#include"ObjectBase.h"

class Bakuha : public ObjectBase
{
private:
	int bakuha_type;

public:
	Bakuha();
	~Bakuha();

	virtual void Initialize() override;
	virtual void Update(Vector2D diff) override;
	virtual void Draw() override;
	virtual int Finalize() override;
	virtual void SetType(int handle) override;
};