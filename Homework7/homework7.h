/*
 * homework7.h
 * ECE 266, spring 2026, for homework 7
 * Created by Zhao Zhang
 */

#ifndef HOMEWORK7_H_
#define HOMEWORK7_H_

#include <stdint.h>

// unsigned max functions, one using conditional branch and
// one using conditional execution
unsigned umax_cb(unsigned a, unsigned b);
unsigned umax_ce(unsigned a, unsigned b);

// sign function, one using conditional branch and
// one using conditional execution
int sign_cb(int n);
int sign_ce(int n);

// if X[k] > X[k+1], swap X[k] and X[k+1] and return 1;
// otherwise, return 0
// one using conditional branch, and one using conditional execution
int swap_cb(unsigned short X[], int k);
int swap_ce(unsigned short X[], int k);

#endif /* HOMEWORK7_H_ */
