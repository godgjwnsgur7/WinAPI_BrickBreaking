#pragma once
#ifndef CUSTOMBALL_H_
#define CUSTOMBALL_H_

#include "Bar.h"

class Ball
{
private:
	void Init(Vector2 initPos, Vector2 velocity, float radius, float moveSpeed);

public:
	Vector2 pos;
	Vector2 velocity;
	float radius;
	float moveSpeed;
	bool isCollision = false;
	bool isClingBar = false;
	bool isUsing = false;

	Ball();
	Ball(Vector2 initPos, Vector2 velocity, float radius, float moveSpeed);
	void SpawnBall();
	void SpawnBall(Vector2 initPos, Vector2 velocity, float radius, float moveSpeed);
	void SpawnBall(const Ball& ball);
	void SetPosX(float x);
	void Update();
	void Draw(HDC& hdc);
	bool CheckCollisionBar(Bar& bar);
	void CheckCollisionBrick(Brick& brick);
};

#endif