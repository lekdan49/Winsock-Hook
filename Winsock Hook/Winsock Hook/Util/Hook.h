#pragma once

#define MINPACKETLEN 2

extern SOCKET mySock;
extern bool bLogRecv;
extern bool bLogSend;

// HOOKS

// winsock recv
extern int (WINAPI* precv)(SOCKET s, char* buffer, int length, int flags);
int WINAPI MyRecv(SOCKET s, char* buffer, int length, int flags);

// winsock send
extern int (WINAPI* psend)(SOCKET s, const char* buffer, int length, int flags);
int WINAPI MySend(SOCKET s, const char* buffer, int length, int flags);