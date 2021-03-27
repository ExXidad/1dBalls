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

bool approxEqual(const long double &a, const long double &b, const double &eps = 0.00001)
{
	return std::abs(a - b) < eps;
}

int main()
{
	std::vector<double> inversionTimes{5};
	for (const auto &inversionTime : inversionTimes)
	{
		std::fstream file, file1;
		file.open("../perturbations/" + std::to_string(inversionTime), std::ios::out);

		double inversionDeltaVStep = pow(10, -4);
		double inversionDeltaVMax = pow(10, -1);

		long double ballInitialX;
		for (double inversionDeltaV = 0; inversionDeltaV < inversionDeltaVMax; inversionDeltaV += inversionDeltaVStep)
		{
			std::vector<Ball> balls(30);

			std::mt19937_64 gen;
			gen.seed(time(nullptr));

			for (int i = 0; i < balls.size(); ++i)
			{
				if (i < 15)
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

			ballInitialX = balls[0].getX();

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

			bool inversionFlag = false;

			while (true)
			{
				bool nonCCIncrementFlag = false;

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

				if (!inversionFlag && t + minDt > inversionTime)
				{
					for (auto &ball : balls)
					{
						ball.update(inversionTime - t);
						ball.applyPeriodicBC();
					}
					t += inversionTime - t;
					nonCCIncrementFlag = true;
				} else if (inversionFlag && t + minDt > inversionTime * 2)
				{
					for (auto &ball : balls)
					{
						ball.update(2 * inversionTime - t);
						ball.applyPeriodicBC();
					}
					t += 2 * inversionTime - t;
					nonCCIncrementFlag = true;
				} else
				{
					for (auto &ball : balls)
					{
						ball.update(minDt);
						ball.applyPeriodicBC();
					}
					t += minDt;
					tmpBall->collideWithNext();
				}

				if (!inversionFlag && approxEqual(t, inversionTime))
				{
					balls[0].setV(balls[0].getV() + inversionDeltaV);
					for (auto &ball : balls)
						ball.setV(-ball.getV());

					if (!nonCCIncrementFlag)
						tmpBall->collideWithNext();

					inversionFlag = true;
				}

//				file1.open("../results/" + std::to_string(t), std::ios::out);
//				for (auto &ball : balls)
//				{
//					file1 << ball.getM() << "\t" << ball.getX() + ball.getBcCounter() << "\t" << ball.getV()
//						  << std::endl;
//				}
//				file1.close();

				if (approxEqual(t, 2 * inversionTime))
					break;
			}
			std::cout << "Progress: " << inversionDeltaV / inversionDeltaVMax * 100 << "%" << std::endl;
			long double x1 = ballInitialX;
			long double x2 = balls[0].getX();
			file << inversionDeltaV << "\t"
				 << std::min({std::abs(x1 - x2), std::abs(1 - (x2 - x1)), std::abs(1 - (x1 - x2))}) << std::endl;
		}
		file.close();
	}

	return 0;
}
