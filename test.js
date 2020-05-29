// require("process")
// console.log(process.dlopen(module, ''));
//console.log(process);

const active_window = require("./");

console.log(active_window);

setTimeout(() => {
    console.log(`active window: `, active_window.getActiveWindow());
}, 3000);
