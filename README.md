# CComponents
The set of high-level instruments for C

## Built-in regex specification
CComponents includes regular expressions implmentation for some reasons:
- Windows MinGW doesn't contains <regex.h> header unlike UNIX-systems
- Including PCRE2 or other regex realizations will cause a lack of independence and may affect on library building and usage difficulty
- Using C++ regex makes to change a project compiler and incompatibility with only-C-oriented compilers

**A Specification can be changed by any time in the version context**, but we trying to reach the POSIX standard. Nonetheless, a current state of this doesn't match the POSIX.

For now, the regex doesn't support subexpressions and other extended stuff. Probably it means a current realization is something like "Regex-lite version". The list of supported things:

| Feature |   Status   |
|:-------:| ---------- |
|  [...]  | Half-works |
| [^...]  |  In plans  |
|    *    |   Works    |
|    +    |   Works    |
|    ?    |   Works    |
|   \s    |   Works    |
|   \w    |   Works    |
|   \d    |   Works    |
|    .    |   Works    |