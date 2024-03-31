#pragma once

class UI
{
private:
	int time;
	int score;
	int coin;
	int anim;
	int anim_count;
	int num_img[15];
	int coin_img[4];
	int str_img[5];

public:
	UI();
	~UI();

	void Initialize();
	void Update(int time,int score,int coin);
	void Draw();
	void Finalize();
	static int Conversion(int score,int num);
};