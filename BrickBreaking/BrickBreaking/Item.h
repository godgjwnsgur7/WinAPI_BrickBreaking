#pragma once
#ifndef ITEM_H_
#define ITEM_H_

#include "Vector2.h"

enum ItemType
{
	ESplitItem = 0,
	ELengthenItem = 1,
	EClingItem = 2,
	EMaxItem
};

class Item
{
private:
	Vector2 pos;
	RECT rect;
	float moveSpeed;
	ItemType type;

	void Init(Vector2 pos, float moveSpeed);
public:
	bool isUsing = false;

	Item();
	Item(Vector2 pos, float moveSpeed);
	RECT GetRect();
	ItemType GetType();
	void SpawnItem(Vector2 pos);
	void Update();
	void Draw(HDC& hdc);
	void OnCollision();
};

#endif