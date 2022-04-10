#pragma once

#define BYTES_PER_READ 1
#define MAX_BYTES_PER_WRITE 256

int init_serial();
int serialRead(char* chr);
int serialWrite();