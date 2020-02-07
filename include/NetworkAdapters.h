#ifndef NETWORKADAPTERS_H
#define NETWORKADAPTERS_H

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

#pragma comment(lib, "IPHLPAPI.lib")

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3
#define NI_MAXSERV 32
#define NI_MAXHOST 1025

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

struct NetworkInterface {
    double IfIndex;
    double IfType;
    const char* Name;
    double Length;
    char* DnsSuffix;
    char* Description;
    char* FriendlyName;
    std::string PhysicalAddress = std::string("");
    std::vector<std::string> DnServer;
    std::vector<std::string> Unicast;
    std::vector<std::string> Anycast;
    std::vector<std::string> Multicast;
    std::vector<std::string> Prefix;
    ULONG ZoneIndices[16];
    char* Dhcpv6Server;
    double Flags;
    double Mtu;
    double OperStatus;
    double TransmitLinkSpeed;
    double ReceiveLinkSpeed;
    double Ipv4Enabled;
    double Ipv6Enabled;
    double Ipv6IfIndex;
    double DdnsEnabled;
    double RegisterAdapterSuffix;
    double ReceiveOnly;
    double NoMulticast;
    double Ipv6OtherStatefulConfig;
    double NetbiosOverTcpipEnabled;
    double Ipv6ManagedAddressConfigurationSupported;
    std::string NetworkGuid;
    double ConnectionType;
    double TunnelType;
    std::string Dhcpv6ClientDuid;
    ULONG Ipv4Metric;
    ULONG Ipv6Metric;
};

struct IfEntry {
    double dwOutOctets;
    double dwInOctets;
    double dwInDiscards;
    double dwInErrors;
    double dwOutDiscards;
    double dwOutErrors;
    double dwSpeed;
    double dwLastChange;
    double dwInNUcastPkts;
    double dwInUcastPkts;
    double dwOutNUcastPkts;
    double dwOutUcastPkts;
    double dwOutQLen;
    double dwInUnknownProtos;
};

class NetworkAdapters {
    public:
        NetworkAdapters();
        bool Initialize();
        bool GetInterfaces(std::vector<NetworkInterface> *vInterfaces);

    private:
        PIP_ADAPTER_ADDRESSES pAddresses;
        DWORD dwRetVal;

};

#endif // NETWORKADAPTERS_H
