#pragma once
#ifndef BRICK_H_
#define BRICK_H_

#include "Item.h"

extern void TrySpawnItem(Vector2 spawnPos);

class Brick
{
private:
	int count;
	Vector2 pos;
	RECT rect;
	bool isCollision = false;
public:
	Brick();
	Brick(int count);
	int GetCount();
	Vector2 GetPos();
	RECT GetRect();
	void SetBrick(Vector2 pos);
	void Draw(HDC& hdc);
	void OnCollision();
};

#endif