#include "NetworkAdapters.h"

// NetworkAdapters Constructor
NetworkAdapters::NetworkAdapters() 
{
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    DWORD dwRetVal = 0;
}

bool NetworkAdapters::Initialize() {
    // Top Properties
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
    ULONG family = AF_UNSPEC;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;

    // Allocate a 15 KB buffer to start with.
    outBufLen = WORKING_BUFFER_SIZE;
    do {
        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
        if (pAddresses == NULL) {
            printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
            return false;
        }

        dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);
        if (dwRetVal != ERROR_BUFFER_OVERFLOW) {
            break;
        }
        FREE(pAddresses);
        pAddresses = NULL;

        Iterations++;
    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    return true;
}

/*
 * PWCharToCChar
 */
char* PWCharToCChar(PWCHAR field) {
    size_t len = wcslen(field) + 1;
    char* CHARField = new char[len];
    wcstombs(CHARField, field, len);
    return CHARField;
}

/*
 * Byte sequence to std::string
 */
string byteSeqToString(const unsigned char bytes[], size_t n) {
    ostringstream stm;
    stm << hex << uppercase;

    for(size_t i = 0; i < n; ++i) {
        stm << setw(2) << setfill('0') << unsigned(bytes[i]);
    }

    return stm.str();
}

std::vector<NetworkInterface> NetworkAdapters::GetInterfaces()  {
    std::vector<NetworkInterface> ret;
    bool isInitialized = Initialize();
    if (isInitialized == false) {
        printf("Failed to initialize interfaces\n");
        return ret;
    }
    DWORD dwSize = 0;
    unsigned int i = 0;
    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;

    // If an Error is encountered
    if (dwRetVal != NO_ERROR) {
        printf("Call to GetAdaptersAddresses failed with error: %d\n", dwRetVal);
        if (dwRetVal == ERROR_NO_DATA) {
            printf("\tNo addresses were found for the requested parameters\n");
        }
        return ret;
    }

    /** Translate datas */
    pCurrAddresses = pAddresses;
    while (pCurrAddresses) {
        NetworkInterface Interface;
        Interface.IfIndex       = (double) pCurrAddresses->IfIndex;
        Interface.IfType        = (double) pCurrAddresses->IfType;
        Interface.Name          = string(pCurrAddresses->AdapterName).c_str();
        Interface.Length        = (double) pCurrAddresses->Length;
        Interface.DnsSuffix     = PWCharToCChar(pCurrAddresses->DnsSuffix);
        Interface.Description   = PWCharToCChar(pCurrAddresses->Description);
        Interface.FriendlyName  = PWCharToCChar(pCurrAddresses->FriendlyName);
        Interface.Flags         = (double) pCurrAddresses->Flags;
        Interface.Mtu           = (double) pCurrAddresses->Mtu;
        Interface.OperStatus    = (double) pCurrAddresses->OperStatus;
        Interface.TransmitLinkSpeed = (double) pCurrAddresses->TransmitLinkSpeed;
        Interface.ReceiveLinkSpeed = (double) pCurrAddresses->ReceiveLinkSpeed;
        Interface.Ipv4Enabled   = (double) pCurrAddresses->Ipv4Enabled;
        Interface.Ipv6Enabled   = (double) pCurrAddresses->Ipv6Enabled;
        Interface.Ipv6IfIndex   = (double) pCurrAddresses->Ipv6IfIndex;
        Interface.DdnsEnabled   = (double) pCurrAddresses->DdnsEnabled;
        Interface.RegisterAdapterSuffix = (double) pCurrAddresses->RegisterAdapterSuffix;
        Interface.ReceiveOnly   = (double) pCurrAddresses->ReceiveOnly;
        Interface.NoMulticast   = (double) pCurrAddresses->NoMulticast;
        Interface.Ipv6OtherStatefulConfig = (double) pCurrAddresses->Ipv6OtherStatefulConfig;
        Interface.NetbiosOverTcpipEnabled = (double) pCurrAddresses->NetbiosOverTcpipEnabled;
        Interface.Ipv6ManagedAddressConfigurationSupported = (double) pCurrAddresses->Ipv6ManagedAddressConfigurationSupported;

        size_t PhysicalAddressLength = (size_t) pCurrAddresses->PhysicalAddressLength;
        if (PhysicalAddressLength != 0) {
            string PhysicalAddrString = byteSeqToString(pCurrAddresses->PhysicalAddress, PhysicalAddressLength);
            const char* PhysicalAddrCStr = PhysicalAddrString.c_str();
            cout << "PhysicalAddr: " << PhysicalAddrCStr << endl;
            Interface.PhysicalAddress = PhysicalAddrCStr;
        }
        else {
            Interface.PhysicalAddress = "";
        }

        // Retrive Zone Indices (fixed size of 16).
        vector<ULONG> ZoneIndices;
        for (i = 0; i < 16; i++) {
            ZoneIndices.push_back(pCurrAddresses->ZoneIndices[i]);
        }
        Interface.ZoneIndices = ZoneIndices;

        sockaddr_in* Dhcpv6ServerAddr = (sockaddr_in*) pCurrAddresses->Dhcpv6Server.lpSockaddr;
        // char Dhcpv6Server[INET_ADDRSTRLEN];
        // inet_ntop(Dhcpv6ServerAddr->sin_family, &(((struct sockaddr_in *)&Dhcpv6ServerAddr)->sin_addr), Dhcpv6Server, INET_ADDRSTRLEN);
        char Dhcpv6Server[NI_MAXHOST];
        getnameinfo((struct sockaddr *)Dhcpv6ServerAddr, pCurrAddresses->Dhcpv6Server.iSockaddrLength, Dhcpv6Server, sizeof(Dhcpv6Server), NULL, 0, NI_NUMERICHOST);

        cout << "dhcp6Server: " << string(Dhcpv6Server) << endl;

        // pDnServer = pCurrAddresses->FirstDnsServerAddress;
        // if (pDnServer) {
        //     for (i = 0; pDnServer != NULL; i++) {
        //         pDnServer = pDnServer->Next;
        //     }
        //     printf("\tNumber of DNS Server Addresses: %d\n", i);
        // } 
        // else {
        //     printf("\tNo DNS Server Addresses\n");
        // }

        // Push back interface!
        ret.push_back(Interface);

        // pUnicast = pCurrAddresses->FirstUnicastAddress;
        // if (pUnicast != NULL) {
        //     for (i = 0; pUnicast != NULL; i++) {
        //         pUnicast = pUnicast->Next;
        //     }
        //     printf("\tNumber of Unicast Addresses: %d\n", i);
        // } 
        // else {
        //     printf("\tNo Unicast Addresses\n");
        // }

        // pAnycast = pCurrAddresses->FirstAnycastAddress;
        // if (pAnycast) {
        //     for (i = 0; pAnycast != NULL; i++) {
        //         pAnycast = pAnycast->Next;
        //     }
        //     printf("\tNumber of Anycast Addresses: %d\n", i);
        // } 
        // else {
        //     printf("\tNo Anycast Addresses\n");
        // }

        // pMulticast = pCurrAddresses->FirstMulticastAddress;
        // if (pMulticast) {
        //     for (i = 0; pMulticast != NULL; i++) {
        //         pMulticast = pMulticast->Next;
        //     }
        //     printf("\tNumber of Multicast Addresses: %d\n", i);
        // } 
        // else {
        //     printf("\tNo Multicast Addresses\n");
        // }

        // pPrefix = pCurrAddresses->FirstPrefix;
        // if (pPrefix) {
        //     for (i = 0; pPrefix != NULL; i++) {
        //         pPrefix = pPrefix->Next;
        //     }
        //     printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
        // } 
        // else {
        //     printf("\tNumber of IP Adapter Prefix entries: 0\n");
        // }
        // printf("\n");

        pCurrAddresses = pCurrAddresses->Next;
    }

    if (pAddresses) {
        FREE(pAddresses);
    }

    return ret;
}
