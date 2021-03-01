//
// Created by xidad on 27.02.2021.
//

#ifndef BALLS_BALL_H
#define BALLS_BALL_H

#include <iostream>
#include <cmath>

class Ball
{
private:
	long double m;
	long double x;
	long double v;
	long double distToNext, distToPrev;
	Ball *nextBall, *prevBall;
	long double nextBallCollisionDt, prevBallCollisionDt;
public:
	long double getDistToPrev() const;

	void setDistToPrev(long double distToPrev);

	long double getNextBallCollisionDt() const;

	void setNextBallCollisionDt(long double nextBallCollisionDt);

	long double getPrevBallCollisionDt() const;

	void setPrevBallCollisionDt(long double prevBallCollisionDt);

	Ball *getNextBall() const;

	void setNextBall(Ball *nextBall);

	Ball *getPrevBall() const;

	void setPrevBall(Ball *prevBall);

	long double getM() const;

	void setM(long double m);

	long double getX() const;

	void setX(long double x);

	long double getV() const;

	void setV(long double v);

	long double getDistToNext() const;

	void setDistToNext(long double distToNext);

	int sgn(const long double &x)
	{ return (x >= 0 ? 1 : -1); }


public:
	Ball(const long double &m = 1, const long double x = 0, const long double v = 0) : m{m}, x{x}, v{v}
	{}

	void show();

	void update(const long double &dt);

	void minImgConv();

	Ball *self()
	{ return this; }

	void updDistancesToNeighbours();

	void updCollisionDt();

	void collideWithNext();
};


#endif //BALLS_BALL_H
