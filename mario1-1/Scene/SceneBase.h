#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"DxLib.h"
#include"../Object/ObjectBase.h"
#include<vector>
#include <iostream>
#include <fstream>
#include<stdio.h>
#include<string.h>

//�V�[�����
enum eSceneType
{
	E_TITLE,
	E_LOADING,
	E_MAIN,
	E_END,
	END
};

//���V�[���N���X
class SceneBase
{
protected:
	std::vector<ObjectBase*> obj;
	int score;
	int coin;
	int time;
	int life;

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

	//�^�C�}�[
	int GetTime()
	{
		return time;
	}

	void LoadDataCSV()
	{
		FILE* fp = nullptr;

		errno_t result = fopen_s(&fp, "Resource/dat/data.csv", "r");

		if (result != 0)
		{
			throw("CSV�t�@�C�����J���܂���ł���\n");
		}

		char buf[512];

		while (fgets(buf, 512, fp) != NULL)
		{
			//life�̒l��ǂݍ���
			if (buf[0] == 'l')
			{
				for (int i = 0; i < 10; i++)
				{
					if (buf[i] >= '0' && buf[i] <= '9')
					{
						this->life = buf[i] - '0';
					}
				}
			}
			//�X�R�A�̒l��ǂݍ���
			else if (buf[0] == 's')
			{
				for (int i = 0; i < 10; i++)
				{
					if (buf[i] >= '0' && buf[i] <= '9')
					{
						if (this->score >= 1)
						{
							this->score = this->score * 10 + (buf[i] - '0');
						}
						else this->score = buf[i] - '0';
					}
				}
			}
			//�R�C���̒l��ǂݍ���
			else if (buf[0] == 'c')
			{
				for (int i = 0; i < 10; i++)
				{
					if (buf[i] >= '0' && buf[i] <= '9')
					{
						if (this->coin >= 1)
						{
							this->coin = this->coin * 10 + (buf[i] - '0');
						}
						else this->coin = buf[i] - '0';
					}
				}
			}
		}

		//�t�@�C���N���[�Y
		fclose(fp);
	}
};