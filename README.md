# Winni

SlimIO Windows Network Interfaces which expose low-level Microsoft APIs on Network Interfaces, Adapter Addresses and IF_ROW.

This binding expose the following methods/struct:
- [GetAdaptersAddresses](https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getadaptersaddresses)
- [IP_ADAPTER_ADDRESSES](https://docs.microsoft.com/en-us/windows/desktop/api/iptypes/ns-iptypes-_ip_adapter_addresses_lh)
- [GetIfEntry2](https://docs.microsoft.com/en-us/windows/desktop/api/netioapi/nf-netioapi-getifentry2)
- [GetIfTable2](https://docs.microsoft.com/en-us/windows/desktop/api/netioapi/nf-netioapi-getiftable2)
- [MIB_IFROW](https://docs.microsoft.com/en-us/previous-versions/windows/desktop/api/ifmib/ns-ifmib-_mib_ifrow)
- [GetNumberOfInterfaces](https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getnumberofinterfaces)

> !! All methods are fully Asynchronous !!

## Getting Started

This package is available in the Node Package Repository and can be easily installed with [npm](https://docs.npmjs.com/getting-started/what-is-npm) or [yarn](https://yarnpkg.com).

```bash
$ npm i @slimio/winni
# or
$ yarn add @slimio/winni
```

## Usage example
Retrieve Adapter Addresses and for each of them retrieve the ifRow with the ifIndex of the interface!

```js
const { getAdaptersAddresses, getIfEntry } = require("@slimio/winni");

async function main() {
    const networkInterfaces = await getAdaptersAddresses();
    for (const iNet of networkInterfaces) {
        console.log(`Interface name: ${iNet.name}`);
        const ifRow = await getIfEntry(iNet.ifIndex);
        console.log(JSON.stringify(ifRow, null, 4));
    }
}
main().catch(console.error);
```

## API

### getAdaptersAddresses(): Promise< NetworkInterface[] >
Retrieves the addresses associated with the adapters on the local computer.

```ts
export interface NetworkInterface {
    name: string;
    ifIndex: number;
    ifType: number;
    length: number;
    physicalAddress: string;
    zoneIndices: number[];
    dnsSuffix: string;
    description: string;
    friendlyName: string;
    flags: number;
    mtu: number;
    operStatus: number;
    transmitLinkSpeed: number;
    receiveLinkSpeed: number;
    ipv4Enabled: boolean;
    ipv6Enabled: boolean;
    ipv6IfIndex: number;
    dnsEnabled: boolean;
    registerAdapterSuffix: number;
    receiveOnly: boolean;
    noMulticast: boolean;
    ipv6OtherStatefulConfig: boolean;
    netbiosOverTcpipEnabled: boolean;
    ipv6ManagedAddressConfigurationSupported: boolean;
    networkGuid: string;
    connectionType: number;
    tunnelType: number;
    dhcpv6ClientDuid: string;
    ipv4Metric: number;
    ipv6Metric: number;
    dnServer: string[];
    anycast: string[];
    unicast: string[];
    multicast: string[];
}
```

### getIfEntry(IfIndex: number): Promise< IfEntry >
Retrieves information for the specified interface on the local computer.

```ts
export interface IfEntry {
    physicalAddress: string;
    interfaceLuid: number;
    interfaceIndex: number;
    interfaceGuid: string;
    alias: string;
    description: string;
    mtu: number;
    type: number;
    tunnelType: number;
    mediaType: number;
    accessType: number;
    physicalMediumType: number;
    directionType: number;
    operStatus: number;
    adminStatus: number;
    mediaConnectState: number;
    networkGuid: string;
    connectionType: number;
    transmitLinkSpeed: number;
    receiveLinkSpeed: number;
    inOctets: number;
    inUcastPkts: number;
    inNUcastPkts: number;
    inDiscards: number;
    inErrors: number;
    inUnknownProtos: number;
    inUcastOctets: number;
    inMulticastOctets: number;
    inBroadcastOctets: number;
    outOctets: number;
    outUcastPkts: number;
    outNUcastPkts: number;
    outDiscards: number;
    outErrors: number;
    outUcastOctets: number;
    outMulticastOctets: number;
    outBroadcastOctets: number;
    outQLen: number;
}
```

### getIfTable(): Promise< IfEntry[] >
Retrieves the MIB-II interfaces table (which contain all ifRow of all interfaces).

### getNumberOfInterfaces(): Promise< number >
Retrieves the number of interfaces on the local computer.

## How to build the project

Before building the project, be sure to get the following npm package installed:

- Install (or upgrade to) NodeJS v10+ and npm v6+
- [Windows build tools](https://www.npmjs.com/package/windows-build-tools)

Then, just run normal npm install command:

```bash
$ npm install
```

## Available commands

All projects commands are described here:

| command | description |
| --- | --- |
| npm run prebuild | Generate addon prebuild |
| npm run doc | Generate JSDoc .HTML documentation (in the /docs root directory) |
| npm run coverage | Generate coverage of tests |
| npm run report | Generate .HTML report of tests coverage |

> the report command have to be triggered after the coverage command.
