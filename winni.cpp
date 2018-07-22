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
    napi_value networkproviders;

    status = napi_create_array(env, &networkproviders);
    assert(status == napi_ok);

    NetworkAdapters Adapters;
    std::vector<NetworkInterface> vInterfaces = Adapters.GetInterfaces();
    for (int i = 0; i < vInterfaces.size(); i++) {
        NetworkInterface Interface = vInterfaces[i];
        napi_value nInterface;

        status = napi_create_object(env, &nInterface);
        assert(status == napi_ok);

        napi_value interfaceName;
        status = napi_create_string_utf8(env, std::string(Interface.Name).c_str(), 10, &interfaceName);
        assert(status == napi_ok);

        status = napi_set_named_property(env, nInterface, "name", interfaceName);
        assert(status == napi_ok);

        napi_value index;
        status = napi_create_int32(env, i, &index);
        assert(status == napi_ok);

        status = napi_set_property(env, networkproviders, index, nInterface);
        assert(status == napi_ok);
    };

    return networkproviders;
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
