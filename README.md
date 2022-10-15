# cdiff

This repository contains my (initial) implementation for comparing two strings (diff operation).

## Usage

```c++
#include <iostream>

#include "cdiff.h"

int main(int argc, char** argv) {

    Diff diff{"GO", "GOGOGLE"};
    std::string patch = diff.verbose_patch();

    std::cout << patch << "\n"; // +G+OGO+G+L+E

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