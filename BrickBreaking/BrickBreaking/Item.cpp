#include "Item.h"

void Item::Init(Vector2 pos, float moveSpeed = 5)
{
	this->pos = pos;
	rect.left = pos.x - 10;
	rect.top = pos.y - 5;
	rect.right = pos.x + 10;
	rect.bottom = pos.y + 5;
	this->moveSpeed = moveSpeed;

	std::random_device rd;
	std::default_random_engine g(rd());
	std::uniform_int_distribution<int> random(0, EMaxItem - 1);
	type = (ItemType)random(g);
}

Item::Item() {};

Item::Item(Vector2 pos, float moveSpeed = 5)
{
	Init(pos, moveSpeed);
};

RECT Item::GetRect() { return rect; }
ItemType Item::GetType() { return type; }

void Item::SpawnItem(Vector2 pos)
{
	std::random_device rd;
	std::default_random_engine g(rd());
	std::uniform_int_distribution<int> random(0, EMaxItem - 1);
	if (random(g) % 2 != 0)
		return;

	Init(pos);
	isUsing = true;
}

void Item::Update()
{
	pos = pos + Vector2{ 0, moveSpeed };

	rect.left = pos.x - 10;
	rect.top = pos.y - 5;
	rect.right = pos.x + 10;
	rect.bottom = pos.y + 5;

	if (pos.y >= g_screenY)
		isUsing = false;
}

void Item::Draw(HDC& hdc)
{
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

	switch (type)
	{
	case ESplitItem: brush = CreateSolidBrush(RGB(255, 0, 0)); break;
	case ELengthenItem: brush = CreateSolidBrush(RGB(0, 255, 0)); break;
	case EClingItem: brush = CreateSolidBrush(RGB(0, 0, 255)); break;
	}

	SelectObject(hdc, brush);

	Rectangle(hdc, pos.x - 10, pos.y - 5, pos.x + 10, pos.y + 5);

	DeleteObject(brush);
}

void Item::OnCollision()
{
	isUsing = false;
}