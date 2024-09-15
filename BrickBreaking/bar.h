#pragma once
#ifndef BAR_H_
#define BAR_H_

#include "Brick.h"

extern void OnSplitBallEvnet();
extern void OnLengthenBarEvent();
extern void OnClingBallEvent(); 

class Ball;

class Bar
{
private:
	Vector2 pos;
	RECT rect;
	float length;

public:
	int ballNum = -1;
	bool isClingBall = false;

	Bar();
	void SetPosX(float x);
	void SetLength(float addLength);
	void ConnectBall(int ballNum);
	Vector2 GetPos();
	RECT GetRect();
	float GetLength();
	bool IsClingBall();
	void Update();
	void Draw(HDC& hdc);
	void ShotBall();
	bool IsCheckCollisionItem(Item& item);
	void OnUseItem(ItemType type);
};

#endif