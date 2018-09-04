#include <winsock2.h>
#include "NetworkAdapters.h"
#include "node_api.h"
#include "napi.h"
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace Napi;
using namespace std;

/*
 * Complete list of Interfaces!
 * 
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iptypes/ns-iptypes-_ip_adapter_addresses_lh
 */
Value getAdaptersAddresses(const CallbackInfo& info) {
    Env env = info.Env();

    // Create JavaScript Array
    Array ret = Array::New(env);

    // Create new instanceof NetworkAdapters
    NetworkAdapters Adapters;

    // Initialize Buffer to retrieve Adapters
    if (!Adapters.Initialize()) {
        Error::New(env, "Failed to initialize Adapters!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Retrieve interfaces
    vector<NetworkInterface> vInterfaces;
    if (!Adapters.GetInterfaces(&vInterfaces)) {
        Error::New(env, "Failed to retrieve network interfaces!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Declare type(s)
    unsigned int j;

    // Iterate throught interfaces
    for (size_t i = 0; i < vInterfaces.size(); i++) {
        NetworkInterface Interface = vInterfaces[i];
        Object oInterface = Object::New(env);
        ret[i] = oInterface;

        /** Setup Properties */
        oInterface.Set("IfIndex", Interface.IfIndex);
        oInterface.Set("IfType", Interface.IfType);
        oInterface.Set("Name", Interface.Name);
        oInterface.Set("PhysicalAddress", Interface.PhysicalAddress);
        oInterface.Set("DnsSuffix", Interface.DnsSuffix);
        oInterface.Set("Description", Interface.Description);
        oInterface.Set("FriendlyName", Interface.FriendlyName);
        oInterface.Set("Flags", Interface.Flags);
        oInterface.Set("Length", Interface.Length);
        oInterface.Set("Mtu", Interface.Mtu);
        oInterface.Set("OperStatus", Interface.OperStatus);
        oInterface.Set("ReceiveLinkSpeed", Interface.ReceiveLinkSpeed);
        oInterface.Set("TransmitLinkSpeed", Interface.TransmitLinkSpeed);
        oInterface.Set("Ipv4Enabled", Interface.Ipv4Enabled == 1 ? true : false );
        oInterface.Set("Ipv6Enabled", Interface.Ipv6Enabled == 1 ? true : false );
        oInterface.Set("Ipv6IfIndex", Interface.Ipv6IfIndex);
        oInterface.Set("DdnsEnabled", Interface.DdnsEnabled == 1 ? true : false );
        oInterface.Set("RegisterAdapterSuffix", Interface.RegisterAdapterSuffix);
        oInterface.Set("ReceiveOnly", Interface.ReceiveOnly == 1 ? true : false );
        oInterface.Set("NoMulticast", Interface.NoMulticast == 1 ? true : false );
        oInterface.Set("Ipv6OtherStatefulConfig", Interface.Ipv6OtherStatefulConfig == 1 ? true : false );
        oInterface.Set("NetbiosOverTcpipEnabled", Interface.NetbiosOverTcpipEnabled == 1 ? true : false );
        oInterface.Set("Ipv6ManagedAddressConfigurationSupported", Interface.Ipv6ManagedAddressConfigurationSupported == 1 ? true : false );
        Array ZoneIndices = Array::New(env, 16);
        for (j = 0; j < 16; j++) {
            ZoneIndices[j] = Number::New(env, Interface.ZoneIndices[j]);
        }
        oInterface.Set("ZoneIndices", ZoneIndices);
    };

    return ret;
}

/*
 * convert wchar_t* to std::string 
 */
string wCharToString(wchar_t* field) {
    wstring ws(field);
    string ret(ws.begin(), ws.end());

    return ret;
}

/*
 * Convert GUID to std::string
 */
string GuidToString(GUID guid) {
	char guid_cstr[39];
	snprintf(guid_cstr, sizeof(guid_cstr),
	         "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
	         guid.Data1, guid.Data2, guid.Data3,
	         guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
	         guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return string(guid_cstr);
}

/*
 * Byte sequence to std::string
 */
string byteSeqToString(UCHAR bytes[], size_t n) {
    ostringstream stm;
    stm << hex << uppercase;

    for(size_t i = 0; i < n; ++i) {
        stm << setw(2) << setfill('0') << unsigned(bytes[i]);
    }

    return stm.str();
}

/*
 * Translate MIB_IF_ROW2 into Napi::Object
 */
Object translateIfRow(Env env, MIB_IF_ROW2 ifRow) {
    Object ret = Object::New(env);

    size_t PhysicalAddrLen = (size_t) ifRow.PhysicalAddressLength;
    ret.Set("physicalAddress", PhysicalAddrLen != 0 ? byteSeqToString(ifRow.PhysicalAddress, PhysicalAddrLen) : "");
    ret.Set("interfaceLuid", ifRow.InterfaceLuid.Value);
    ret.Set("interfaceIndex", (ULONG) ifRow.InterfaceIndex);
    ret.Set("interfaceGuid", GuidToString(ifRow.InterfaceGuid));
    ret.Set("alias", wCharToString((wchar_t*) ifRow.Alias));
    ret.Set("description", wCharToString((wchar_t*) ifRow.Description));
    ret.Set("mtu", ifRow.Mtu);
    ret.Set("type", ifRow.Type);
    ret.Set("tunnelType", (double) ifRow.TunnelType);
    ret.Set("mediaType", (double) ifRow.MediaType);
    ret.Set("accessType", (double) ifRow.AccessType);
    ret.Set("physicalMediumType", (double) ifRow.PhysicalMediumType);
    ret.Set("directionType", (double) ifRow.DirectionType);
    ret.Set("operStatus", (double) ifRow.OperStatus);
    ret.Set("adminStatus", (double) ifRow.AdminStatus);
    ret.Set("mediaConnectState", (double) ifRow.MediaConnectState);
    ret.Set("networkGuid", GuidToString(ifRow.NetworkGuid));
    ret.Set("connectionType", (double) ifRow.ConnectionType);
    ret.Set("transmitLinkSpeed", ifRow.TransmitLinkSpeed);
    ret.Set("receiveLinkSpeed", ifRow.ReceiveLinkSpeed);
    ret.Set("inOctets", ifRow.InOctets);
    ret.Set("inUcastPkts", ifRow.InUcastPkts);
    ret.Set("inNUcastPkts", ifRow.InNUcastPkts);
    ret.Set("inDiscards", ifRow.InDiscards);
    ret.Set("inErrors", ifRow.InErrors);
    ret.Set("inUnknownProtos", ifRow.InUnknownProtos);
    ret.Set("inUcastOctets", ifRow.InUcastOctets);
    ret.Set("inMulticastOctets", ifRow.InMulticastOctets);
    ret.Set("inBroadcastOctets", ifRow.InBroadcastOctets);
    ret.Set("outOctets", ifRow.OutOctets);
    ret.Set("outUcastPkts", ifRow.OutUcastPkts);
    ret.Set("outNUcastPkts", ifRow.OutNUcastPkts);
    ret.Set("outDiscards", ifRow.OutDiscards);
    ret.Set("outErrors", ifRow.OutErrors);
    ret.Set("outUcastOctets", ifRow.OutUcastOctets);
    ret.Set("outMulticastOctets", ifRow.OutMulticastOctets);
    ret.Set("outBroadcastOctets", ifRow.OutBroadcastOctets);
    ret.Set("outQLen", ifRow.OutQLen);

    return ret;
}

/*
 * getIfEntry retrieves information for the specified interface on the local computer.
 * 
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getifentry
 * @doc: https://docs.microsoft.com/en-us/previous-versions/windows/desktop/api/ifmib/ns-ifmib-_mib_ifrow
 */
Value getIfEntry(const CallbackInfo& info) {
    Env env = info.Env();
    DWORD retVal = 0;
    NET_IFINDEX ifIndex;
    MIB_IF_ROW2 ifRow;
    SecureZeroMemory((PVOID) &ifRow, sizeof(MIB_IF_ROW2));


    // Check if there is less than one argument, if then throw a JavaScript exception
    if (info.Length() < 1) {
        Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // The first argument (ifIndex) should be typeof Napi::Number
    if (!info[0].IsNumber()) {
        Error::New(env, "argument ifIndex should be typeof number!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Retrieve ifIndex argument (casted as a double integer)
    ifIndex = (NET_IFINDEX) info[0].As<Number>().Int64Value();
    ifRow.InterfaceIndex = ifIndex;
    retVal = GetIfEntry2(&ifRow);
    if (retVal != NO_ERROR) {
        // wprintf(L"GetIfEntry returned error: %lu\n", retVal);
        Error::New(env, "Failed to retrieve IfEntry!").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    return translateIfRow(env, ifRow);
}

/*
 * Retrieves the MIB-II interface table.
 * 
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/netioapi/nf-netioapi-getiftable2
 */
Value getIfTable(const CallbackInfo& info) {
    Env env = info.Env();

    // Instanciate variables
    PMIB_IF_TABLE2 ifTable;
    DWORD dwVal;

    // Get all ifEntry
    dwVal = GetIfTable2(&ifTable);
    if (dwVal != NO_ERROR) {
        Error::New(env, "Failed to execute getIfTable!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Retrieve all ifEntry of the table
    Array ret = Array::New(env);
    int numEntries = (int) ifTable->NumEntries;
    if (numEntries > 0) {
        for (int i = 0; i < numEntries; ++i) {
            MIB_IF_ROW2 ifRow = ifTable->Table[i];
            ret[i] = translateIfRow(env, ifRow);
        }
    }
    
    return ret;
}


/*
 * Retrieves the number of interfaces on the local computer.
 * 
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getnumberofinterfaces
 */
Value getNumberOfInterfaces(const CallbackInfo& info) {
    Env env = info.Env();

    DWORD numInterfaces = 0;
    DWORD error = GetNumberOfInterfaces(&numInterfaces);
    if (error != NO_ERROR) {
        Error::New(env, "Failed to retrieve the number of interfaces on the local computer!").ThrowAsJavaScriptException();
        return env.Null();
    }

    return Number::New(env, numInterfaces);
}

/*
 * Initialize Node.JS addon binding exports
 */
Object Init(Env env, Object exports) {

    // Setup methods
    // TODO: Launch with AsyncWorker to avoid event loop starvation
    exports.Set("getAdaptersAddresses", Function::New(env, getAdaptersAddresses));
    exports.Set("getIfEntry", Function::New(env, getIfEntry));
    exports.Set("getIfTable", Function::New(env, getIfTable));
    exports.Set("getNumberOfInterfaces", Function::New(env, getNumberOfInterfaces));

    return exports;
}

// Declare NATIVE API/ADDON
NODE_API_MODULE(winni, Init)
