#include <winsock2.h>
#include "NetworkAdapters.h"
#include "node_api.h"
#include "napi.h"
using namespace Napi;
using namespace std;

/*
 * Complete list of Interfaces!
 */
Value getInterfaces(const CallbackInfo& info) {
    Env env = info.Env();
    NetworkAdapters Adapters;

    // Create JavaScript Array
    Array ret = Array::New(env);

    // Retrieve interfaces
    vector<NetworkInterface> vInterfaces = Adapters.GetInterfaces();
    for (unsigned int i = 0; i < vInterfaces.size(); i++) {
        NetworkInterface Interface = vInterfaces[i];
        Object oInterface = Object::New(env);
        ret[i] = oInterface;

        /** Setup Properties */
        oInterface.Set("IfIndex", Interface.IfIndex);
        oInterface.Set("IfType", Interface.IfType);
        oInterface.Set("Name", Interface.Name);
        oInterface.Set("PhysicalAddress", String::New(env, Interface.PhysicalAddress));
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
        for(unsigned int j = 0; j < 16; j++) {
            ZoneIndices[j] = Number::New(env, Interface.ZoneIndices[j]);
        }
        oInterface.Set("ZoneIndices", ZoneIndices);
    };

    return ret;
}

/*
 * Get Interfaces ifEntry with ifIndex
 */
Value getIfEntry(const CallbackInfo& info) {
    Env env = info.Env();

    // Check argument length!
    if (info.Length() < 1) {
        Error::New(env, "Wrong number of argument provided!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // DriveName should be typeof string
    if (!info[0].IsNumber()) {
        Error::New(env, "argument ifIndex should be typeof number!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Retrieve ifIndex
    double ifIndex = info[0].As<Number>().DoubleValue();

    MIB_IFROW ifrow;
    ifrow.dwIndex = (IF_INDEX) ifIndex;
    if(GetIfEntry(&ifrow) != NO_ERROR ) {
        Error::New(env, "Failed to retrieve ifEntry!").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    // Create JavaScript Object
    Object ret = Object::New(env);
    ret.Set("dwOutOctets", ifrow.dwOutOctets);
    ret.Set("dwInOctets", ifrow.dwInOctets);
    ret.Set("dwInDiscards", ifrow.dwInDiscards);
    ret.Set("dwInErrors", ifrow.dwInErrors);
    ret.Set("dwOutDiscards", ifrow.dwOutDiscards);
    ret.Set("dwOutErrors", ifrow.dwOutErrors);
    ret.Set("dwSpeed", ifrow.dwSpeed);
    ret.Set("dwLastChange", ifrow.dwLastChange);
    ret.Set("dwInNUcastPkts", ifrow.dwInNUcastPkts);
    ret.Set("dwOutNUcastPkts", ifrow.dwOutNUcastPkts);
    ret.Set("dwOutUcastPkts", ifrow.dwOutUcastPkts);
    ret.Set("dwInUcastPkts", ifrow.dwInUcastPkts);
    ret.Set("dwOutQLen", ifrow.dwOutQLen);
    ret.Set("dwInUnknownProtos", ifrow.dwInUnknownProtos);

    return ret;
}


// Initialize Native Addon
Object Init(Env env, Object exports) {

    // Setup methods
    // TODO: Launch with AsyncWorker to avoid event loop starvation
    exports.Set("getInterfaces", Function::New(env, getInterfaces));
    exports.Set("getIfEntry", Function::New(env, getIfEntry));

    return exports;
}

// Export
NODE_API_MODULE(winni, Init)
