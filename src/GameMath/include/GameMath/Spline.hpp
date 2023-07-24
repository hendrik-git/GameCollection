/// @file Spline.hpp
/// @brief Defines a class representing a Catmull-Rom spline
/// @details ...
/// @see ...
/// @date 2023.07.23
/// @author Hendrik Poettker

#pragma once
#include <cassert>
#include <iostream>
#include <vector>

namespace Math
{
	struct Point
	{
		float x = 0.;
		float y = 0.;
	};


	class Spline
	{
	  public:
		Point get_spline_point(float t, bool is_looping = false)
		{
			// P1 o-------o P2
			//   /         \
			//  /           \
			// o P0       P3 o

			int p0, p1, p2, p3;
			if(!is_looping)
			{
				p1 = (int)t + 1;
				p2 = p1 + 1;
				p3 = p2 + 1;
				p0 = p1 - 1;

				if(p3 >= points.size())
					return {10.0f, 10.0f};
			}
			else
			{
				p1 = (int)t;
				p2 = (p1 + 1) % points.size();
				p3 = (p2 + 1) % points.size();
				p0 = (p1 - 1 + points.size()) % points.size();
			}

			t = t - (int)t;

			float tt  = t * t;
			float ttt = tt * t;

			float q1 = -ttt + 2.0f * tt - t;
			float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
			float q3 = -3.0f * ttt + 4.0f * tt + t;
			float q4 = ttt - tt;

			float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 +
							   points[p3].x * q4);
			float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 +
							   points[p3].y * q4);

			return {tx, ty};
		}

		Point get_spline_gradient(float t, bool is_looping = false)
		{
			int p0, p1, p2, p3;
			if(!is_looping)
			{
				p1 = (int)t + 1;
				p2 = p1 + 1;
				p3 = p2 + 1;
				p0 = p1 - 1;

				if(p3 >= points.size())
					return {10.0f, 10.0f};
			}
			else
			{
				p1 = (int)t;
				p2 = (p1 + 1) % points.size();
				p3 = (p2 + 1) % points.size();
				p0 = (p1 - 1 + points.size()) % points.size();
			}

			t = t - (int)t;

			float tt  = t * t;
			float ttt = tt * t;

			float q1 = -3.f * tt + 4.f * t - 1;
			float q2 = 9.f * tt - 10.f * t;
			float q3 = -9.f * tt + 8.f * t + 1;
			float q4 = 3 * tt - 2*t;

			float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 +
							   points[p3].x * q4);
			float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 +
							   points[p3].y * q4);

			return {tx, ty};
		}

		std::vector<Point> points;
	};
}  // namespace Math
