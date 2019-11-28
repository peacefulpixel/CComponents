# CComponents
The set of high-level instruments for C

## Built-in regex specification
The CComponents library includes a lightweight regular expressions realization for simple cases you need it. A regex required by some methods like `StringClassType.match` and `StringClassType.replace` e.g.
If it don't satisfies yours requirements, you just can to replace `src/util/regex.c` file content to waht you want. Custom regex realization was written because of some reasons, a few of that described below:
- Windows MinGW doesn't contains `<regex.h>` header unlike UNIX-systems
- Including PCRE2 or other regex realizations will affect on library independence
- Using C++ regex makes to change a project compiler and incompatibility with only-C-oriented compilers

Here a list that describes some things that this custom regex supports:

**TODO: Replace this table columns to | Pattern | Description |**
|  Feature  |   Status   |
|:---------:| ---------- |
| `$ and ^` |  In plans  |
|  `[...]`  |   Works    |
|    `*`    |   Works    |
|    `+`    |   Works    |
|    `?`    |   Works    |
|   `\s`    |   Works    |
|   `\w`    |   Works    |
|   `\d`    |   Works    |
|   `\S`    |  In plans  |
|   `\W`    |  In plans  |
|   `\D`    |  In plans  |
|    `.`    |   Works    |
|   `()`    |  In plans  |
|  `(\|)`   |  In plans  |
|  `{0,3}`  |  In plans  |