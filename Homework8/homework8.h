/*
 * homework8.h
 * ECE 266, spring 2026, for homework 8
 * Created by Zhao Zhang
 */

#ifndef HOMEWORK8_H_
#define HOMEWORK8_H_

#include <stdint.h>

// Copy a zero-terminated short array
void copy1(short X[], short Y[], int N);

// Saturate every element of X[] to 12 bits
void Ssat12Array(short X[], int N);

// Calculate the dot product of X[] and Y[]
int DotProduct(short X[], short Y[], int N);

// Count the occurance of ch in str
int CountChar(const char *str, char ch);

#endif /* HOMEWORK8_H_ */
