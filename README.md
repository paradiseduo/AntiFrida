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

