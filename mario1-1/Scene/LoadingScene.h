#pragma once

#include "SceneBase.h"

class LoadingScene :public SceneBase
{
private:
	int count;
	int life;

public:
	LoadingScene();
	virtual ~LoadingScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};