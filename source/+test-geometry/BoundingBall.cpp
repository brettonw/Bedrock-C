#include "Test.h"
#include "BoundingBall.h"

TEST_CASE(BoundingBall) {
    //Log::Scope scope (Log::TRACE);
    Vector2 points[] = {Vector2 (0, 0), Vector2 (1, 0), Vector2 (0, 1)};
    auto ball = BoundingBall2::fromPoints (points, 3);

    TEST_EQUALS(ball.getRadius(), sqrt (2) / 2);
    TEST_FALSE(ball.isEmpty());
    TEST_TRUE(ball.contains(Vector2(0, 0)));
    TEST_TRUE(ball.contains(Vector2(1, 0)));
    TEST_TRUE(ball.contains(Vector2(0, 1)));
    TEST_TRUE(ball.contains(Vector2(0.25, 0.25)));
    TEST_TRUE(ball.contains(Vector2(0.75, 0.75)));
    TEST_FALSE(ball.contains(Vector2(0, 2)));
    TEST_FALSE(ball.contains(Vector2(2, 0)));
    TEST_FALSE(ball.contains(Vector2(0, -2)));
    TEST_FALSE(ball.contains(Vector2(-2, 0)));
    TEST_EQUALS(ball.getCenter(), Vector2(0.5, 0.5));
}

TEST_CASE(FromBoundaryPoints) {
    Log::Scope scope (Log::TRACE);
    BoundingBall2::PointList points = { Vector2 (1, 0), Vector2 (-1, 0) };
    Log::debug () << "Points size: " << points.size () << endl;
    auto ball = BoundingBall2::makeBall (points);
    Log::debug () << "center: " << ball.getCenter () << endl;
    TEST_FALSE(ball.isEmpty());
    TEST_EQUALS(ball.getRadius(), 1);
}
