#include "Tests.hpp"
#include "UCIUtils.hpp"

int main() {

    // Do not buffer output or else SearchThread output does not print out instantly
    if (setvbuf(stdout, nullptr, _IONBF, 0) != 0) {
        printf("Failed to set buffer size\n");
    }

    if (setvbuf(stdin, nullptr, _IONBF, 0) != 0) {
        printf("Failed to set buffer size\n");
    }

    UCIUtils::loop();

    return 0;
}