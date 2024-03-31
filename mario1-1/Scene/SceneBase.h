#pragma once
#include"DxLib.h"
#include"../Object/ObjectBase.h"
#include<vector>

//�V�[�����
enum eSceneType
{
	E_TITLE,
	E_LOADING,
	E_MAIN,
	END
};

//���V�[���N���X
class SceneBase
{
protected:
	std::vector<ObjectBase*> obj;
	int score;
	int coin;
public:
	SceneBase() {}
	virtual ~SceneBase() {}

	//����������
	virtual void Initialize() {}
	//�X�V����
	virtual eSceneType Update()
	{
		return GetNowScene();
	}
	//�`�揈��
	virtual void Draw() const {}
	//�I��������
	virtual void Finalize() {}

	//���݂̃V�[�������擾
	virtual eSceneType GetNowScene() const = 0;

	//�X�R�A
	int GetScore()
	{
		return score;
	}

	//�R�C��
	int GetCoin()
	{
		return coin;
	}
};