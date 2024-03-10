const japaneseText = JSON.stringify(require('../resources/langs/japanese.json')).replaceAll("\n","") + "日本語"

const letters = japaneseText.split("").map((a) => {
    if (a.codePointAt(0) < 0x3000 || a.codePointAt(0) > 0x9faf) return 0x0
    return a.codePointAt(0)
}).filter(x => x != 0).sort((a, b) => a - b)

console.log([...new Set(letters)])

require('fs').writeFileSync("./kata.txt", JSON.stringify([...new Set(letters)]), 'utf8');

