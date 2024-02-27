#pragma once

#include <cstdio>

struct visit1
{
    int func() const { return 1; }

    int x;
};

struct visit2
{
    int func() const { return 2; }

    int x, y;
};

struct visit3
{
    int func() const { return 3; }

    int x, y, z;
};

struct visit4
{
    int func() const { return 4; }

    int x;
};

struct visit5
{
    int func() const { return 5; }

    int x, y;
};

struct visit6
{
    int func() const { return 6; }

    int x, y, z;
};

struct visit7
{
    int func() const { return 7; }

    int x;
};

struct visit8
{
    int func() const { return 8; }

    int x, y;
};

struct visit9
{
    int func() const { return 9; }

    int x, y, z;
};

struct virtual_base
{
    virtual int func() const = 0;

    virtual ~virtual_base() = default;
};

struct virtual1 : virtual_base
{
    int func() const { return 1; }
    int x;
};

struct virtual2 : virtual_base
{
    int func() const { return 2; }
    int x, y;
};

struct virtual3 : virtual_base
{
    int func() const { return 3; }
    int x, y, z;
};

struct virtual4 : virtual_base
{
    int func() const { return 4; }
    int x;
};

struct virtual5 : virtual_base
{
    int func() const { return 5; }
    int x, y;
};

struct virtual6 : virtual_base
{
    int func() const { return 6; }
    int x, y, z;
};

struct virtual7 : virtual_base
{
    int func() const { return 7; }
    int x;
};

struct virtual8 : virtual_base
{
    int func() const { return 8; }
    int x, y;
};

struct virtual9 : virtual_base
{
    int func() const { return 9; }
    int x, y, z;
};