#pragma once
#include "windns.h"

// HOOKS

// winsock recv
extern int (WINAPI* precv)(SOCKET s, char* buffer, int length, int flags);
int WINAPI MyRecv(SOCKET s, char* buffer, int length, int flags);

// winsock send
extern int (WINAPI* psend)(SOCKET s, const char* buffer, int length, int flags);
int WINAPI MySend(SOCKET s, const char* buffer, int length, int flags);

// windns DnsQuery
extern DNS_STATUS (WINAPI* pDNSQueryA)(PCSTR pszName, WORD wType, DWORD Options, PVOID pExtra, PDNS_RECORD* ppQueryResults, PVOID* pReserved);
DNS_STATUS __stdcall myDNSQueryA(PCSTR pszName, WORD wType, DWORD Options, PVOID pExtra, PDNS_RECORD* ppQueryResults, PVOID* pReserved);