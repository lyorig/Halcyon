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