"use strict";

/**
 * @namespace winni
 * @description Windows Network interfaces - Node.JS low level binding
 */
const winni = require("node-gyp-build")(__dirname);

/**
 * @version 1.0.0
 *
 * @async
 * @function getAdaptersAddresses
 * @memberof winni#
 * @description Retrieves the addresses associated with the adapters on the local computer.
 * @returns {Promise<Winni.NetworkInterface[]>}
 *
 * @example
 * const { getAdaptersAddresses } = require("@slimio/winni");
 *
 * async function main() {
 *     const adapterAddr = await getAdaptersAddresses();
 *     for (const networkInterface of adapterAddr) {
 *         console.log(`networkInterface name => ${networkInterface.Name}`);
 *     }
 * }
 * main().catch(console.error)
 */
function getAdaptersAddresses() {
    return new Promise((resolve, reject) => {
        winni.getAdaptersAddresses((error, adapterAddr) => {
            if (error) {
                return reject(error);
            }

            return resolve(adapterAddr);
        });
    });
}

/**
 * @version 1.0.0
 *
 * @async
 * @function getIfTable
 * @memberof winni#
 * @description Retrieves the MIB-II interfaces table
 * @returns {Promise<Winni.IfEntry[]>}
 *
 * @example
 * const { getIfTable } = require("@slimio/winni");
 *
 * async function main() {
 *     const ifTables = await getIfTable();
 *     for (const ifEntry of ifTables) {
 *         console.log(JSON.stringify(ifEntry, null, 4));
 *     }
 * }
 * main().catch(console.error)
 */
function getIfTable() {
    return new Promise((resolve, reject) => {
        winni.getIfTable((error, ifTable) => {
            if (error) {
                return reject(error);
            }

            return resolve(ifTable);
        });
    });
}

/**
 * @version 1.0.0
 *
 * @async
 * @function getIfEntry
 * @memberof winni#
 * @param {!number} ifIndex Interface IfIndex
 * @description Retrieves information for the specified interface on the local computer
 * @returns {Promise<Winni.IfEntry>}
 *
 * @example
 * const { getAdaptersAddresses, getIfEntry } = require("@slimio/winni");
 *
 * async function main() {
 *     const adapterAddr = await getAdaptersAddresses();
 *     for (const networkInterface of adapterAddr) {
 *         console.log(`networkInterface name => ${networkInterface.Name}`);
 *         const ifEntry = await getIfEntry(networkInterface.IfIndex);
 *         console.log(JSON.stringify(ifEntry, null, 4));
 *     }
 * }
 * main().catch(console.error)
 */
function getIfEntry(ifIndex) {
    return new Promise((resolve, reject) => {
        winni.getIfEntry(ifIndex, (error, ifEntry) => {
            if (error) {
                return reject(error);
            }

            return resolve(ifEntry);
        });
    });
}

/**
 * @version 1.0.0
 *
 * @async
 * @function getNumberOfInterfaces
 * @memberof winni#
 * @description Retrieves the number of interfaces on the local computer
 * @returns {Promise<number>}
 *
 * @example
 * const { getNumberOfInterfaces } = require("@slimio/winni");
 *
 * async function main() {
 *     const nbrOfInterfaces = await getNumberOfInterfaces();
 *     console.log(`Number of interfaces on the local computer: ${nbrOfInterfaces}`);
 * }
 * main().catch(console.error)
 */
function getNumberOfInterfaces() {
    return new Promise((resolve, reject) => {
        winni.getNumberOfInterfaces((error, nb) => {
            if (error) {
                return reject(error);
            }

            return resolve(nb);
        });
    });
}

module.exports = {
    getAdaptersAddresses,
    getIfTable,
    getIfEntry,
    getNumberOfInterfaces
};
