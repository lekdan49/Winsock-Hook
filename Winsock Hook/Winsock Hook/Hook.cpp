#include <windows.h>
#include <stdio.h>

#include "Util\Common.h"
#include "Util\Hook.h"

#include <vector>
#include <iostream>

bool bLogRecv = false;
bool bLogSend = false;

SOCKET mySock = 0;
int (WINAPI* precv)(SOCKET s, char* buffer, int length, int flags) = NULL;
int (WINAPI* psend)(SOCKET s, const char* buffer, int length, int flags) = NULL;
DNS_STATUS (WINAPI* pDNSQueryA)(PCSTR pszName, WORD wType, DWORD Options, PVOID pExtra, PDNS_RECORD* ppQueryResults, PVOID* pReserved) = NULL;


int WINAPI MyRecv(SOCKET s, char* buffer, int length, int flags)
{
	std::cout << "RECEIVING PACKET" << '\n';
	util::Log("%s\n", "RECEIVING PACKET");
	util::Log("LENGTH: %d\n", length);

	sockaddr_in socket;
	int sock_size = sizeof(socket);

	if (getpeername(s, (struct sockaddr*)&socket, &sock_size) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEFAULT)
		{
		}
	}
	else
	{
		util::Log("SOCKET PEER: %s\n", inet_ntoa(socket.sin_addr));
		util::Log("SCOKET PORT: %d\n", socket.sin_port);
	}

	std::vector<uint8_t> msg;
	for (int i = 0; i < length; i++)
	{
		msg.push_back((uint8_t)buffer[i]);
	}

	util::Log_buffer("BUFFER:\n", msg);
	util::Log("\n\n\n\n");

	return precv(s, buffer, length, flags);
}

int WINAPI MySend(SOCKET s, const char* buffer, int length, int flags)
{
	std::cout << "SENDING PACKET" << '\n';
	util::Log("%s\n", "SENDING PACKET");
	util::Log("LENGTH: %d\n", length);

	sockaddr_in socket;
	int sock_size = sizeof(socket);

	if (getpeername(s, (struct sockaddr*)&socket, &sock_size) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEFAULT)
		{
		}
	}
	else
	{
		util::Log("SOCKET PEER: %s\n", inet_ntoa(socket.sin_addr));
		util::Log("SCOKET PORT: %d\n", socket.sin_port);
	}

	std::vector<uint8_t> msg;
	for (int i = 0; i < length; i++)
	{
		msg.push_back((uint8_t)buffer[i]);
	}

	util::Log_buffer("BUFFER:\n", msg);

	util::Log("\n\n\n\n");

	return psend(s, buffer, length, flags);
}

DNS_STATUS __stdcall myDNSQueryA(PCSTR pszName, WORD wType, DWORD Options, PVOID pExtra, PDNS_RECORD* ppQueryResults, PVOID* pReserved)
{
	std::cout << pszName << '\n';
	return pDNSQueryA(pszName, wType, Options, pExtra, ppQueryResults, pReserved);
}
