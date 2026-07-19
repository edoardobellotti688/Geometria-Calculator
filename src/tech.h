#pragma once
#include "geom.h"
#ifdef _WIN32
#include <io.h> //for Windows
#define access _access
#define F_OK 0
#else
#include <unistd.h> // for Linux and macOS
#endif

int file_exists(const char* filename); //checks if file named 'filename' exists
void create_file(const char* filename);//creates file named 'filename'
int fcomp(const void *a, const void *b); //comparing function for qsort of object arrays
void clear_buffer();    //clears the buffer of all excessive characters (used after every scanf)
void save(const char* filename, object total[], const int num); //saves current objects into data file
void load(const char* filename, object total[], int *num);  //loads previous objects from data file
int full_reg_check(int counter);    //checks if total[] is full (0= not full, 1=full)