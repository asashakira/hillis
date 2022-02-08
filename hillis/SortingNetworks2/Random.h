#pragma once

// rng [from, to)
template<typename T>
T random(T from, T to);

// Marsaglia's xorshf96
// static unsigned long rx=123456789, ry=362436069, rz=521288629;
static unsigned long rx=random(0, 123456789), ry=random(0, 362436069), rz=random(0, 521288629);
unsigned long fastrng(void);
