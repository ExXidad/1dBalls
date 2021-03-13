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

int main()
{
	std::vector<double> inversionTimes{30, 60, 90};
	for (const auto &inversionTime : inversionTimes)
	{
		std::fstream file;
		file.open("../perturbations/" + std::to_string(inversionTime), std::ios::out);

		double inversionDeltaVStep = 0.01;
		double inversionDeltaVMax = 1;
		for (double inversionDeltaV = 0; inversionDeltaV < inversionDeltaVMax; inversionDeltaV += inversionDeltaVStep)
		{
			std::vector<Ball> balls(10);

			std::mt19937_64 gen;
			gen.seed(time(nullptr));

			for (int i = 0; i < balls.size(); ++i)
			{
				if (i < 5)
				{
					balls[i].setM(1);
					balls[i].setX(1. / balls.size() * i);
				} else
				{
					balls[i].setM(1. / 4);
					balls[i].setX(1. / balls.size() * i);
				}
			}

			std::sort(balls.begin(), balls.end(),
					  [](Ball &ball1, Ball &ball2) { return ball1.getX() < ball2.getX(); });

			long double ballInitialX = balls[0].getX();

			for (int i = 1; i < balls.size() - 1; ++i)
			{
				balls[i].setNextBall(balls[i + 1].self());
			}
			balls[0].setNextBall(balls[1].self());
			(balls.end() - 1)->setNextBall(balls[0].self());

			long double v0 = 1;

			balls[0].setV(v0);

			///////////////////////////////////////////////////////////////////////////////////

			long double t = 0;

			bool flag = false;

			while (t < inversionTime * 2)
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
					ball.update(minDt);
					ball.applyPeriodicBC();
				}

				tmpBall->collideWithNext();

				t += minDt;

				if (!flag && t > inversionTime)
				{
					for (auto &ball : balls)
					{
						ball.setV(-ball.getV());
					}
					tmpBall->collideWithNext();
					flag = true;
				}
			}
			std::cout << "Progress: " << inversionDeltaV / inversionDeltaVMax * 100 << "%" << std::endl;
			file << inversionDeltaV << "\t" << balls[0].getX() - ballInitialX <<std::endl;
		}
		file.close();
	}

	return 0;
}
