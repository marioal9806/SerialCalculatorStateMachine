#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>

#define BYTES_PER_READ 1

int init_serial();
int serialRead(char* chr);
int serialWrite(char* fmt, char* str);