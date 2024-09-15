#include "Ball.h"

void Ball::Init(Vector2 initPos, Vector2 velocity, float radius = 10, float moveSpeed = 10)
{
	pos = initPos;
	this->velocity = velocity;
	this->radius = radius;
	this->moveSpeed = moveSpeed;
}

Ball::Ball()
{
	Init(Vector2(g_screenX / 2, g_screenY - 120), Vector2(-2, -2), 10, 10);
};

Ball::Ball(Vector2 initPos, Vector2 velocity, float radius = 10, float moveSpeed = 10)
{
	Init(initPos, velocity, radius, moveSpeed);
}

void Ball::SpawnBall()
{
	isUsing = false;
}

void Ball::SpawnBall(Vector2 initPos, Vector2 velocity = { -2, -2 }, float radius = 10, float moveSpeed = 10)
{
	Init(initPos, velocity, radius, moveSpeed);
	isUsing = true;
}

void Ball::SpawnBall(const Ball& ball)
{
	std::random_device rd;
	std::default_random_engine g(rd());
	std::uniform_int_distribution<int> random(1, 360);
	int radian = random(g);
	Vector2 tempVel{ ball.velocity.x * (float)std::cos(radian), ball.velocity.y * (float)std::sin(radian) };
	Init(ball.pos, tempVel.Normalize());
	isUsing = true;
}

void Ball::SetPosX(float x)
{
	pos.x = x;
}

void Ball::Update()
{
	if (isClingBar)
		return;

	pos = pos + velocity.Normalize() * moveSpeed;

	if (pos.x - radius <= 0)
	{
		pos.x = radius + 1.0f;
		velocity.x = abs(velocity.x);
	}

	if (pos.x + radius >= g_screenX)
	{
		pos.x = g_screenX - radius - 1.0f;
		velocity.x = -abs(velocity.x);
	}

	if (pos.y - radius <= 0)
	{
		pos.y = radius + 1.0f;
		velocity.y = abs(velocity.y);
	}

	if (pos.y + radius >= g_screenY)
	{
		isUsing = false;
	}
};

void Ball::Draw(HDC& hdc)
{
	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, brush);

	Ellipse(hdc, pos.x - radius, pos.y - radius, pos.x + radius, pos.y + radius);

	DeleteObject(brush);
};

bool Ball::CheckCollisionBar(Bar& bar)
{
	int closestX = std::max((float)(bar.GetRect().left), std::min(pos.x, (float)(bar.GetRect().right)));
	int closestY = std::max((float)(bar.GetRect().top), std::min(pos.y, (float)(bar.GetRect().bottom)));

	int distanceX = pos.x - closestX;
	int distanceY = pos.y - closestY;

	if ((distanceX * distanceX + distanceY * distanceY) <= (radius * radius))
	{
		float diffX = pos.x - bar.GetPos().x;
		float barHalfLength = bar.GetLength() / 2;

		float velocityX = diffX / barHalfLength;
		velocity = { velocityX * 2.0f , -1 };

		if (pos.y >= bar.GetPos().y - 10 - radius)
			pos.y = bar.GetPos().y - 10 - radius;

		return true;
	}

	return false;
}

void Ball::CheckCollisionBrick(Brick& brick)
{
	int closestX = std::max((float)(brick.GetRect().left), std::min(pos.x, (float)(brick.GetRect().right)));
	int closestY = std::max((float)(brick.GetRect().top), std::min(pos.y, (float)(brick.GetRect().bottom)));

	int distanceX = pos.x - closestX;
	int distanceY = pos.y - closestY;

	if ((distanceX * distanceX + distanceY * distanceY) <= (radius * radius))
	{
		velocity = velocity.Normalize();
		Vector2 colVec = pos - Vector2(closestX, closestY);
		Vector2 normalVec = colVec.Normalize();
		float dotProduct = velocity.Dot(normalVec);
		velocity = velocity - normalVec * (2.0f * dotProduct);
		pos = pos + normalVec * 1.5f * (radius - sqrt((pos.x - closestX) * (pos.x - closestX) + (pos.y - closestY) * (pos.y - closestY)));
		velocity = velocity.Normalize();

		isCollision = true;
		brick.OnCollision();
	}
}
