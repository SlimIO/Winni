#include "NetworkAdapters.h"

// NetworkAdapters Constructor
NetworkAdapters::NetworkAdapters() 
{
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    DWORD dwRetVal = 0;
}

bool NetworkAdapters::Initialize() 
{
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

char* NetworkAdapters::toChar(PWCHAR field) {
    size_t len = wcslen(field) + 1;
    char* CHARField = new char[len];
    wcstombs(CHARField, field, len);
    return CHARField;
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
    pCurrAddresses = pAddresses;
    while (pCurrAddresses) {
        NetworkInterface Interface;
        Interface.Index     = pCurrAddresses->IfIndex;
        Interface.Name      = std::string(pCurrAddresses->AdapterName);
        Interface.Length    = pCurrAddresses->Length;
        Interface.DnsSuffix = toChar(pCurrAddresses->DnsSuffix);
        Interface.Description = toChar(pCurrAddresses->Description);
        Interface.FriendlyName = toChar(pCurrAddresses->FriendlyName);

        printf("\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n", pCurrAddresses->Length);
        printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);

        printf("\tFlags: %ld\n", pCurrAddresses->Flags);
        printf("\tMtu: %lu\n", pCurrAddresses->Mtu);
        printf("\tIfType: %ld\n", pCurrAddresses->IfType);
        printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
        printf("\n");

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

        // if (pCurrAddresses->PhysicalAddressLength != 0) {
        //     printf("\tPhysical address: ");
        //     for (i = 0; i < (int) pCurrAddresses->PhysicalAddressLength; i++) {
        //         if (i == (pCurrAddresses->PhysicalAddressLength - 1)) {
        //             printf("%.2X\n", (int) pCurrAddresses->PhysicalAddress[i]);
        //         }
        //         else {
        //             printf("%.2X-", (int) pCurrAddresses->PhysicalAddress[i]);
        //         }
        //     }
        // }

        // printf("\tIpv6IfIndex (IPv6 interface): %u\n", pCurrAddresses->Ipv6IfIndex);
        // printf("\tZoneIndices (hex): ");
        // for (i = 0; i < 16; i++) {
        //     printf("%lx ", pCurrAddresses->ZoneIndices[i]);
        // }
        // printf("\n");

        // printf("\tTransmit link speed: %I64u\n", pCurrAddresses->TransmitLinkSpeed);
        // printf("\tReceive link speed: %I64u\n", pCurrAddresses->ReceiveLinkSpeed);

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

        pCurrAddresses = pCurrAddresses->Next;
    }

    if (pAddresses) {
        FREE(pAddresses);
    }

    return ret;
}

IfEntry NetworkAdapters::GetIf(IF_INDEX Index) {
    MIB_IFROW ifrow;
    ifrow.dwIndex = Index;
    if( GetIfEntry( &ifrow ) == NO_ERROR ) {
        IfEntry entry;
        entry.dwInOctets = ifrow.dwInOctets;
        entry.dwOutOctets = ifrow.dwOutOctets;
        return entry;
    }
    else {
        printf("Could not open default interface !\n");
        IfEntry entry;
        entry.dwInOctets = 0;
        entry.dwOutOctets = 0;
        return entry;
    }
}
