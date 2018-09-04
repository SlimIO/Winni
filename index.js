/** @type {Winni} */
const winni = require("./build/Release/winni.node");

console.time("getNumberOfInterfaces");
const interfaceNumber = winni.getNumberOfInterfaces();
console.timeEnd("getNumberOfInterfaces");

console.log(`number of interfaces: ${interfaceNumber}\n`);

console.time("getInterfaces");
const networkInterfaces = winni.getAdaptersAddresses();
console.timeEnd("getInterfaces"); 

for (const interface of networkInterfaces) {
    console.log(interface.Name);
    console.log(interface.IfIndex);
    console.time("getIfEntry");
    const ifEntry = winni.getIfEntry(interface.IfIndex);
    console.timeEnd("getIfEntry");
    console.log(ifEntry);
    console.log("----------\n");
}
