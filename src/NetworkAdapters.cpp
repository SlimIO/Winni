#include "NetworkAdapters.h"

// NetworkAdapters Constructor
NetworkAdapters::NetworkAdapters() 
{
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
}

bool NetworkAdapters::Initialize() 
{
    PIP_ADAPTER_INFO pAdapterInfo;
    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        printf("Error allocating memory needed to call GetAdaptersinfo\n");
        return false;
    }

    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            printf("Error allocating memory needed to call GetAdaptersinfo\n");
            return false;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        return true;
    }
    printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
    return false;
}

std::vector<NetworkInterface> NetworkAdapters::GetNetworkInterfaces()
{
    std::vector<NetworkInterface> ret;
    while (pAdapter) {
        NetworkInterface temp;
        temp.ComboIndex = pAdapter->ComboIndex;
        temp.AdapterName = pAdapter->AdapterName;
        temp.Description = pAdapter->Description;
        temp.IpAddress = pAdapter->IpAddressList.IpAddress.String;
        printf("\tComboIndex: \t%d\n", pAdapter->ComboIndex);
        printf("\tAdapter Name: \t%s\n", pAdapter->AdapterName);
        printf("\tAdapter Desc: \t%s\n", pAdapter->Description);
        printf("\tIP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);

        ret.push_back(temp);
        pAdapter = pAdapter->Next;
    }
    if(pAdapter) {
        free(pAdapter);
    }

    return ret;
}
