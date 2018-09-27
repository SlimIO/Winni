// Require Third-party dependencies
const test = require("ava");
const is = require("@slimio/is");

// Require package
const winni = require("../index");

test("getAdaptersAddresses()", async function getAdaptersAddresses(assert) {
    assert.true(Reflect.has(winni, "getAdaptersAddresses"));

    const adapterAddr = await winni.getAdaptersAddresses();
    assert.true(is.array(adapterAddr));
    for (const networkInterface of adapterAddr) {
        assert.true(is.plainObject(networkInterface));
        assert.true(is.string(networkInterface.name));
        assert.true(is.number(networkInterface.ifIndex));
        assert.true(is.number(networkInterface.ifType));
        assert.true(is.number(networkInterface.length));
        assert.true(is.string(networkInterface.physicalAddress));
        assert.true(is.array(networkInterface.zoneIndices));
        assert.true(networkInterface.zoneIndices.length === 16);
        assert.true(networkInterface.zoneIndices.every(
            (indice) => is.number(indice)
        ));
        assert.true(is.string(networkInterface.dnsSuffix));
        assert.true(is.string(networkInterface.description));
        assert.true(is.string(networkInterface.friendlyName));
        assert.true(is.number(networkInterface.flags));
        assert.true(is.number(networkInterface.mtu));
        assert.true(is.number(networkInterface.operStatus));
        assert.true(is.number(networkInterface.transmitLinkSpeed));
        assert.true(is.number(networkInterface.receiveLinkSpeed));
        assert.true(is.bool(networkInterface.ipv4Enabled));
        assert.true(is.bool(networkInterface.ipv6Enabled));
        assert.true(is.number(networkInterface.ipv6IfIndex));
        assert.true(is.bool(networkInterface.dnsEnabled));
        assert.true(is.number(networkInterface.registerAdapterSuffix));
        assert.true(is.bool(networkInterface.receiveOnly));
        assert.true(is.bool(networkInterface.noMulticast));
        assert.true(is.bool(networkInterface.ipv6OtherStatefulConfig));
        assert.true(is.bool(networkInterface.netbiosOverTcpipEnabled));
        assert.true(is.bool(networkInterface.ipv6ManagedAddressConfigurationSupported));
        assert.true(is.string(networkInterface.networkGuid));
        assert.true(is.number(networkInterface.connectionType));
        assert.true(is.number(networkInterface.tunnelType));
        assert.true(is.string(networkInterface.dhcpv6ClientDuid));
        assert.true(is.number(networkInterface.ipv4Metric));
        assert.true(is.number(networkInterface.ipv6Metric));
        assert.true(is.array(networkInterface.dnServer));
        assert.true(is.array(networkInterface.anycast));
        assert.true(is.array(networkInterface.unicast));
        assert.true(is.array(networkInterface.multicast));
    }
});

test("getNumberOfInterfaces()", async function getNumberOfInterfaces(assert) {
    assert.true(Reflect.has(winni, "getNumberOfInterfaces"));

    const nbInterfaces = await winni.getNumberOfInterfaces();
    assert.true(is.number(nbInterfaces));
});

/**
 * @function checkIfEntry
 * @param {*} assert ava assertion
 * @param {Winni.IfEntry} ifEntry ifEntry
 * @returns {void}
 */
function checkIfEntry(assert, ifEntry) {
    assert.true(is.plainObject(ifEntry));
    assert.true(is.string(ifEntry.physicalAddress));
    assert.true(is.number(ifEntry.interfaceLuid));
    assert.true(is.number(ifEntry.interfaceIndex));
    assert.true(is.string(ifEntry.interfaceGuid));
    assert.true(is.string(ifEntry.alias));
    assert.true(is.string(ifEntry.description));
    assert.true(is.number(ifEntry.mtu));
    assert.true(is.number(ifEntry.type));
    assert.true(is.number(ifEntry.tunnelType));
    assert.true(is.number(ifEntry.mediaType));
    assert.true(is.number(ifEntry.accessType));
    assert.true(is.number(ifEntry.physicalMediumType));
    assert.true(is.number(ifEntry.directionType));
    assert.true(is.number(ifEntry.operStatus));
    assert.true(is.number(ifEntry.adminStatus));
    assert.true(is.number(ifEntry.mediaConnectState));
    assert.true(is.string(ifEntry.networkGuid));
    assert.true(is.number(ifEntry.connectionType));
    assert.true(is.number(ifEntry.transmitLinkSpeed));
    assert.true(is.number(ifEntry.receiveLinkSpeed));
    assert.true(is.number(ifEntry.inOctets));
    assert.true(is.number(ifEntry.inUcastPkts));
    assert.true(is.number(ifEntry.inNUcastPkts));
    assert.true(is.number(ifEntry.inDiscards));
    assert.true(is.number(ifEntry.inErrors));
    assert.true(is.number(ifEntry.inUnknownProtos));
    assert.true(is.number(ifEntry.inUcastOctets));
    assert.true(is.number(ifEntry.inMulticastOctets));
    assert.true(is.number(ifEntry.inBroadcastOctets));
    assert.true(is.number(ifEntry.outOctets));
    assert.true(is.number(ifEntry.outUcastPkts));
    assert.true(is.number(ifEntry.outNUcastPkts));
    assert.true(is.number(ifEntry.outDiscards));
    assert.true(is.number(ifEntry.outErrors));
    assert.true(is.number(ifEntry.outUcastOctets));
    assert.true(is.number(ifEntry.outMulticastOctets));
    assert.true(is.number(ifEntry.outBroadcastOctets));
    assert.true(is.number(ifEntry.outQLen));
}

test("getIfTable()", async function getIfTable(assert) {
    assert.true(Reflect.has(winni, "getIfTable"));

    const ifTable = await winni.getIfTable();
    assert.true(is.array(ifTable));
    for (const ifEntry of ifTable) {
        checkIfEntry(assert, ifEntry);
    }
});

test("getIfEntry() - ifIndex should be typeof number", async function getIfEntry(assert) {
    assert.true(Reflect.has(winni, "getIfEntry"));

    const error = await assert.throws(winni.getIfEntry("hello"), Error);
    assert.is(error.message, "argument ifIndex should be typeof number!");
});

test("getIfEntry()", async function getIfEntry(assert) {
    assert.true(Reflect.has(winni, "getAdaptersAddresses"));
    assert.true(Reflect.has(winni, "getIfEntry"));

    const adapterAddr = await winni.getAdaptersAddresses();
    assert.true(is.array(adapterAddr));
    for (const networkInterface of adapterAddr) {
        assert.true(is.plainObject(networkInterface));
        assert.true(is.number(networkInterface.ifIndex));
        const ifEntry = await winni.getIfEntry(networkInterface.ifIndex);
        checkIfEntry(assert, ifEntry);
    }
});
