#pragma once
#include "SceneBase.h"

class LoadingScene :public SceneBase
{
private:
	int count;
	int life;
	int image[10];
	int num_img[15];

public:
	LoadingScene();
	virtual ~LoadingScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;

private:
	void LoadDataCSV();
};