# Hacks
In this directory contains all hacks and modules for Prism Menu.
It is not advised to change any of this, as it may cause your game to crash if you do not know what you are doing.

# Hack Types
* `bool` - A boolean value, usually for toggles. Defaults to false. (Checkbox)
* `int` - An integer value. (Increment and Decrement buttons)
* `float` - A float value, usually used for values that require decimals. (Slider)
* `dropdown` - An integer in disguise. Same thing as "int" but you can assume what it does.
* `button` - A hack with no saving involved, since why would you need to. Anyways, this is usually used for functionality.

# Format (Do not include //)
```js
[
    {
        "name": "Hack Name", // Name of the Hack
        "desc": "Hack Description", // Description for the hack (Will show on the info button, or when hovering)
        "opcodes": { // Bytes to patch. Similar style to Geode's "version" key, except expanded for android32 and android64. This will only work on "bool" types.
            "win": [{"addr": "0xA5424", "cocos": true, "on": "90 90 90 90 90", "off": "F3 0F 10 45 08"}], // cocos is usually reserved for Windows as libcocos2d.dll isn't statically linked. thanks robert!
            "android32": [
                {"addr": "0x20A3B2", "on": "E9 9A 01 00 00 90 90", "off": "80 BB 94 04 00 00 00"}, // this isnt arm code! this is x86! make sure to do arm bytes
                {"addr": "0x1FD40F", "on": "E9 13 06 00 00 90 90", "off": "83 B9 64 03 00 00 01"}
            ],
            "android64": [{}], // same format
            "imac": [{}], // same format
            "m1": [{}], // same format
            "ios": [{}] // same format
        },
        "platforms": ["win", "android32", "android64"], // Only include this key if the hack isn't implemented on a specific platform. Any platform not specified will be assumed not supported, and hidden.
        "type": "dropdown", // Type of hack. Refer to the "Hack Types" for more info.
        "default": 0, // Default value. Make sure this is accurate with the hack type.
        "save": false, // Only used if the hack shouldn't be saved when the game closes. (Hacks like speedhack for example)
        "values": ["Value 1", "Value 2", "etc"] // Exclusive to dropdowns. Do not include this key if the type isn't a "dropdown".
    }
]
``` 
Note that creating patches will disable saving the setting, meaning the user would have to enable it again. This is because Prism Menu **currently does not** have a check to see if hacks are enabled when Geometry Dash is launched, and to apply the patches.

# Example 
```js
[
    {
        "name": "Backup Stars Limit",
        "desc": "Lets you backup data, even with less than 10 stars.",
        "opcodes": {
            "win": [{"addr": "0x57c8e", "on": "EB 52", "off": "7D 52"}], // JMP short +0x52
            "android32": [{"addr": "0x448116", "on": "16 D0", "off": "16 DC"}], // BEQ (why is base 0x10000)
            "android64": [{"addr": "0x8181f0", "on": "00 05 00 54", "off": "0C 05 00 54"}], // B.EQ
            "imac": [{"addr": "0xc8d09", "on": "75 4D", "off": "7F 4D"}], // JNZ
            // m1 isnt included but it's supported in the "platforms" key. If m1 not included, it will assume that functionality still exists through hooking instead of patching.
            "ios": [{"addr": "0x16cee4", "on": "20 03 00 54", "off": "2C 03 00 54"}] // B.EQ
        },
        // no "platforms" key, since we assume it's supported for all platforms
        "type": "bool",
        "save": false // Since it is a "patch" (and patch saving isnt implemented), we will assume this wont be enabled on startup.
    }
]
```
# References (ignore)
- https://en.wikipedia.org/wiki/X86_instruction_listings
- https://en.wikipedia.org/wiki/Branch_(computer_science)
- https://developer.arm.com/documentation/ddi0222/b/introduction/arm9ej-s-instruction-set-summary/thumb-instruction-set-summary
- ghidra!
