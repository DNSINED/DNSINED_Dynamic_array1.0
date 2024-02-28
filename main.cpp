#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

struct Dynamic_Array {
    char *array;
    int size;
    int capacity;
};

void init(struct Dynamic_Array *arr, int init_capacity) {
    arr->array = (char *)malloc(init_capacity * sizeof(char));
    arr->size = 0;
    arr->capacity = init_capacity;
}

void resize(struct Dynamic_Array *arr) {
    arr->capacity *= 2;
    arr->array = (char *)realloc(arr->array, arr->capacity * sizeof(char));
}

void add(struct Dynamic_Array *arr, char val) {
    if (arr->size == arr->capacity) {
        resize(arr);
    }
    arr->array[arr->size++] = val;
}

void clear(struct Dynamic_Array *arr) {
    free(arr->array);
}

bool search_word(struct Dynamic_Array *arr, const char *word, int correct_lines) {
    bool found = false;
    int line = 1;
    int count_lines = 0;//for test
    for (int i = 0; i < arr->size; ++i) {
        if (arr->array[i] == '\n') {
            line++;
        } else {
            int j = 0;
            int start = i;
            while (tolower(arr->array[i + j]) == tolower(word[j]) && word[j] != '\0') {
                j++;
            }
            if (word[j] == '\0') {
                //cout << "The word '" << word << "' was found in line " << line << "." << endl;
                found = true;
                count_lines ++;//for test
            }
            if (count_lines != correct_lines) {//for test
                found = false;
            }
        }
    }
    return found;
}

bool run_test(const string& file_path, const char *word, int correct_lines) {
    bool test_passed = false;
    struct Dynamic_Array myArray{};
    //ifstream file(R"(C:\Users\savan\CLionProjects\untitled1\fisier.txt)");
    ifstream file(file_path);

    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return test_passed;
    }

    init(&myArray, 10);

    char ch;
    while (file.get(ch)) {
        add(&myArray, ch);
    }

    file.close();

    int lines = correct_lines;//for test
    //bool wordFound = search_word(&myArray, word);
    bool wordFound = search_word(&myArray, (const char *) word, lines);

    if (!wordFound) {
        //cout << "The searched word does not exist in the file." << endl;
        return test_passed;
    }
    else {
        test_passed = true;
    }
    clear(&myArray);

    return test_passed;
}



int main() {
    /*
    struct Dynamic_Array myArray{};
    ifstream file(R"(C:\Users\savan\CLionProjects\untitled1\fisier.txt)");

    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    init(&myArray, 10);

    char ch;
    while (file.get(ch)) {
        add(&myArray, ch);
    }

    file.close();

    char word[100];
    cout << "Enter a word to search: ";
    cin >> word;

    bool wordFound = search_word(&myArray, word);

    if (!wordFound) {
        cout << "The searched word does not exist in the file." << endl;
    }

    clear(&myArray);

    return 0;*/
    string file_path = R"(C:\Users\savan\CLionProjects\untitled1\fisier.txt)";
    string word_0 = "how";
    int correct_lines_0 = 2453;
    const char *word_1 = "holmes";
    int correct_lines_1 = 467;
    string word_2 = "gentleman";
    int correct_lines_2 = 100;
    bool test_passed = false;
    test_passed = run_test(file_path, word_1, correct_lines_1);
    if (test_passed) {
        cout << "Test passed succesfully" << endl;
    }
    else {
        cout << "Test failed." << endl;
    }
    return 0;
}
