var className = "ViewController";
var funcName = "- viewDidLoad";

function implementation() {
    console.log("start hook -[ViewController viewDidLoad]\n");

    var method = ObjC.classes[className][funcName];

    var origImp = method.implementation;
    console.log("\nBefore => Imp Addr:" + origImp);

    var func_pointer = method.implementation;
    console.log("\nBefore hook, head instructions:\n");

    var ins = Instruction.parse(func_pointer);
    for (var i = 0; i < 100000; i++) {
        console.log(ins.toString());
        if (ins.toString() === "ret") {
            break;
        }
        ins = Instruction.parse(ins.next);
    }

    method.implementation = ObjC.implement(method, function (self, selector) {
        return origImp(self, selector);
    });

    console.log("\nAfter hook, head instructions:\n");
    var ins2 = Instruction.parse(func_pointer);

    for (var i = 0; i < 100000; i++) {
        console.log(ins2.toString());
        if (ins2.toString() === "ret") {
            break;
        }
        ins2 = Instruction.parse(ins2.next);
    }
    console.log("\nAfter => Imp Addr:" + method.implementation);
}


function attach() {
    var hook = eval("ObjC.classes." + className + '["' + funcName + '"]');
    console.log("[*] Class Name: " + className);
    console.log("[*] Method Name: \n" + funcName);
    console.log("BeforeImp Addr:" + hook.implementation);

    var func_pointer = hook.implementation;
    console.log("\nBefore hook, head instructions:");
    var ins = Instruction.parse(func_pointer);
    for (var i = 0; i < 100000; i++) {
        console.log(ins.toString());
        if (ins.toString() === "ret") {
            break;
        }
        ins = Instruction.parse(ins.next);
    }

    Interceptor.attach(hook.implementation, {
        onEnter: function (args) { },
        onLeave: function (returnValue) { },
    });

    console.log("\nAfter hook, head instructions:\n");
    var ins2 = Instruction.parse(func_pointer);
    console.log(ins2.toString());
    ins2 = Instruction.parse(ins2.next);
    console.log(ins2.toString());

    hook = eval("\nObjC.classes." + className + '["' + funcName + '"]');
    console.log("\nAfter => Imp Addr:" + hook.implementation);
}


function dump() {
    var hook = eval("ObjC.classes." + className + '["' + funcName + '"]');
    var func_pointer = hook.implementation;
    console.log("Before hook, head instructions: \n");
    var ins = Instruction.parse(func_pointer);
    for (var i = 0; i < 100000; i++) {
        console.log(
            hexdump(ins.address, {
                offset: 0,
                length: ins.size,
                header: false,
                ansi: false,
            }),
            ins.toString()
        );
        if (ins.toString() === 'ret') {
            break;
        }
        ins = Instruction.parse(ins.next);
    }

    Interceptor.attach(hook.implementation, {
        onEnter: function (args) { },
        onLeave: function (returnValue) { },
    });

    console.log("\nAfter hook, head instructions: \n");
    var ins2 = Instruction.parse(func_pointer);
    console.log(
        hexdump(ins2.address, {
            offset: 0,
            length: ins2.size,
            header: false,
            ansi: false,
        }),
        ins2.toString()
    );
    ins2 = Instruction.parse(ins2.next);
    console.log(
        hexdump(ins2.address, {
            offset: 0,
            length: ins2.size,
            header: false,
            ansi: false,
        }),
        ins2.toString()
    );

    hook = eval("ObjC.classes." + className + '["' + funcName + '"]');
    console.log('\nAfter => Imp Addr:' + hook.implementation);
}

dump();
