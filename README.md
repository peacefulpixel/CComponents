# CComponents
The set of high-level instruments for C

## Built-in regex specification
The CComponents library includes a lightweight regular expressions realization for simple cases you need it. A regex required by some methods like `StringClassType.match` and `StringClassType.replace` e.g.
If it don't satisfies yours requirements, you just can to replace `src/util/regex.c` file content to what you want. Custom regex realization was written because of some reasons, a few of that described below:
- Windows MinGW doesn't contains `<regex.h>` header unlike UNIX-systems
- Including PCRE2 or other regex realizations will affect on library independence
- Using C++ regex makes to change a project compiler and incompatibility with only-C-oriented compilers

Here a list that describes some things that this custom regex supports:

|  Pattern  | Description                                                                        |
|:---------:| ---------------------------------------------------------------------------------- |
| `$ and ^` | Means a begin (^) and end ($) of line                                              |
|  `[...]`  | A symbol sequence what recognizes as one unit                                      |
|    `*`    | Indicates **any** amount of unit that described before                             |
|    `+`    | Indicates **one or more** unit amount                                              |
|    `?`    | Indicates **one or zero** unit amount                                              |
|   `\s`    | Space characters: `\r\n\t\f\v `                                                    |
|   `\w`    | Word characters: `abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_` |
|   `\d`    | Digit characters: `0123456789`                                                     |
|   `\S`    | **Will available after 0.0.2 release** Non-space characters                        |
|   `\W`    | **Will available after 0.0.2 release** Non-word characters                         |
|   `\D`    | **Will available after 0.0.2 release** Non-digit characters                        |
|    `.`    | Means any character                                                                |
|   `()`    | **Will available after 0.0.2 release** Subexpression as a single unit              |
|  `(\|)`   | **Will available after 0.0.2 release** Logical **or** between subexpressions       |
|  `{0,3}`  | The custom range of unit matches (`{3,}` works too)                                |