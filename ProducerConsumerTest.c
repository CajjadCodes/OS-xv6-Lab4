#include "types.h"
#include "user.h"

#define BUFFER_SIZE 5
#define REPEAT_NUM 20

#define EMPTY_SPACE_SEMAPHORE 0
#define AVAILABLE_DATA_SEMAPHORE 1
#define MUTEX_SEMAPHORE 2
#define STDOUT_SEMAPHORE 3

void producer(void) {
    semaphore_aquire(STDOUT_SEMAPHORE);
    printf(1, "Producer started..\n");
    semaphore_release(STDOUT_SEMAPHORE);
    int data;
    int in = 0;
    for (int i = 0; i < REPEAT_NUM; i++) {
        
        data = (i % 20) + 0x41; // producing some alphabets

        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "[%d]Producer wants to put '%c' in buffer[%d]\n", i, data, in);
        semaphore_release(STDOUT_SEMAPHORE);

        semaphore_aquire(EMPTY_SPACE_SEMAPHORE);
        semaphore_aquire(MUTEX_SEMAPHORE);


        set_buffer(in, data);  // buffer[counter] = data;
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Producer WROTE '%c' in buffer[%d]\n", data, in);
        semaphore_release(STDOUT_SEMAPHORE);
        in = (in + 1) % BUFFER_SIZE; // counter = (counter + 1) % BUFFER_SIZE;


        semaphore_release(MUTEX_SEMAPHORE);
        semaphore_release(AVAILABLE_DATA_SEMAPHORE);

    }

    semaphore_aquire(STDOUT_SEMAPHORE);
    printf(1, "Producer died\n");
    semaphore_release(STDOUT_SEMAPHORE);
    exit();
}

void consumer(void) {
    semaphore_aquire(STDOUT_SEMAPHORE);
    printf(1, "Consumer started..\n");
    semaphore_release(STDOUT_SEMAPHORE);
    char data = 0;
    int out = 0;
    for (int i = 0; i < REPEAT_NUM; i++) {
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "[%d]Consumer wants to read new data\n", i);
        semaphore_release(STDOUT_SEMAPHORE);

        semaphore_aquire(AVAILABLE_DATA_SEMAPHORE);
        semaphore_aquire(MUTEX_SEMAPHORE);
 

        data = get_buffer(out); // data = buffer[counter];
        out = (out + 1) % BUFFER_SIZE;
        semaphore_aquire(STDOUT_SEMAPHORE);
        printf(1, "Consumer READ data '%c' from buffer[%d]\n", data, out);
        semaphore_release(STDOUT_SEMAPHORE);


        semaphore_release(MUTEX_SEMAPHORE);
        semaphore_release(EMPTY_SPACE_SEMAPHORE);

    }

    semaphore_aquire(STDOUT_SEMAPHORE);
    printf(1, "Consumer died\n");
    semaphore_release(STDOUT_SEMAPHORE);
    exit();
}

int main(int argc, char* argv[]) {

    semaphore_initialize(AVAILABLE_DATA_SEMAPHORE, 1, 1);
    semaphore_initialize(EMPTY_SPACE_SEMAPHORE, BUFFER_SIZE, 0);
    semaphore_initialize(MUTEX_SEMAPHORE, 1, 0);
    semaphore_initialize(STDOUT_SEMAPHORE, 1, 0);

    printf(1, "\nAll semaphores are intialized\n");

    if (fork() == 0) { // producer child
        consumer();
    }
    else {
        if (fork() == 0) { // consumer child
            producer();
        }
        else { // tester parent
            wait();
            wait();
        }
    }
    exit();
}