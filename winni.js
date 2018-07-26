/** @type {Winni} */
const winni = require("./build/Release/winni.node");

console.time("getInterfaces");
const networkInterfaces = winni.getInterfaces();
console.timeEnd("getInterfaces"); 

for (const interface of networkInterfaces) {
    console.log(`Interface Name => ${interface.Name}`);
    console.log(winni.getIfEntry(interface.IfIndex));
    console.log("----------\n");
}
