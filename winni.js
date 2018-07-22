const addon = require('bindings')('winni');

console.log(addon);
console.log("interfaces :");
console.log(addon.getInterfaces());
