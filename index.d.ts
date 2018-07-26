declare namespace Winni {
    interface NetworkInterface {
        Name: string;
        IfIndex: number;
        IfType: number;
        Length: number;
        PhysicalAddress: string;
        DnsSuffix: string;
        Description: string;
        FriendlyName: string;
        Flags: number;
        Mtu: number;
        OperStatus: number;
        TransmitLinkSpeed: number;
        ReceiveLinkSpeed: number;
        Ipv4Enabled: number;
        Ipv6Enabled: number;
        Ipv6IfIndex: number;
    }

    interface IfEntry {
        dwOutOctets: number;
        dwInOctets: number;
        dwInDiscards: number;
        dwInErrors: number;
        dwOutDiscards: number;
        dwOutErrors: number;
        dwSpeed: number;
        dwLastChange: number;
        dwInNUcastPkts: number;
        dwOutNUcastPkts: number;
        dwOutUcastPkts: number;
        dwInUcastPkts: number;
        dwOutQLen: number;
        dwInUnknownProtos: number;
    }

    export function getInterfaces(): NetworkInterface[];
    export function getIfEntry(IfIndex: number): IfEntry;
}

export as namespace Winni;
export = Winni;
