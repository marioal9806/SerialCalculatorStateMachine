#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>

#include "serial.h"

// Serial variables
HANDLE serial;
unsigned char TxDBuffer[10];
unsigned char RxDBuffer[10];
DWORD aux = 0;

DCB config;
COMMTIMEOUTS touts;

int init_serial() {
	//TCHAR* pcCommPort = (TCHAR *)TEXT("COM1");
	printf("Iniciando comunicacion ...\n");
	serial = CreateFile("COM1", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (serial == INVALID_HANDLE_VALUE) {
		printf("Error: COM Port Inaccessible\n");
		CloseHandle(serial);
		return 1;
	}
	printf("... vamos bien! ... \n");

	//Configurar protocolo y velocidad
	GetCommState(serial, &config);
	config.BaudRate = CBR_9600;
	config.fParity = 0;
	config.fBinary = 1;
	config.StopBits = ONESTOPBIT;
	config.ByteSize = 8;
	SetCommState(serial, &config);

	//Configurar "timeouts"
	touts.ReadTotalTimeoutConstant = 0;
	touts.ReadIntervalTimeout = 0;
	touts.ReadTotalTimeoutMultiplier = 0;
	SetCommTimeouts(serial, &touts);
	return 0;
}

int serialRead(char* chr) {
	int ret = ReadFile(serial, chr, BYTES_PER_READ, &aux, NULL);
	return (ret ? 0 : 1);
}

int serialWrite(char* fmt, char* str) {
	return 0;
}