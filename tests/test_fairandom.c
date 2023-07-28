#include <fairandom/fairandom.h>

int main(void) {
    int result = fr_add(1, 2);

    if (result == 3) {
        return 0;
    }
    return 1;
}