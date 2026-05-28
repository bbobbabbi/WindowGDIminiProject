#pragma once
#include "pch.h"
#include "Utillity.h"

class MyRender;

namespace learning
{
	struct Collider {
		Vector2f center;
		virtual Collider& GetCollider() = 0;
		virtual bool IsIntersect(Collider* colTarget) = 0;
		virtual void Draw(MyRender& render) = 0;
		bool isPlayerIntersect = false;
		//int배열로 변환해서 할까?
		// 충돌중인 애들만 처리?
		bool isEnemyIntersect = false;
	};

	struct ColliderCircle : Collider
	{
		double radius;
		Collider& GetCollider() override;
		bool IsIntersect(Collider* colTarget) override;
		void Draw(MyRender& render)override;
	};

	struct ColliderBox : Collider
	{
		Vector2f halfSize;
		Collider& GetCollider() override;
		bool IsIntersect(Collider* colTarget) override;
		void Draw(MyRender& render)override;
		bool IsAbove(Collider* platform);
	};

	// Circle
	bool Intersect(ColliderCircle const& lhs, ColliderCircle const& rhs);
	// AABB
	bool Intersect(ColliderBox const& lhs, ColliderBox const& rhs);

	bool Intersect(ColliderCircle const& circle, ColliderBox const& box);
	bool Intersect(ColliderBox const& box, ColliderCircle const& circle);
}

