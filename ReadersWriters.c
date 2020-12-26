#include "types.h"
#include "user.h"
#include "uspinlock.h"
#include "condvar.h"

void reader(void) {
    start_reading();
    exit();
}

void writer(void) {
    start_writing();
    exit();
}

int main() {
    // 5 child -> 2 Writer, 3 Reader
    init_readers_writers();

    if (fork() > 0) {
        if (fork() > 0) {
            if (fork() > 0) {
                if (fork() > 0) {
                    if (fork() > 0) { // parent
                        for (int i = 0; i < 5; i++)
                            wait();
                        exit();
                    }
                    else {
                        reader();
                    }
                }
                else {
                    writer();
                }
            }
            else {
                reader();
            }
        }
        else {
            reader();
        }
    }
    else {
        writer();
    }
}