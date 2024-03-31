#pragma once
#include"ObjectBase.h"

class ClayPipe : public ObjectBase
{
private:
	int length; //“yŠÇ‚Ì’·‚³
public:
	ClayPipe();
	~ClayPipe();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw(Vector2D diff) override;
	virtual int Finalize() override;
	virtual void SetPreset(int preset) override;
};