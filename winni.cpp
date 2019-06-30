#include <winsock2.h>
#include "NetworkAdapters.h"
#include "node_api.h"
#include "napi.h"
#include <sstream>
#include  "slimio.h"

using namespace Napi;
using namespace std;
using namespace Slimio;

/*
 * Complete list of Interfaces!
 *
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getadaptersaddresses
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iptypes/ns-iptypes-_ip_adapter_addresses_lh
 */
class GetAdapterAddrWorker : public AsyncWorker {
    public:
        GetAdapterAddrWorker(Function& callback) : AsyncWorker(callback) {}
        ~GetAdapterAddrWorker() {}
    private:
        vector<NetworkInterface> vInterfaces;

        void Execute(){
            NetworkAdapters Adapters;

            // Initialize Buffer to retrieve Adapters
            if (!Adapters.Initialize()) {
                return SetError("Failed to initialize Adapters!");
            }

            // Retrieve interfaces
            if (!Adapters.GetInterfaces(&vInterfaces)) {
                return SetError("Failed to retrieve network interfaces!");
            }
        }

        void OnError(const Error& e) {
            DWORD errorCode = GetLastError();
            stringstream error;
            error << e.what();
            if (errorCode != 0) {
                error << " - code (" << errorCode << ") - " << getLastErrorMessage();
            }

            Callback().Call({String::New(Env(), error.str()), Env().Null()});
        }

        void OnOK() {
            HandleScope scope(Env());
            Array ret = Array::New(Env());
            size_t j;

            // Iterate throught interfaces
            NetworkInterface Interface;
            for (size_t i = 0; i < vInterfaces.size(); i++) {
                SecureZeroMemory(&Interface, sizeof(Interface));
                Interface = vInterfaces[i];
                Object oInterface = Object::New(Env());
                ret[i] = oInterface;

                /** Setup Properties */
                oInterface.Set("ifIndex", Interface.IfIndex);
                oInterface.Set("ifType", Interface.IfType);
                oInterface.Set("name", Interface.Name);
                oInterface.Set("physicalAddress", Interface.PhysicalAddress);
                oInterface.Set("dnsSuffix", Interface.DnsSuffix);
                oInterface.Set("description", Interface.Description);
                oInterface.Set("friendlyName", Interface.FriendlyName);
                oInterface.Set("flags", Interface.Flags);
                oInterface.Set("length", Interface.Length);
                oInterface.Set("mtu", Interface.Mtu);
                oInterface.Set("operStatus", Interface.OperStatus);
                oInterface.Set("receiveLinkSpeed", Interface.ReceiveLinkSpeed);
                oInterface.Set("transmitLinkSpeed", Interface.TransmitLinkSpeed);
                oInterface.Set("ipv4Enabled", Interface.Ipv4Enabled == 1 ? true : false );
                oInterface.Set("ipv6Enabled", Interface.Ipv6Enabled == 1 ? true : false );
                oInterface.Set("ipv6IfIndex", Interface.Ipv6IfIndex);
                oInterface.Set("dnsEnabled", Interface.DdnsEnabled == 1 ? true : false );
                oInterface.Set("registerAdapterSuffix", Interface.RegisterAdapterSuffix);
                oInterface.Set("receiveOnly", Interface.ReceiveOnly == 1 ? true : false );
                oInterface.Set("noMulticast", Interface.NoMulticast == 1 ? true : false );
                oInterface.Set("ipv6OtherStatefulConfig", Interface.Ipv6OtherStatefulConfig == 1 ? true : false );
                oInterface.Set("netbiosOverTcpipEnabled", Interface.NetbiosOverTcpipEnabled == 1 ? true : false );
                oInterface.Set("ipv6ManagedAddressConfigurationSupported", Interface.Ipv6ManagedAddressConfigurationSupported == 1 ? true : false );
                oInterface.Set("networkGuid", Interface.NetworkGuid);
                oInterface.Set("connectionType", Interface.ConnectionType);
                oInterface.Set("tunnelType", Interface.TunnelType);
                oInterface.Set("dhcpv6ClientDuid", Interface.Dhcpv6ClientDuid);
                oInterface.Set("ipv4Metric", Interface.Ipv4Metric);
                oInterface.Set("ipv6Metric", Interface.Ipv6Metric);
                Array ZoneIndices = Array::New(Env(), 16);
                for (j = 0; j < 16; j++) {
                    ZoneIndices[j] = Number::New(Env(), Interface.ZoneIndices[j]);
                }
                oInterface.Set("zoneIndices", ZoneIndices);

                Array DnServer = Array::New(Env(), Interface.DnServer.size());
                for (j = 0; j < Interface.DnServer.size(); j++) {
                    DnServer[j] = String::New(Env(), Interface.DnServer.at(j));
                }
                oInterface.Set("dnServer", DnServer);

                Array Anycast = Array::New(Env(), Interface.Anycast.size());
                for (j = 0; j < Interface.Anycast.size(); j++) {
                    Anycast[j] = String::New(Env(), Interface.Anycast.at(j));
                }
                oInterface.Set("anycast", Anycast);

                Array Multicast = Array::New(Env(), Interface.Multicast.size());
                for (j = 0; j < Interface.Multicast.size(); j++) {
                    Multicast[j] = String::New(Env(), Interface.Multicast.at(j));
                }
                oInterface.Set("multicast", Multicast);

                Array Unicast = Array::New(Env(), Interface.Unicast.size());
                for (j = 0; j < Interface.Unicast.size(); j++) {
                    Unicast[j] = String::New(Env(), Interface.Unicast.at(j));
                }
                oInterface.Set("unicast", Unicast);
            };

            Callback().Call({Env().Null(), ret});
        }
};

