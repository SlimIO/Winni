/** @type {Winni} */
const winni = require("./build/Release/winni.node");

console.time("getInterfaces");
const networkInterfaces = winni.getInterfaces();
console.timeEnd("getInterfaces"); 

for (const interface of networkInterfaces) {
    console.log(interface);
    console.time("getIfEntry");
    const ifEntry = winni.getIfEntry(interface.IfIndex);
    console.timeEnd("getIfEntry");
    console.log(ifEntry);
    console.log("----------\n");
}
