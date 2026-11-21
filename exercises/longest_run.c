#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    size_t length;
} Run;

Run longest_run(const int *arr, size_t size){
    Run best_run = {0, 0};
    /* Validiity checks */
    if (size == 0)
        return best_run;

    int current = arr[0];
    size_t count = 1;
    best_run.value = current;
    best_run.length = 1;

    /*Taverse the list of integers */
    for (size_t i = 1; i < size; i++){
        if (current == arr[i]) {
            count++;
        }
        else {
            if (count > best_run.length) {
                best_run.value = current;
                best_run.length = count;
            }
            current = arr[i];
            count = 1;
        }
    }

    if (count > best_run.length) {
        best_run.value = current;
        best_run.length = count;
    }

    return best_run;
}

int main() {

    int my_array[] = {3, 3, 3, 5, 5, 5, 7, 7};
    /* Useful C idiom */
    size_t arr_size = sizeof(my_array)/sizeof(my_array[0]);

    Run result = longest_run(my_array, arr_size);
    printf("The longest value is %d\n", result.value);
    printf("The longest run is %zu\n", result.length);

    return EXIT_SUCCESS;
}
