#include "NetworkAdapters.h"

// NetworkAdapters Constructor
NetworkAdapters::NetworkAdapters() {
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

    // If an Error is encountered
    if (dwRetVal != NO_ERROR) {
        printf("Call to GetAdaptersAddresses failed with error: %d\n", dwRetVal);
        if (dwRetVal == ERROR_NO_DATA) {
            printf("\tNo addresses were found for the requested parameters\n");
        }
        return false;
    }

    return true;
}

/*
 * Function to Convert GUID to std::string
 */
string guidToString(GUID guid) {
	char guid_cstr[39];
	snprintf(guid_cstr, sizeof(guid_cstr),
	         "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
	         guid.Data1, guid.Data2, guid.Data3,
	         guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
	         guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return string(guid_cstr);
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

/**
 * Cast SOCKET_ADDRESS to String
 */
bool socketAddrToString(SOCKET_ADDRESS *addr, string *strAddr) {
    char currAddr[MAX_PATH] = {0};
    DWORD addrLen = sizeof(currAddr);
    int success = WSAAddressToStringA(
        addr->lpSockaddr,
        (DWORD) addr->iSockaddrLength,
        NULL,
        currAddr,
        &addrLen
    );
    if (success == 0) {
        *strAddr = string(currAddr);
        return true;
    }

    return false;
}

bool NetworkAdapters::GetInterfaces(vector<NetworkInterface> *vInterfaces)  {
    unsigned int i = 0;
    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;
    size_t PhysicalAddressLength = 0;
    WSADATA data;

    int initialized = WSAStartup(MAKEWORD(2, 2), &data);
    if (initialized != 0) {
        return false;
    }

    /** Translate datas */
    pCurrAddresses = pAddresses;
    while (pCurrAddresses) {
        NetworkInterface Interface;
        SecureZeroMemory(&Interface, sizeof(Interface));

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
        Interface.NetworkGuid = guidToString(pCurrAddresses->NetworkGuid);
        Interface.ConnectionType = (double) pCurrAddresses->ConnectionType;
        Interface.TunnelType = (double) pCurrAddresses->TunnelType;
        Interface.Dhcpv6ClientDuid = byteSeqToString(pCurrAddresses->Dhcpv6ClientDuid, pCurrAddresses->Dhcpv6ClientDuidLength);
        Interface.Ipv4Metric = pAddresses->Ipv4Metric;
        Interface.Ipv6Metric = pAddresses->Ipv6Metric;

        PhysicalAddressLength = (size_t) pCurrAddresses->PhysicalAddressLength;
        if (PhysicalAddressLength != 0) {
            Interface.PhysicalAddress = byteSeqToString(pCurrAddresses->PhysicalAddress, PhysicalAddressLength);
        }

        // Retrive Zone Indices (fixed size of 16).
        for (i = 0; i < 16; i++) {
            Interface.ZoneIndices[i] = pCurrAddresses->ZoneIndices[i];
        }

        pDnServer = pCurrAddresses->FirstDnsServerAddress;
        if (pDnServer) {
            for (i = 0; pDnServer != NULL; i++) {
                string currAddr;
                if (socketAddrToString(&pDnServer->Address, &currAddr)) {
                    Interface.DnServer.push_back(currAddr);
                }
                pDnServer = pDnServer->Next;
            }
        } 

        pUnicast = pCurrAddresses->FirstUnicastAddress;
        while (pUnicast != NULL) {
            string currAddr;
            if (socketAddrToString(&pUnicast->Address, &currAddr)) {
                Interface.Unicast.push_back(currAddr);
            }
            pUnicast = pUnicast->Next;
        }

        pAnycast = pCurrAddresses->FirstAnycastAddress;
        while (pAnycast != NULL) {
            string currAddr;
            if (socketAddrToString(&pAnycast->Address, &currAddr)) {
                Interface.Anycast.push_back(currAddr);
            }
            pAnycast = pAnycast->Next;
        }

        pMulticast = pCurrAddresses->FirstMulticastAddress;
        while (pMulticast != NULL) {
            string currAddr;
            if (socketAddrToString(&pMulticast->Address, &currAddr)) {
                Interface.Multicast.push_back(currAddr);
            }
            pMulticast = pMulticast->Next;
        }

        pPrefix = pCurrAddresses->FirstPrefix;
        while (pPrefix != NULL) {
            string currAddr;
            if (socketAddrToString(&pPrefix->Address, &currAddr)) {
                Interface.Prefix.push_back(currAddr);
            }
            pPrefix = pPrefix->Next;
        }

        // Push back interface!
        vInterfaces->push_back(Interface);
        pCurrAddresses = pCurrAddresses->Next;
    }

    // Cleanup allocation
    if (pAddresses) {
        FREE(pAddresses);
    }
    WSACleanup();

    return true;
}
