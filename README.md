# Winni
![V1.1](https://img.shields.io/badge/version-1.1.0-blue.svg)
![N-API](https://img.shields.io/badge/N--API-v3-green.svg)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/SlimIO/Winni/commit-activity)
[![GitHub license](https://img.shields.io/github/license/Naereen/StrapDown.js.svg)](https://github.com/SlimIO/Queue/blob/master/LICENSE)
![2DEP](https://img.shields.io/badge/Dependencies-2-yellow.svg)
[![Known Vulnerabilities](https://snyk.io/test/github/SlimIO/Winni/badge.svg?targetFile=package.json)](https://snyk.io/test/github/SlimIO/Winni?targetFile=package.json)
[![Build Status](https://travis-ci.com/SlimIO/Winni.svg?branch=master)](https://travis-ci.com/SlimIO/Winni) [![Greenkeeper badge](https://badges.greenkeeper.io/SlimIO/Winni.svg)](https://greenkeeper.io/)

SlimIO Windows Network Interfaces which expose low-level Microsoft APIs on Network Interfaces, Adapter Addresses and IF_ROW.

This binding expose the following methods/struct:
- [GetAdaptersAddresses](https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getadaptersaddresses)
- [IP_ADAPTER_ADDRESSES](https://docs.microsoft.com/en-us/windows/desktop/api/iptypes/ns-iptypes-_ip_adapter_addresses_lh)
- [GetIfEntry2](https://docs.microsoft.com/en-us/windows/desktop/api/netioapi/nf-netioapi-getifentry2)
- [GetIfTable2](https://docs.microsoft.com/en-us/windows/desktop/api/netioapi/nf-netioapi-getiftable2)
- [MIB_IFROW](https://docs.microsoft.com/en-us/previous-versions/windows/desktop/api/ifmib/ns-ifmib-_mib_ifrow)
- [GetNumberOfInterfaces](https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getnumberofinterfaces)

## Requirements
- Node.js v10 or higher

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

## Contribution Guidelines
To contribute to the project, please read the [code of conduct](https://github.com/SlimIO/Governance/blob/master/COC_POLICY.md) and the guide for [N-API compilation](https://github.com/SlimIO/Governance/blob/master/docs/native_addons.md).

## License
MIT
