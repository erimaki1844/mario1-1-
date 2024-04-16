#pragma once

#include "SceneBase.h"


class GameMainScene :public SceneBase
{
private:
	int bgm;
	int background_img;
	int siro_img[2];
	int bakuha_count;
	int se[2];
	int fps;
	bool goal_flg;
	bool bakuha_flg;
	
	Vector2D pos;  //城の位置
	Vector2D diff_location; //オブジェクトのplayerとのズレ
	float  flag_pos; //フラッグの位置

public:
	GameMainScene();
	virtual ~GameMainScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;

private:
	bool IsHitCheck(ObjectBase* obj1, ObjectBase* obj2);
	void LoadStageCSV();
};