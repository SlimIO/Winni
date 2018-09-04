declare namespace Winni {
    interface NetworkInterface {
        Name: string;
        IfIndex: number;
        IfType: number;
        Length: number;
        PhysicalAddress: string;
        ZoneIndices: number[];
        DnsSuffix: string;
        Description: string;
        FriendlyName: string;
        Flags: number;
        Mtu: number;
        OperStatus: number;
        TransmitLinkSpeed: number;
        ReceiveLinkSpeed: number;
        Ipv4Enabled: boolean;
        Ipv6Enabled: boolean;
        Ipv6IfIndex: number;
        DdnsEnabled: boolean;
        RegisterAdapterSuffix: number;
        ReceiveOnly: boolean;
        NoMulticast: boolean;
        Ipv6OtherStatefulConfig: boolean;
        NetbiosOverTcpipEnabled: boolean;
        Ipv6ManagedAddressConfigurationSupported: boolean;
    }

    interface IfEntry {
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
        outUnknownProtos: number;
        outUcastOctets: number;
        outMulticastOctets: number;
        outBroadcastOctets: number;
        outQLen: number;
    }

    export function getAdaptersAddresses(): NetworkInterface[];
    export function getIfEntry(IfIndex: number): IfEntry;
    export function getIfTable(): IfEntry[];
    export function getNumberOfInterfaces(): number;
}

export as namespace Winni;
export = Winni;
