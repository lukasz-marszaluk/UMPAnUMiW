#ifndef _COMMON_
#define _COMMON_

struct point
{
    int x;
    int y;
};

struct line
{
    double slope;
    int vertical_shift;
    int x;
    int y;
};

// round angle to multiple of 45 degrees
int round_angle_45 (double angle);

#endif // _COMMON_
