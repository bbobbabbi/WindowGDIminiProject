#include "pch.h"
#include "MyRender.h"



learning::Collider& learning::ColliderCircle::GetCollider() {
	return *this;
}

bool learning::ColliderCircle::IsIntersect(Collider* colTarget) {
	if (auto circle = dynamic_cast<ColliderCircle*>(colTarget)) {
		return learning::Intersect(*this, *circle);
	}
	else if (auto box = dynamic_cast<ColliderBox*>(colTarget)) {
		return learning::Intersect(*this, *box);
	}
	return false;
}

learning::Collider& learning::ColliderBox::GetCollider() {
	return *this;
}

bool learning::ColliderBox::IsIntersect(Collider* colTarget) {
	if (auto box = dynamic_cast<ColliderBox*>(colTarget)) {
		return Intersect(*this, *box);
	}
	else if (auto circle = dynamic_cast<ColliderCircle*>(colTarget)) {
		return learning::Intersect(*this, *circle);
	}
	return false;
}


void learning::ColliderCircle::Draw(MyRender& render) {
	render.DrawCircleCollider(this);
}

void learning::ColliderBox::Draw(MyRender& render) {
	render.DrawBoxCollider(this);
}


bool learning::ColliderBox::IsAbove(Collider* platform)
{
	auto platformCol = dynamic_cast<ColliderBox*>(platform);
	if (platformCol == nullptr) return false;

	//player นูลา
	int playerBottom = this->center.y + this->halfSize.y;

	int platformTop = platformCol->center.y - platformCol->halfSize.y;

	return playerBottom <= platformTop;
 }


bool learning::Intersect(ColliderCircle const& lhs, ColliderCircle const& rhs)
{
	return (rhs.center - lhs.center).LengthSquared() < pow(lhs.radius + rhs.radius, 2);
}

bool learning::Intersect(ColliderBox const& lhs, ColliderBox const& rhs)
{
	//x-direction
	if (lhs.center.x - lhs.halfSize.x >= rhs.center.x + rhs.halfSize.x) return false;
	if (lhs.center.x + lhs.halfSize.x <= rhs.center.x - rhs.halfSize.x) return false;

	//y-direction
	if (lhs.center.y - lhs.halfSize.y >= rhs.center.y + rhs.halfSize.y) return false;
	if (lhs.center.y + lhs.halfSize.y <= rhs.center.y - rhs.halfSize.y) return false;

	return true;

}

float Clamp(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

bool learning::Intersect(ColliderCircle const& circle, ColliderBox const& box)
{
	float left = box.center.x - box.halfSize.x;
	float right = box.center.x + box.halfSize.x;
	float top = box.center.y - box.halfSize.y;
	float bottom = box.center.y + box.halfSize.y;

	float closestX = Clamp(circle.center.x, left, right);
	float closestY = Clamp(circle.center.y, top, bottom);

	float dx = circle.center.x - closestX;
	float dy = circle.center.y - closestY;

	return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
}

bool learning::Intersect(ColliderBox const& box, ColliderCircle const& circle)
{
	float left = box.center.x - box.halfSize.x;
	float right = box.center.x + box.halfSize.x;
	float top = box.center.y - box.halfSize.y;
	float bottom = box.center.y + box.halfSize.y;

	float closestX = Clamp(circle.center.x, left, right);
	float closestY = Clamp(circle.center.y, top, bottom);

	float dx = circle.center.x - closestX;
	float dy = circle.center.y - closestY;

	return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
}

