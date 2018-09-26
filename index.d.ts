declare namespace Winni {
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

    export function getAdaptersAddresses(): Promise<NetworkInterface[]>;
    export function getIfEntry(IfIndex: number): Promise<IfEntry>;
    export function getIfTable(): Promise<IfEntry[]>;
    export function getNumberOfInterfaces(): Promise<number>;
}

export as namespace Winni;
export = Winni;
