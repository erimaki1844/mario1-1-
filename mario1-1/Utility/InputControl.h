#pragma once

#include "Vector2D.h"

#define D_BUTTON_MAX	(16)	// �{�^���ő吔

//���͊Ǘ��@�\
class InputControl
{
private:
	static bool now_button[D_BUTTON_MAX]; //���݃t���[�����͒l
	static bool old_button[D_BUTTON_MAX]; //�ߋ��t���[�����͒l
	static float trigger[2]; //���E�g���K�[���͒l
	static Vector2D stick[2]; //���E�X�e�B�b�N���͒l

public:
	static void Update(); //�X�V����

	//�{�^�����͎擾����
	static bool GetButton(int button);  //���������Ă����
	static bool GetButtonDown(int button); //�������u��
	static bool GetButtonUp(int button); //�b�����u��

	//�g���K�[���͎擾����
	static float GetLeftTrigger(); //���g���K�[
	static float GetRightTrigger();  //�E�g���K�[

	//�X�e�B�b�N���͎擾����
	static Vector2D GetLeftStick();  //���X�e�B�b�N
	static Vector2D GetRightStick(); //�E�X�e�B�b�N

private:
	//�{�^���z��͈̓`�F�b�N
	static bool CheckButtonRange(int button);
};