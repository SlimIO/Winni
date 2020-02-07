#include <winsock2.h>
#include "NetworkAdapters.h"
#include "node_api.h"
#include "napi.h"
#include <sstream>
#include  "slimio.h"

using namespace Slimio;

/*
 * Complete list of Interfaces!
 *
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getadaptersaddresses
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iptypes/ns-iptypes-_ip_adapter_addresses_lh
 */
class GetAdapterAddrWorker : public Napi::AsyncWorker {
    public:
        GetAdapterAddrWorker(Napi::Function& callback) : AsyncWorker(callback) {}
        ~GetAdapterAddrWorker() {}
    private:
        std::vector<NetworkInterface> vInterfaces;

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

        void OnError(const Napi::Error& e) {
            DWORD errorCode = GetLastError();
            std::stringstream error;
            error << e.what();
            if (errorCode != 0) {
                error << " - code (" << errorCode << ") - " << getLastErrorMessage();
            }

            Callback().Call({Napi::String::New(Env(), error.str()), Env().Null()});
        }

        void OnOK() {
            Napi::HandleScope scope(Env());
            Napi::Array ret = Napi::Array::New(Env());
            size_t j;

            // Iterate throught interfaces
            NetworkInterface Interface;
            for (size_t i = 0; i < vInterfaces.size(); i++) {
                SecureZeroMemory(&Interface, sizeof(Interface));
                Interface = vInterfaces[i];
                Napi::Object oInterface = Napi::Object::New(Env());
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
                Napi::Array ZoneIndices = Napi::Array::New(Env(), 16);
                for (j = 0; j < 16; j++) {
                    ZoneIndices[j] = Napi::Number::New(Env(), Interface.ZoneIndices[j]);
                }
                oInterface.Set("zoneIndices", ZoneIndices);

                Napi::Array DnServer = Napi::Array::New(Env(), Interface.DnServer.size());
                for (j = 0; j < Interface.DnServer.size(); j++) {
                    DnServer[j] = Napi::String::New(Env(), Interface.DnServer.at(j));
                }
                oInterface.Set("dnServer", DnServer);

                Napi::Array Anycast = Napi::Array::New(Env(), Interface.Anycast.size());
                for (j = 0; j < Interface.Anycast.size(); j++) {
                    Anycast[j] = Napi::String::New(Env(), Interface.Anycast.at(j));
                }
                oInterface.Set("anycast", Anycast);

                Napi::Array Multicast = Napi::Array::New(Env(), Interface.Multicast.size());
                for (j = 0; j < Interface.Multicast.size(); j++) {
                    Multicast[j] = Napi::String::New(Env(), Interface.Multicast.at(j));
                }
                oInterface.Set("multicast", Multicast);

                Napi::Array Unicast = Napi::Array::New(Env(), Interface.Unicast.size());
                for (j = 0; j < Interface.Unicast.size(); j++) {
                    Unicast[j] = Napi::String::New(Env(), Interface.Unicast.at(j));
                }
                oInterface.Set("unicast", Unicast);
            };

            Callback().Call({Env().Null(), ret});
        }
};

/*
 * Complete list of Interfaces! (JavaScript Binding)
 */
Napi::Value getAdaptersAddresses(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Function cb;

    if (info.Length() < 1) {
        Napi::Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[0].IsFunction()) {
        Napi::Error::New(env, "argument callback should be a Function!").ThrowAsJavaScriptException();
        return env.Null();
    }

    cb = info[0].As<Napi::Function>();
    (new GetAdapterAddrWorker(cb))->Queue();

    return env.Undefined();
}

/*
 * Translate MIB_IF_ROW2 into Napi::Object
 * This method is used by getIfEntry and getIfTable binding
 */
