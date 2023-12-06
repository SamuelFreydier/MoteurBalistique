#include "pch.h"
#include "BoundingSphere.h"
#include "Rigidbody.h"
#include "RigidbodyCuboid.h"

namespace BoundingSphereTest
{
	TEST(BoundingSphereTest, testBoundingSphereInit) {
		BoundingSphere bounding1(0.0f, Vector3(0,0,0));
		BoundingSphere bounding2(2.0f, Vector3(2.0f, 1.0f, 0.0f));
		EXPECT_EQ(bounding1.getRadius(), 0.0f);
		EXPECT_EQ(bounding2.getRadius(), 2.0f);
		EXPECT_EQ(bounding1.getPosition().getX(), 0.0f);
		EXPECT_EQ(bounding2.getPosition().getY(), 1.0f);
	}
	TEST(BoundingSphereTest, testBoundingSphereCollides) {
		BoundingSphere bounding1(0.0f, Vector3(0, 0, 0));
		BoundingSphere bounding2(2.0f, Vector3(2.0f, 1.0f, 0.0f));
		//ne s'intersectent clairement pas
		EXPECT_FALSE(bounding1.collides(bounding2));
		bounding2.setPosition(Vector3(2.0f, 0.0f, 0.0f));
		// pile à la limite
		EXPECT_TRUE(bounding2.collides(bounding1));

		// autres tests
		bounding2.setPosition(Vector3(0.4f, 0.0f, 0.0f));
		EXPECT_TRUE(bounding2.collides(bounding1));
		EXPECT_TRUE(bounding2.collides(bounding2));
		EXPECT_TRUE(bounding1.collides(bounding1));
	}
}