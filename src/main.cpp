#include <iostream>
#include <unordered_map>
#include <string_view>

#include "cdiff.h"


int main(int argc, char** argv) {

    Diff diff{"Hello World", "World\nHello"};
    std::string d = diff.line_diff();

    std::cout << d << "\n";


    return 0;
}
