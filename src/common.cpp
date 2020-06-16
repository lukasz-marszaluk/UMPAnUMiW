#include "../inc/common.hpp"

int round_angle_45 (double angle)
{
  int result = 0;

  while (angle < 0.0)
    angle += 180.0;

  while (angle >= 180.0)
    angle -= 180.0;

  while (angle - result > 22.5)
    result += 45;

  if (result == 180)
    result = 0;

  return result;
}
