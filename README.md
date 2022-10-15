# cdiff

This repository contains my (initial) implementation for comparing two strings (diff operation).

## Usage

### Character diff

If the diff between two character strings is required, the `verbose_diff` method can be used.
This method outputs a diff on a per-character basis. Note that for comparing long string, this
method is not efficient.

```c++
#include <iostream>
#include "cdiff.h"

int main(int argc, char** argv) {

    Diff diff{"GO", "GOGOGLE"};
    std::string d = diff.verbose_diff();
    std::cout << d << "\n"; // +G+OGO+G+L+E

    return 0;
}
```

### Line diff

For a much more efficient variant of `diff` that operates on a per line basis, `line_diff`
should be used. This method computes for every line (character sequences separated by `\n`)
a hash for which then the longest common subsequence is calculated.

Note, that due to the nature of hash functions, a collision can occur. In this case, the
diff is incorrect.

```c++
#include <iostream>
#include "cdiff.h"

int main(int argc, char** argv) {
    
    Diff diff{"Hello World", "World\nHello"};
    std::string d = diff.line_diff();
    std::cout << d << "\n";
    
    // + World
    // + Hello
    // - Hello World
    
    return 0;
}
```

## Progress

This project was just created and misses some important functionality:

- **Speed**: the current implementation uses a very simple dynamic programming approach which requires
`O(n*m)` time
    - Improvement: Implementation of better algorithm (e.g. Myers-Miller)
    
- **Patching**: Using the generated patches to retrieve the new version of the string
- **Standards**: Support different patch standards