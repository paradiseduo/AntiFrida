# AntiFrida
What is Frida，Why is Frida， How about Frida

```bash
❯ frida -UF -l AntiFrida.js

// Four function But olny one File
❯ frida-trace -UF -m "-[👆🏻👇🏻👈🏻👉🏻* *]"
Instrumenting...
-[👆🏻👇🏻👈🏻👉🏻 👆🏻]: Auto-generated handler at "/Users/a/b/c/d/__handlers__/________/__.js"
-[👆🏻👇🏻👈🏻👉🏻 👇🏻]: Loaded handler at "/Users/a/b/c/d/__handlers__/________/__.js"
-[👆🏻👇🏻👈🏻👉🏻 👈🏻]: Loaded handler at "/Users/a/b/c/d/__handlers__/________/__.js"
-[👆🏻👇🏻👈🏻👉🏻 👉🏻]: Loaded handler at "/Users/a/b/c/d/__handlers__/________/__.js"
Started tracing 4 functions. Press Ctrl+C to stop.


// Not Found class
❯ frida-trace -UF -m "-[Test* *]" 
Started tracing 0 functions. Press Ctrl+C to stop.

```

If Frida Hook
```bash
0x58000050|0xd61f0200|0x093e4000|0x00000001|0xf81f03a1
0x58000050|0xd61f0200|0x04d68000|0x00000001|0xf81e83a1
0x58000050|0xd61f0200|0x067b4000|0x00000001|0xf81e83a1
0x58000050|0xd61f0200|0x09ad4000|0x00000001|0xf81f03a1

#include <libkern/OSByteOrder.h>

- (NSString *)check {
    uint64_t addr = (uint64_t)method_getImplementation(class_getInstanceMethod(objc_getClass("ViewController"), NSSelectorFromString(@"viewDidLoad")));
    uint32_t * ins = (uint32_t*)(addr);
    uint32_t ins0 = OSSwapInt32(*(ins+0));
    uint32_t ins1 = OSSwapInt32(*(ins+1));
    return [NSString stringWithFormat:@"%p|0x%08x|0x%08x", ins, ins0, ins1];
}
```
