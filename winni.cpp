#include <iostream>
#include <string>
#include "NetworkAdapters.h"
#include "node_api.h"
#include "assert.h"
using namespace std;

// void main() {
//     printf("Start main\n");
//     NetworkAdapters Adapters;
//     printf("Get interfaces...\n");
//     std::vector<NetworkInterface> vInterfaces = Adapters.GetInterfaces();
//     for (int i = 0; i < vInterfaces.size(); i++) {
//         NetworkInterface Interface = vInterfaces[i];
//         printf("\tAdapter name: %s\n", Interface.Name);

//         IfEntry ifInfo = Adapters.GetIf(Interface.Index);
//         printf("\tdwOutOctets: %s Octets\n", to_string(ifInfo.dwOutOctets).c_str());
//     }
// }

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

            /** Setup NAME property */
            napi_value interfaceUTF8StringName;
            status = napi_create_string_utf8(env, Interface.Name.c_str(), Interface.Name.length(), &interfaceUTF8StringName);
            assert(status == napi_ok);

            status = napi_set_named_property(env, JSInterfaceObject, "Name", interfaceUTF8StringName);
            assert(status == napi_ok);

            /** Setup DnsSuffix property */
            napi_value dnsSuffixUTF8String;
            status = napi_create_string_utf8(env, Interface.DnsSuffix, strlen(Interface.DnsSuffix), &dnsSuffixUTF8String);
            assert(status == napi_ok);

            status = napi_set_named_property(env, JSInterfaceObject, "DnsSuffix", dnsSuffixUTF8String);
            assert(status == napi_ok);

            /** Setup Description property */
            napi_value descriptionUTF8String;
            status = napi_create_string_utf8(env, Interface.Description, strlen(Interface.Description), &descriptionUTF8String);
            assert(status == napi_ok);

            status = napi_set_named_property(env, JSInterfaceObject, "Description", descriptionUTF8String);
            assert(status == napi_ok);

            /** Setup FriendlyName property */
            napi_value friendlyNameUTF8String;
            status = napi_create_string_utf8(env, Interface.FriendlyName, strlen(Interface.FriendlyName), &friendlyNameUTF8String);
            assert(status == napi_ok);

            status = napi_set_named_property(env, JSInterfaceObject, "FriendlyName", friendlyNameUTF8String);
            assert(status == napi_ok);

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
