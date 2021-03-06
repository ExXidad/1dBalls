#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <random>
#include <ctime>

#include "Ball.h"

long double collisionDt(Ball &ball1, Ball &ball2)
{
	long double xMax = std::max(ball2.getX(), ball1.getX()), xMin = std::min(ball2.getX(), ball1.getX());
	if (xMax - xMin > 0.5)
	{

	}
	long double t = (ball2.getX() - ball1.getX()) / (ball1.getV() - ball2.getV());

	return t >= 0 ? t : 0;
}

int main()
{
	system("bash add_rm_res");

	std::vector<Ball> balls(3);

	std::mt19937_64 gen;
	gen.seed(time(nullptr));

	std::uniform_real_distribution<> urd(0, 1);

	for (int i = 0; i < balls.size(); ++i)
	{
		if (i < 10)
		{
			balls[i].setM(1);
//			balls[i].setX(urd(gen));
			balls[i].setX(1. / balls.size() * i);
		} else
		{
			balls[i].setM(1. / 4);
			balls[i].setX(urd(gen));
		}
	}

	std::sort(balls.begin(), balls.end(),
			  [](Ball &ball1, Ball &ball2) { return ball1.getX() < ball2.getX(); });

	for (int i = 1; i < balls.size() - 1; ++i)
	{
		balls[i].setNextBall(balls[i + 1].self());
	}
	balls[0].setNextBall(balls[1].self());
	(balls.end() - 1)->setNextBall(balls[0].self());

	long double v0 = 1;

	balls[0].setV(v0);

//	set t here
	long double tMax = 10, t = 0;

	for (auto &ball : balls)
	{
		ball.show();
	}

	std::fstream file;
	file.open("../results/" + std::to_string(t), std::ios::out);

	for (auto &ball : balls)
	{
		file << ball.getM() << "\t" << ball.getX() << "\t" << ball.getV() << std::endl;
	}

	file.close();

	bool flag = false;

	while (t < tMax)
	{
		for (Ball &ball : balls)
		{
			ball.updDistancesToNeighbours();
		}
		for (Ball &ball : balls)
		{
			ball.updCollisionDt();
		}
		long double minDt = -1;
		Ball *tmpBall;
		for (auto &ball : balls)
		{
			if (minDt < 0)
			{
				if (ball.getNextBallCollisionDt() > 0)
				{
					tmpBall = &ball;
					minDt = ball.getNextBallCollisionDt();
				}
			} else if (ball.getNextBallCollisionDt() > 0 && ball.getNextBallCollisionDt() < minDt)
			{
				tmpBall = &ball;
				minDt = ball.getNextBallCollisionDt();
			}
		}

		if (minDt < 0) exit(1);

		for (auto &ball : balls)
		{
//			std::cout << ball.getX() << std::endl;
			ball.update(minDt);
			ball.applyPeriodicBC();
		}

		tmpBall->collideWithNext();

		t += minDt;

		// Debug
//		std::cout << "t: " << t << " dt: " << minDt << std::endl;
//		for (auto &ball : balls)
//		{
//			ball.show();
//		}
//		std::cout << std::endl;

		file.open("../results/" + std::to_string(t), std::ios::out);

		for (auto &ball : balls)
		{
			file << ball.getM() << "\t" << ball.getX() + ball.getBcCounter() << "\t" << ball.getV() << std::endl;
		}

		file.close();

//		if (!flag && t > tMax/2)
//		{
//			for (auto &ball : balls)
//			{
//				ball.setV(-ball.getV());
//			}
//			flag = true;
//		}
	}

	return 0;
}
