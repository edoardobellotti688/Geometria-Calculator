#pragma once
#include "geom.h"
int fcomp(const void *a, const void *b); //comparing function for qsort of object arrays
void clear_buffer();    //clears the buffer of all excessive characters (used after every scanf)
void save(const char* filename, object total[], const int num); //saves current objects into data.bin
void load(const char* filename, object total[], int *num);  //loads previous objects from data.bin
int full_reg_check(int counter);    //checks if total[] is full (0= not full, 1=full)