/*
 * homework6.h
 * ECE 266, spring 2026, for homework 6
 * Created by Zhao Zhang
 */

#ifndef HOMEWORK6_H_
#define HOMEWORK6_H_

#include <stdint.h>

// Copy functions: Copy from array X[] to array Y[], each of four elements
void Copy4ByPreIndex(int Y[], int X[]);
void Copy4ByPreIndexUpdate(int Y[], int X[]);
void Copy4ByPostIndex(int Y[], int X[]);
void Copy4ByRegOffset(int Y[], int X[]);

// swap() function: Swap X[i] and Y[i]
void Swap2(short X[], short Y[], int i);

#endif /* HOMEWORK6_H_ */

