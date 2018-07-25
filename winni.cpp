#include <iostream>
#include <string>
#include "NetworkAdapters.h"
#include "node_api.h"
#include "assert.h"
using namespace std;

void addDoubleProperty(napi_env env, napi_value JSObject, char* fieldName, double fieldValue) {
    napi_value doubleNAPIValue;
    napi_status status;
    status = napi_create_double(env, fieldValue, &doubleNAPIValue);
    assert(status == napi_ok);

    status = napi_set_named_property(env, JSObject, fieldName, doubleNAPIValue);
    assert(status == napi_ok);
}

void addCharProperty(napi_env env, napi_value JSObject, char* fieldName, char* fieldValue) {
    napi_value charNAPIValue;
    napi_status status;
    status = napi_create_string_utf8(env, fieldValue, strlen(fieldValue), &charNAPIValue);
    assert(status == napi_ok);

    status = napi_set_named_property(env, JSObject, fieldName, charNAPIValue);
    assert(status == napi_ok);
}

napi_value GetInterfaces(napi_env env, napi_callback_info info) {
    napi_status status;
    NetworkAdapters Adapters;

    // Create JavaScript Array
    napi_value ArrayRet;
    status = napi_create_array(env, &ArrayRet);
    assert(status == napi_ok);

    try {
        // Retrieve interfaces
        vector<NetworkInterface> vInterfaces = Adapters.GetInterfaces();
        for (int i = 0; i < vInterfaces.size(); i++) {
            NetworkInterface Interface = vInterfaces[i];

            // Create JavaScript Object
            napi_value JSInterfaceObject;
            status = napi_create_object(env, &JSInterfaceObject);
            assert(status == napi_ok);

            /** Setup Properties */
            addDoubleProperty(env, JSInterfaceObject, "IfIndex", Interface.IfIndex);
            addDoubleProperty(env, JSInterfaceObject, "IfType", Interface.IfType);
            addCharProperty(env, JSInterfaceObject, "Name", (char*) Interface.Name);
            addCharProperty(env, JSInterfaceObject, "DnsSuffix", Interface.DnsSuffix);
            addCharProperty(env, JSInterfaceObject, "Description", Interface.Description);
            addCharProperty(env, JSInterfaceObject, "FriendlyName", Interface.FriendlyName);
            addDoubleProperty(env, JSInterfaceObject, "Flags", Interface.Flags);
            addDoubleProperty(env, JSInterfaceObject, "Length", Interface.Length);
            addDoubleProperty(env, JSInterfaceObject, "Mtu", Interface.Mtu);
            addDoubleProperty(env, JSInterfaceObject, "OperStatus", Interface.OperStatus);
            addDoubleProperty(env, JSInterfaceObject, "ReceiveLinkSpeed", Interface.ReceiveLinkSpeed);
            addDoubleProperty(env, JSInterfaceObject, "TransmitLinkSpeed", Interface.TransmitLinkSpeed);
            addDoubleProperty(env, JSInterfaceObject, "Ipv4Enabled", Interface.Ipv4Enabled);
            addDoubleProperty(env, JSInterfaceObject, "Ipv6Enabled", Interface.Ipv6Enabled);
            addDoubleProperty(env, JSInterfaceObject, "Ipv6IfIndex", Interface.Ipv6IfIndex);

            /** Create array entry **/
            napi_value index;
            status = napi_create_int32(env, i, &index);
            assert(status == napi_ok);

            status = napi_set_property(env, ArrayRet, index, JSInterfaceObject);
            assert(status == napi_ok);
        };
    }
    catch(...) {
        bool result = false;
        napi_is_exception_pending(env, &result);
        if (result) {
            const napi_extended_error_info *errorInfo = 0;
            napi_get_last_error_info(env, &errorInfo);
            napi_throw_error(env, "ERR", errorInfo->error_message);
        }
        else {
            napi_throw_error(env, "ERR", "Unknow error!");
        }
        return NULL;
    }

    return ArrayRet;
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_property_descriptor desc = DECLARE_NAPI_METHOD("getInterfaces", GetInterfaces);
    status = napi_define_properties(env, exports, 1, &desc);
    assert(status == napi_ok);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
