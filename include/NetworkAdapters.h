#ifndef NETWORKADAPTERS_H
#define NETWORKADAPTERS_H

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#pragma comment(lib, "IPHLPAPI.lib")

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

struct NetworkInterface {
    IF_INDEX Index;
    PCHAR Name;
    ULONG Length;
};

struct IfEntry {
    DWORD dwOutOctets;
    DWORD dwInOctets;
};

class NetworkAdapters {
    public:
        NetworkAdapters();
        std::vector<NetworkInterface> GetInterfaces();
        IfEntry GetIf(IF_INDEX Index);

    private:
        PIP_ADAPTER_ADDRESSES pAddresses;
        DWORD dwRetVal;
        bool Initialize();
};

#endif // NETWORKADAPTERS_H
