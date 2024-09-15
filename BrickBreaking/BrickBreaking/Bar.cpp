#include "Bar.h"

Bar::Bar()
{
	pos = { g_screenX / 2, g_screenY - 100 };
	rect.left = pos.x - (100 / 2);
	rect.top = pos.y - 10;
	rect.right = pos.x + (100 / 2);
	rect.bottom = pos.y + 10;
	length = 100;
};

void Bar::SetPosX(float x)
{

	float halfLength = length / 2;
	if (pos.x <= 0)
	{
		pos.x = halfLength;
		return;
	}

	if (pos.x >= g_screenX)
	{
		pos.x = g_screenX - halfLength;
		return;
	}

	pos.x = x;
}

void Bar::SetLength(float addLength)
{
	length += addLength;
}

void Bar::ConnectBall(int ballNum)
{
	this->ballNum = ballNum;
	isClingBall = false;
}

Vector2 Bar::GetPos() { return pos; }
RECT Bar::GetRect() { return rect; }
float Bar::GetLength() { return length; }
bool Bar::IsClingBall() { return ballNum != -1; }

void Bar::Update()
{

	float halfLength = length / 2;

	rect.left = pos.x - halfLength;
	rect.top = pos.y - 10;
	rect.right = pos.x + halfLength;
	rect.bottom = pos.y + 10;
};

void Bar::Draw(HDC& hdc)
{
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));

	if (isClingBall)
		brush = CreateSolidBrush(RGB(255, 255, 255));

	SelectObject(hdc, brush);

	Rectangle(hdc, pos.x - (length / 2), pos.y - 10, pos.x + (length / 2), pos.y + 10);

	DeleteObject(brush);
};

void Bar::ShotBall()
{
	ballNum = -1;
}

bool Bar::IsCheckCollisionItem(Item& item)
{
	bool y = (item.GetRect().bottom >= rect.top && item.GetRect().top <= rect.bottom);
	bool x = (item.GetRect().left >= rect.left && item.GetRect().right <= rect.right);

	if (y && x)
	{
		OnUseItem(item.GetType());
		item.isUsing = false;
	}

	return false;
}

void Bar::OnUseItem(ItemType type)
{
	switch (type)
	{
	case ESplitItem:
		OnSplitBallEvnet();
		break;
	case ELengthenItem:
		OnLengthenBarEvent();
		break;
	case EClingItem:
		OnClingBallEvent();
		break;
	}
}