#pragma once
#include"ObjectBase.h"

class ClayPipe : public ObjectBase
{
private:
	int length; //�y�ǂ̒���
public:
	ClayPipe();
	~ClayPipe();

	virtual void Initialize() override;
	virtual void Update(Vector2D diff) override;
	virtual void Draw() override;
	virtual int Finalize() override;
	virtual void SetPreset(int preset) override;
};