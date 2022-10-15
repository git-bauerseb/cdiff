#include <iostream>

#include "cdiff.h"

int main(int argc, char** argv) {

    Diff diff{"GO", "GOGOGLE"};
    std::string patch = diff.verbose_patch();

    std::cout << patch << "\n";

    return 0;
}
