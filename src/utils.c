#include <unistd.h>

#include "utils.h"

void UtilsDelay(unsigned long ulCount) {
    usleep(ulCount / 10);
}