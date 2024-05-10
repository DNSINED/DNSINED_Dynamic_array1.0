#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <chrono>
#include <filesystem>
#include "Boyer_Moore.h"

using namespace std;
namespace fs = filesystem;

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
    arr->size = 0;
    arr->capacity = 0;
}

struct Dynamic_Array_Int {
    int *array;
    int size;
    int capacity;
};

void init_Int(struct Dynamic_Array_Int *arr_int, int init_capacity) {
    arr_int->array = (int *)malloc(init_capacity * sizeof(int));
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
    arr_int->size = 0;
    arr_int->capacity = 0;
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
    auto start_1 = chrono::high_resolution_clock::now();
    while (file.get(ch)) {
        add(&myArray, ch);
    }
    auto end_1 = chrono::high_resolution_clock::now();
    auto duration_1 = chrono::duration_cast<chrono::milliseconds>(end_1 - start_1).count();
    cout << "Load words from file execution time: " << duration_1 << " ms" << endl;

    file.close();

    auto start_0 = chrono::high_resolution_clock::now();
    search_word(&myArray, &myArray_Int, word);
    auto end_0 = chrono::high_resolution_clock::now();
    auto duration_0 = chrono::duration_cast<chrono::milliseconds>(end_0 - start_0).count();
    cout << "Search_Word execution time: " << duration_0 << " ms" << endl;

    int lines_found = myArray_Int.size;

    if (lines_found == correct_lines) {
        test_passed = true;
    }

    clear_Int(&myArray_Int);
    clear(&myArray);
    return test_passed;
}
void search_word_boyer_moore(uint8_t *text, size_t text_len, Dynamic_Array_Int *lines, uint8_t *word) {
    vector<size_t> occurrences = boyer_moore_all(text, text_len, reinterpret_cast<char*>(word), strlen(reinterpret_cast<char*>(word)));
    for (size_t i = 0; i < occurrences.size(); ++i) {
        int line_number = 1;
        for (size_t j = 0; j < occurrences[i]; ++j) {
            if (text[j] == '\n') {
                line_number++;
            }
        }
        add_Int(lines, line_number);
    }
}

bool run_test_boyer_moore(const string& file_path, const char *word, int correct_lines) {
    bool test_passed = false;
    struct Dynamic_Array_Int myArray_Int{};

    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return test_passed;
    }
    streampos file_size = file.tellg();
    file.seekg(0, ios::beg);
    auto* file_content = new uint8_t[file_size];
    file.read(reinterpret_cast<char*>(file_content), file_size);
    file.close();

    init_Int(&myArray_Int, 10);

    auto start_0 = chrono::high_resolution_clock::now();
    search_word_boyer_moore(file_content, file_size, &myArray_Int, reinterpret_cast<uint8_t*>(const_cast<char*>(word)));
    auto end_0 = chrono::high_resolution_clock::now();
    auto duration_0 = chrono::duration_cast<chrono::milliseconds>(end_0 - start_0).count();
    cout << "Search_Word with Boyer-Moore execution time: " << duration_0 << " ms" << endl;

    int lines_found = myArray_Int.size;

    if (lines_found == correct_lines){
        test_passed = true;
    }

    clear_Int(&myArray_Int);
    delete[] file_content;
    return test_passed;
}


int main() {
    cout << endl;
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

    auto end_0 = chrono::high_resolution_clock::now();
    auto duration_0 = chrono::duration_cast<chrono::milliseconds>(end_0 - start).count();

    uintmax_t filesize = fs::file_size(file_path);
    double megabytes = static_cast<double>(filesize) / (1024 * 1024);

    cout << "File: " << file_path << "        Size: " << megabytes << " MB"<< "        Execution time: " << duration_0 << " ms" << endl;
    cout << endl;

    test_passed = run_test_boyer_moore(file_path, word_1, correct_lines_1);
    if (test_passed) {
        cout << "Test with Boyer-Moore passed successfully" << endl;
    }
    else {
         cout << "Test with Boyer-Moore failed." << endl;
    }

    auto end_1 = chrono::high_resolution_clock::now();
    auto duration_1 = chrono::duration_cast<chrono::milliseconds>(end_1 - start).count();

    cout << "File: " << file_path << "        Size: " << megabytes << " MB"<< "        Execution time: " << duration_1 - duration_0 << " ms" << endl;


    uint8_t string[] = "this is a test, to test something i want to test";
    uint8_t pat[] = "test";
    vector<size_t> results = boyer_moore_all(string, strlen(reinterpret_cast<char*>(string)), reinterpret_cast<char *>(pat), strlen(reinterpret_cast<char*>(pat)));

    if (!results.empty()) {
        cout << "Pattern found at index: ";
        for (size_t i = 0; i < results.size(); ++i) {
            cout << results[i] << " ";
        }
        cout << endl;
    } else {
        cout << "Pattern not found" << endl;
    }

    return 0;
}
