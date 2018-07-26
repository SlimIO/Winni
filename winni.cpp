#include <iostream>
#include <string>
#include "NetworkAdapters.h"
#include "JavaScriptObject.h"
#include "node_api.h"
#include "assert.h"
using namespace std;

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value GetInterfaces(napi_env env, napi_callback_info info) {
    napi_status status;
    NetworkAdapters Adapters;

    // Create JavaScript Array
    napi_value JSInterfaceArray;
    status = napi_create_array(env, &JSInterfaceArray);
    assert(status == napi_ok);

    // Retrieve interfaces
    vector<NetworkInterface> vInterfaces = Adapters.GetInterfaces();
    for (int i = 0; i < vInterfaces.size(); i++) {
        NetworkInterface Interface = vInterfaces[i];
        JavaScriptObject JSInterfaceObject(env);

        /** Setup Properties */
        JSInterfaceObject.addDouble("IfIndex", Interface.IfIndex);
        JSInterfaceObject.addDouble("IfType", Interface.IfType);
        JSInterfaceObject.addString("Name", (char*) Interface.Name);
        JSInterfaceObject.addString("PhysicalAddress", Interface.PhysicalAddress);
        JSInterfaceObject.addString("DnsSuffix", Interface.DnsSuffix);
        JSInterfaceObject.addString("Description", Interface.Description);
        JSInterfaceObject.addString("FriendlyName", Interface.FriendlyName);
        JSInterfaceObject.addDouble("Flags", Interface.Flags);
        JSInterfaceObject.addDouble("Length", Interface.Length);
        JSInterfaceObject.addDouble("Mtu", Interface.Mtu);
        JSInterfaceObject.addDouble("OperStatus", Interface.OperStatus);
        JSInterfaceObject.addDouble("ReceiveLinkSpeed", Interface.ReceiveLinkSpeed);
        JSInterfaceObject.addDouble("TransmitLinkSpeed", Interface.TransmitLinkSpeed);
        JSInterfaceObject.addBool("Ipv4Enabled", Interface.Ipv4Enabled == 1 ? true : false );
        JSInterfaceObject.addBool("Ipv6Enabled", Interface.Ipv6Enabled == 1 ? true : false );
        JSInterfaceObject.addDouble("Ipv6IfIndex", Interface.Ipv6IfIndex);
        JSInterfaceObject.addBool("DdnsEnabled", Interface.DdnsEnabled == 1 ? true : false );
        JSInterfaceObject.addDouble("RegisterAdapterSuffix", Interface.RegisterAdapterSuffix);
        JSInterfaceObject.addBool("ReceiveOnly", Interface.ReceiveOnly == 1 ? true : false );
        JSInterfaceObject.addBool("NoMulticast", Interface.NoMulticast == 1 ? true : false );
        JSInterfaceObject.addBool("Ipv6OtherStatefulConfig", Interface.Ipv6OtherStatefulConfig == 1 ? true : false );
        JSInterfaceObject.addBool("NetbiosOverTcpipEnabled", Interface.NetbiosOverTcpipEnabled == 1 ? true : false );
        JSInterfaceObject.addBool("Ipv6ManagedAddressConfigurationSupported", Interface.Ipv6ManagedAddressConfigurationSupported == 1 ? true : false );

        /** Create array entry **/
        napi_value index;
        status = napi_create_int32(env, i, &index);
        assert(status == napi_ok);

        status = napi_set_property(env, JSInterfaceArray, index, JSInterfaceObject.getSelf());
        assert(status == napi_ok);
    };

    return JSInterfaceArray;
}

napi_value GetIfEntry(napi_env env, napi_callback_info info) {
    napi_status status;

    // Retrieve args
    size_t argc = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    // Check arguments length
    if (argc < 1) {
        napi_throw_type_error(env, nullptr, "Wrong number of arguments provided!");
        return nullptr;
    }

    // Get typeof of first arg
    napi_valuetype ifIndexNAPIType;
    status = napi_typeof(env, args[0], &ifIndexNAPIType);
    assert(status == napi_ok);

    if (ifIndexNAPIType != napi_number) {
        napi_throw_type_error(env, nullptr, "ifIndex argument should be typeof number");
        return nullptr;
    }

    double ifIndex;
    status = napi_get_value_double(env, args[0], &ifIndex);
    assert(status == napi_ok);

    NetworkAdapters Adapters;
    IfEntry ifEntry = Adapters.GetIf((IF_INDEX) ifIndex);
    
    // Create JavaScript Object
    JavaScriptObject JSInterfaceObject(env);

    JSInterfaceObject.addDouble("dwOutOctets", ifEntry.dwOutOctets);
    JSInterfaceObject.addDouble("dwInOctets", ifEntry.dwInOctets);
    JSInterfaceObject.addDouble("dwInDiscards", ifEntry.dwInDiscards);
    JSInterfaceObject.addDouble("dwInErrors", ifEntry.dwInErrors);
    JSInterfaceObject.addDouble("dwOutDiscards", ifEntry.dwOutDiscards);
    JSInterfaceObject.addDouble("dwOutErrors", ifEntry.dwOutErrors);
    JSInterfaceObject.addDouble("dwSpeed", ifEntry.dwSpeed);
    JSInterfaceObject.addDouble("dwLastChange", ifEntry.dwLastChange);
    JSInterfaceObject.addDouble("dwInNUcastPkts", ifEntry.dwInNUcastPkts);
    JSInterfaceObject.addDouble("dwOutNUcastPkts", ifEntry.dwOutNUcastPkts);
    JSInterfaceObject.addDouble("dwOutUcastPkts", ifEntry.dwOutUcastPkts);
    JSInterfaceObject.addDouble("dwInUcastPkts", ifEntry.dwInUcastPkts);
    JSInterfaceObject.addDouble("dwOutQLen", ifEntry.dwOutQLen);
    JSInterfaceObject.addDouble("dwInUnknownProtos", ifEntry.dwInUnknownProtos);

    return JSInterfaceObject.self;
}

napi_value Init(napi_env env, napi_value exports) {
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_METHOD("getInterfaces", GetInterfaces),
        DECLARE_NAPI_METHOD("getIfEntry", GetIfEntry)
    };
    napi_status status = napi_define_properties(env, exports, sizeof(desc) / sizeof(*desc), desc);
    assert(status == napi_ok);

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
