#pragma once
#include"DxLib.h"
#include"../Object/ObjectBase.h"
#include<vector>

//シーン情報
enum eSceneType
{
	E_TITLE,
	E_LOADING,
	E_MAIN,
	END
};

//基底シーンクラス
class SceneBase
{
protected:
	std::vector<ObjectBase*> obj;
public:
	SceneBase() {}
	virtual ~SceneBase() {}

	//初期化処理
	virtual void Initialize() {}
	//更新処理
	virtual eSceneType Update()
	{
		return GetNowScene();
	}
	//描画処理
	virtual void Draw() const {}
	//終了時処理
	virtual void Finalize() {}

	//現在のシーン情報を取得
	virtual eSceneType GetNowScene() const = 0;

};