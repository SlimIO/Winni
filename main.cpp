#include <iostream>
#include <vector>
#include "NetworkAdapters.h"
using namespace std;

void main() {
    printf("Start main\n");
    NetworkAdapters Adapters;
    printf("Get interfaces...\n");
    std::vector<NetworkInterface> vInterfaces = Adapters.GetInterfaces();
    for (int i = 0; i < vInterfaces.size(); i++) {
        NetworkInterface Interface = vInterfaces[i];
        printf("\tAdapter name: %s\n", Interface.Name);
    }
}