/*
 * Complete list of Interfaces! (JavaScript Binding)
 */
Value getAdaptersAddresses(const CallbackInfo& info) {
    Env env = info.Env();

    // Check argument length!
    if (info.Length() < 1) {
        Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // callback should be a Napi::Function
    if (!info[0].IsFunction()) {
        Error::New(env, "argument callback should be a Function!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Execute worker
    Function cb = info[0].As<Function>();
    (new GetAdapterAddrWorker(cb))->Queue();

    return env.Undefined();
}

/*
 * Translate MIB_IF_ROW2 into Napi::Object
 * This method is used by getIfEntry and getIfTable binding
 */
Object translateIfRow(Env env, MIB_IF_ROW2 ifRow) {
    Object ret = Object::New(env);

    size_t PhysicalAddrLen = (size_t) ifRow.PhysicalAddressLength;
    ret.Set("physicalAddress", PhysicalAddrLen != 0 ? byteSeqToString(ifRow.PhysicalAddress, PhysicalAddrLen) : "");
    ret.Set("interfaceLuid", ifRow.InterfaceLuid.Value);
    ret.Set("interfaceIndex", (ULONG) ifRow.InterfaceIndex);
    ret.Set("interfaceGuid", guidToString(ifRow.InterfaceGuid));
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
    ret.Set("networkGuid", guidToString(ifRow.NetworkGuid));
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
 * The GetIfEntry2 function retrieves information for the specified interface on the local computer.
 *
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/netioapi/nf-netioapi-getifentry2
 */
class GetIfEntryWorker : public AsyncWorker {
    public:
        GetIfEntryWorker(Function& callback, NET_IFINDEX ifIndex) : AsyncWorker(callback), ifIndex(ifIndex) {}
        ~GetIfEntryWorker() {}
    private:
        MIB_IF_ROW2 ifRow;
        NET_IFINDEX ifIndex;

        void Execute(){
            SecureZeroMemory(&ifRow, sizeof(ifRow));
            ifRow.InterfaceIndex = ifIndex;
            DWORD error = GetIfEntry2(&ifRow);
            if (error != NO_ERROR) {
                stringstream errStr;
                errStr << "Failed to retrieve ifEntry for IfIndex id " << (DWORD) ifIndex;
                if (error == ERROR_FILE_NOT_FOUND) {
                    errStr << " - ERROR_FILE_NOT_FOUND";
                }
                else if (error == ERROR_INVALID_PARAMETER) {
                    errStr << " - ERROR_INVALID_PARAMETER";
                }

                return SetError(errStr.str());
            }
        }

        void OnOK() {
            HandleScope scope(Env());
            Callback().Call({Env().Null(), translateIfRow(Env(), ifRow)});
        }
};

/*
 * getIfEntry retrieves information for the specified interface on the local computer.
 *
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getifentry
 * @doc: https://docs.microsoft.com/en-us/previous-versions/windows/desktop/api/ifmib/ns-ifmib-_mib_ifrow
 */
Value getIfEntry(const CallbackInfo& info) {
    Env env = info.Env();

    // Check if there is less than one argument, if then throw a JavaScript exception
    if (info.Length() < 2) {
        Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // The first argument (ifIndex) should be typeof Napi::Number
    if (!info[0].IsNumber()) {
        Error::New(env, "argument ifIndex should be typeof number!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Callback should be a Napi::Function
    if (!info[1].IsFunction()) {
        Error::New(env, "argument callback should be a Function!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Retrieve ifIndex argument (casted as a double integer)
    NET_IFINDEX ifIndex = (NET_IFINDEX) info[0].As<Number>().Int64Value();

    // Execute worker
    Function cb = info[1].As<Function>();
    (new GetIfEntryWorker(cb, ifIndex))->Queue();

    return env.Undefined();
}

/*
 * Retrieves the MIB-II interface table.
 *
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/netioapi/nf-netioapi-getiftable2
 */
class GetIfTableWorker : public AsyncWorker {
    public:
        GetIfTableWorker(Function& callback) : AsyncWorker(callback) {}
        ~GetIfTableWorker() {}
    private:
        PMIB_IF_TABLE2 ifTable;

        void Execute(){
            if (GetIfTable2(&ifTable) != NO_ERROR) {
                return SetError("Failed to retrieve ifTable");
            }
        }

        void OnError(const Error& e) {
            DWORD errorCode = GetLastError();
            stringstream error;
            error << e.what();
            if (errorCode != 0) {
                error << " - code (" << errorCode << ") - " << getLastErrorMessage();
            }

            Callback().Call({String::New(Env(), error.str()), Env().Null()});
        }

        void OnOK() {
            HandleScope scope(Env());
            Array ret = Array::New(Env());
            for (int i = 0; i < (int) ifTable->NumEntries; ++i) {
                ret[i] = translateIfRow(Env(), ifTable->Table[i]);
            }

            Callback().Call({Env().Null(), ret});
        }
};

/*
 * Retrieves the MIB-II interfaces table. (JavaScript Binding)
 */
Value getIfTable(const CallbackInfo& info) {
    Env env = info.Env();

    // Check argument length!
    if (info.Length() < 1) {
        Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // callback should be a Napi::Function
    if (!info[0].IsFunction()) {
        Error::New(env, "argument callback should be a Function!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Execute worker
    Function cb = info[0].As<Function>();
    (new GetIfTableWorker(cb))->Queue();

    return env.Undefined();
}

/*
 * Retrieves the number of interfaces on the local computer.
 *
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getnumberofinterfaces
 */
class GetInterfaceNumberWorker : public AsyncWorker {
    public:
        GetInterfaceNumberWorker(Function& callback) : AsyncWorker(callback) {}
        ~GetInterfaceNumberWorker() {}
    private:
        DWORD numInterfaces;

        void Execute(){
            if (GetNumberOfInterfaces(&numInterfaces) != NO_ERROR) {
                return SetError("Failed to retrieve the number of interfaces on the local computer!");
            }
        }

        void OnOK() {
            HandleScope scope(Env());
            Callback().Call({Env().Null(), Number::New(Env(), numInterfaces)});
        }
};

/*
 * Retrieves the number of interfaces on the local computer (JavaScript Binding).
 */
Value getNumberOfInterfaces(const CallbackInfo& info) {
    Env env = info.Env();

    // Check argument length!
    if (info.Length() < 1) {
        Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // callback should be a Napi::Function
    if (!info[0].IsFunction()) {
        Error::New(env, "argument callback should be a Function!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Execute worker
    Function cb = info[0].As<Function>();
    (new GetInterfaceNumberWorker(cb))->Queue();

    return env.Undefined();
}

/*
 * Initialize Node.JS addon binding exports
 */
Object Init(Env env, Object exports) {

    // Setup addon methods
    exports.Set("getAdaptersAddresses", Function::New(env, getAdaptersAddresses));
    exports.Set("getIfEntry", Function::New(env, getIfEntry));
    exports.Set("getIfTable", Function::New(env, getIfTable));
    exports.Set("getNumberOfInterfaces", Function::New(env, getNumberOfInterfaces));

    return exports;
}

// Declare NATIVE API/ADDON
NODE_API_MODULE(winni, Init)