Napi::Object translateIfRow(Napi::Env env, MIB_IF_ROW2 ifRow) {
    Napi::Object ret = Napi::Object::New(env);

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
class GetIfEntryWorker : public Napi::AsyncWorker {
    public:
        GetIfEntryWorker(Napi::Function& callback, NET_IFINDEX ifIndex) : AsyncWorker(callback), ifIndex(ifIndex) {}
        ~GetIfEntryWorker() {}
    private:
        MIB_IF_ROW2 ifRow;
        NET_IFINDEX ifIndex;

        void Execute(){
            SecureZeroMemory(&ifRow, sizeof(ifRow));
            ifRow.InterfaceIndex = ifIndex;
            DWORD error = GetIfEntry2(&ifRow);
            if (error != NO_ERROR) {
                std::stringstream errStr;
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
            Napi::HandleScope scope(Env());
            Callback().Call({Env().Null(), translateIfRow(Env(), ifRow)});
        }
};

/*
 * getIfEntry retrieves information for the specified interface on the local computer.
 *
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getifentry
 * @doc: https://docs.microsoft.com/en-us/previous-versions/windows/desktop/api/ifmib/ns-ifmib-_mib_ifrow
 */
Napi::Value getIfEntry(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Function cb;
    NET_IFINDEX ifIndex;

    if (info.Length() < 2) {
        Napi::Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[0].IsNumber()) {
        Napi::Error::New(env, "argument ifIndex should be typeof number!").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[1].IsFunction()) {
        Napi::Error::New(env, "argument callback should be a Function!").ThrowAsJavaScriptException();
        return env.Null();
    }

    ifIndex = (NET_IFINDEX) info[0].As<Napi::Number>().Int64Value();
    cb = info[1].As<Napi::Function>();
    (new GetIfEntryWorker(cb, ifIndex))->Queue();

    return env.Undefined();
}

/*
 * Retrieves the MIB-II interface table.
 *
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/netioapi/nf-netioapi-getiftable2
 */
class GetIfTableWorker : public Napi::AsyncWorker {
    public:
        GetIfTableWorker(Napi::Function& callback) : AsyncWorker(callback) {}
        ~GetIfTableWorker() {}
    private:
        PMIB_IF_TABLE2 ifTable;

        void Execute(){
            if (GetIfTable2(&ifTable) != NO_ERROR) {
                return SetError("Failed to retrieve ifTable");
            }
        }

        void OnError(const Napi::Error& e) {
            DWORD errorCode = GetLastError();
            std::stringstream error;
            error << e.what();
            if (errorCode != 0) {
                error << " - code (" << errorCode << ") - " << getLastErrorMessage();
            }

            Callback().Call({Napi::String::New(Env(), error.str()), Env().Null()});
        }

        void OnOK() {
            Napi::HandleScope scope(Env());
            Napi::Array ret = Napi::Array::New(Env());
            for (int i = 0; i < (int) ifTable->NumEntries; ++i) {
                ret[i] = translateIfRow(Env(), ifTable->Table[i]);
            }

            Callback().Call({Env().Null(), ret});
        }
};

/*
 * Retrieves the MIB-II interfaces table. (JavaScript Binding)
 */
Napi::Value getIfTable(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Function cb;

    if (info.Length() < 1) {
        Napi::Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[0].IsFunction()) {
        Napi::Error::New(env, "argument callback should be a Function!").ThrowAsJavaScriptException();
        return env.Null();
    }

    cb = info[0].As<Napi::Function>();
    (new GetIfTableWorker(cb))->Queue();

    return env.Undefined();
}

/*
 * Retrieves the number of interfaces on the local computer.
 *
 * @doc: https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getnumberofinterfaces
 */
class GetInterfaceNumberWorker : public Napi::AsyncWorker {
    public:
        GetInterfaceNumberWorker(Napi::Function& callback) : AsyncWorker(callback) {}
        ~GetInterfaceNumberWorker() {}
    private:
        DWORD numInterfaces;

        void Execute(){
            if (GetNumberOfInterfaces(&numInterfaces) != NO_ERROR) {
                return SetError("Failed to retrieve the number of interfaces on the local computer!");
            }
        }

        void OnOK() {
            Napi::HandleScope scope(Env());
            Callback().Call({Env().Null(), Napi::Number::New(Env(), numInterfaces)});
        }
};

/*
 * Retrieves the number of interfaces on the local computer (JavaScript Binding).
 */
Napi::Value getNumberOfInterfaces(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Function cb;

    if (info.Length() < 1) {
        Napi::Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[0].IsFunction()) {
        Napi::Error::New(env, "argument callback should be a Function!").ThrowAsJavaScriptException();
        return env.Null();
    }

    cb = info[0].As<Napi::Function>();
    (new GetInterfaceNumberWorker(cb))->Queue();

    return env.Undefined();
}

/*
 * Initialize Node.js addon binding exports
 */
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getAdaptersAddresses", Napi::Function::New(env, getAdaptersAddresses));
    exports.Set("getIfEntry", Napi::Function::New(env, getIfEntry));
    exports.Set("getIfTable", Napi::Function::New(env, getIfTable));
    exports.Set("getNumberOfInterfaces", Napi::Function::New(env, getNumberOfInterfaces));

    return exports;
}

NODE_API_MODULE(winni, Init)
