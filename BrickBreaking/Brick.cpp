#include "Brick.h"

Brick::Brick() {};
Brick::Brick(int count) : count(count) { this->count = count; };

int Brick::GetCount() { return count; };
Vector2 Brick::GetPos() { return pos; };
RECT Brick::GetRect() { return rect; };

void Brick::SetBrick(Vector2 pos)
{
	this->pos = pos;
	rect.left = pos.x - 40;
	rect.top = pos.y - 20;
	rect.right = pos.x + 40;
	rect.bottom = pos.y + 20;
}

void Brick::Draw(HDC& hdc)
{
	if (count <= 0)
		return;

	COLORREF color;

	switch (count)
	{
	case 1: color = RGB(255, 255, 0); break; // 노랑
	case 2: color = RGB(128, 128, 0); break; // 올리브
	case 3: color = RGB(0, 255, 0); break; // 초록
	case 4: color = RGB(0, 128, 128); break; // 청록
	case 5: color = RGB(128, 128, 128); break;
	default: color = RGB(128, 128, 128); break;
	}
	if (isCollision)
	{
		color = RGB(0, 0, 0);
		isCollision = false;
	}

	HBRUSH brush = CreateSolidBrush(color);
	SelectObject(hdc, brush);

	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	DeleteObject(brush);
}

void Brick::OnCollision()
{
	isCollision = true;
	--count;

	if (count == 0)
	{
		TrySpawnItem(pos);
	}
}