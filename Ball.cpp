//
// Created by xidad on 27.02.2021.
//

#include "Ball.h"

void Ball::show()
{
	std::cout << "m: " << m << " x: " << x << " v: " << v << std::endl;
}

long double Ball::getM() const
{
	return m;
}

void Ball::setM(long double m)
{
	Ball::m = m;
}

long double Ball::getX() const
{
	return x;
}

void Ball::setX(long double x)
{
	Ball::x = x;
}

long double Ball::getV() const
{
	return v;
}

void Ball::setV(long double v)
{
	Ball::v = v;
}

void Ball::update(const long double &dt)
{
	x += v * dt;
}

void Ball::minImgConv()
{
	if (x > 1)
	{
		x -= 1;
	} else if (x < 0)
	{
		x += 1;
	}
}

long double Ball::getDistToNext() const
{
	return distToNext;
}

void Ball::setDistToNext(long double distToNext)
{
	Ball::distToNext = distToNext;
}

Ball *Ball::getNextBall() const
{
	return nextBall;
}

void Ball::setNextBall(Ball *nextBall)
{
	Ball::nextBall = nextBall;
}

Ball *Ball::getPrevBall() const
{
	return prevBall;
}

void Ball::setPrevBall(Ball *prevBall)
{
	Ball::prevBall = prevBall;
}

void Ball::updDistancesToNeighbours()
{
	if (sgn(cos(2 * M_PI * x)) != sgn(cos(2 * M_PI * nextBall->getX())))
	{
		distToNext = 1 - (nextBall->getX() - x);
	} else
	{
		distToNext = nextBall->getX() - x;
	}
	distToPrev = 1 - (x - prevBall->getX());
}

void Ball::updCollisionDt()
{
	nextBallCollisionDt = distToNext / (v - nextBall->getV());
	prevBallCollisionDt = distToPrev / (prevBall->getV() - v);
}

long double Ball::getDistToPrev() const
{
	return distToPrev;
}

void Ball::setDistToPrev(long double distToPrev)
{
	Ball::distToPrev = distToPrev;
}

long double Ball::getNextBallCollisionDt() const
{
	return nextBallCollisionDt;
}

void Ball::setNextBallCollisionDt(long double nextBallCollisionDt)
{
	Ball::nextBallCollisionDt = nextBallCollisionDt;
}

long double Ball::getPrevBallCollisionDt() const
{
	return prevBallCollisionDt;
}

void Ball::setPrevBallCollisionDt(long double prevBallCollisionDt)
{
	Ball::prevBallCollisionDt = prevBallCollisionDt;
}

void Ball::collideWithNext()
{
	long double m1 = m, m2 = nextBall->getM();
	long double v1 = v, v2 = nextBall->getV();
	v = (v1 * (m1 - m2) + 2 * m2 * v2) / (m1 + m2);
	nextBall->setV((v2 * (m2 - m1) - 2 * m1 * v1) / (m1 + m2));
}
