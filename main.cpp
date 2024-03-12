#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <chrono>

using namespace std;

struct Dynamic_Array_Char {
    char *array;
    int size;
    int capacity;
};

void init(struct Dynamic_Array_Char *arr, int init_capacity) {
    arr->array = (char *)malloc(init_capacity * sizeof(char));
    arr->size = 0;
    arr->capacity = init_capacity;
}

void resize(struct Dynamic_Array_Char *arr) {
    arr->capacity *= 2;
    arr->array = (char *)realloc(arr->array, arr->capacity * sizeof(char));
}

void add(struct Dynamic_Array_Char *arr, char val) {
    if (arr->size == arr->capacity) {
        resize(arr);
    }
    arr->array[arr->size++] = val;
}

void clear(struct Dynamic_Array_Char *arr) {
    free(arr->array);
}

struct Dynamic_Array_Int {
    int *array;
    int size;
    int capacity;
};

void init_Int(struct Dynamic_Array_Int *arr_int, int init_capacity) {
    arr_int->array = (int *)malloc(init_capacity * sizeof(double));
    arr_int->size = 0;
    arr_int->capacity = init_capacity;
}

void resize_Int(struct Dynamic_Array_Int *arr_int) {
    arr_int->capacity *= 2;
    arr_int->array = (int *)realloc(arr_int->array, arr_int->capacity * sizeof(int));
}

void add_Int(struct Dynamic_Array_Int *arr_int,  int val) {
    if (arr_int->size == arr_int->capacity) {
        resize_Int(arr_int);
    }
    arr_int->array[arr_int->size++] = val;
}

void clear_Int(struct Dynamic_Array_Int *arr_int) {
    free(arr_int->array);
}
void search_word(Dynamic_Array_Char *arr, Dynamic_Array_Int *lines, const char *word) {
    int line = 1;
    int lines_found = 0;
    for (int i = 0; i < arr->size; ++i) {
        if (arr->array[i] == '\n') {
            line++;
        } else {
            int j = 0;
            while (tolower(arr->array[i + j]) == tolower(word[j]) && word[j] != '\0') {
                j++;
            }
            if (word[j] == '\0') {
                lines_found++;
                add_Int(lines, line);
            }
        }
    }
}

bool run_test(const string& file_path, const char *word, int correct_lines) {
    bool test_passed = false;
    struct Dynamic_Array_Char myArray{};
    struct Dynamic_Array_Int myArray_Int{};
    ifstream file(file_path);

    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return test_passed;
    }
    init_Int(&myArray_Int, 10);
    init(&myArray, 10);

    char ch;
    while (file.get(ch)) {
        add(&myArray, ch);
    }

    file.close();

    search_word(&myArray, &myArray_Int, word);
    int lines_found = 0;
    while (lines_found < myArray_Int.size) {
        ++lines_found;
    }

    if (lines_found == correct_lines) {
        test_passed = true;
    }

    clear_Int(&myArray_Int);
    clear(&myArray);
    return test_passed;
}



int main() {
    auto start = chrono::high_resolution_clock::now();

    string file_path = R"(../fisier.txt)";
    const char *word_0 = "how";
    int correct_lines_0 = 19616;
    const char *word_1 = "holmes";
    int correct_lines_1 = 3736;
    const char * word_2 = "gentleman";
    int correct_lines_2 = 800;
    bool test_passed = false;
    test_passed = run_test(file_path, word_1, correct_lines_1);
    if (test_passed) {
        cout << "Test passed successfully" << endl;
    }
    else {
        cout << "Test failed." << endl;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Execution time: " << duration << " milliseconds" << endl;
    return 0;
}
