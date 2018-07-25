declare namespace Winni {
    interface NetworkInterface {
        Name: string;
        IfIndex: number;
        IfType: number;
        Length: number;
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
    }

    export function getInterfaces(): NetworkInterface[];
    export function getIfEntry(IfIndex: number): IfEntry;
}

export as namespace Winni;
export = Winni;
