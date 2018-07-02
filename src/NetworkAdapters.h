#ifndef NETWORKADAPTERS_H
#define NETWORKADAPTERS_H

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#pragma comment(lib, "IPHLPAPI.lib")

struct NetworkInterface {
    DWORD ComboIndex;
    char* AdapterName;
    char* Description;
    String IpAddress[16];
};

class NetworkAdapters {
    public:
        NetworkAdapters();
        PIP_ADAPTER_INFO pAdapter;
        DWORD dwRetVal;
        bool Initialize();
        std::vector<NetworkInterface> GetNetworkInterfaces();
};

#endif // NETWORKADAPTERS_H
