#ifndef BOYER_MOORE_H
#define BOYER_MOORE_H
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>

using namespace std;

constexpr size_t ALPHABET_LEN = 256;

template<typename T>
T max_(T a, T b) {
    return (a < b) ? b : a;
}


void make_delta1(vector<ptrdiff_t>& delta1, const string& pat) {
    size_t patlen = pat.length();
    for (int i = 0; i < ALPHABET_LEN; i++) {
        delta1[i] = patlen;
    }
    for (size_t i = 0; i < patlen; i++) {
        delta1[pat[i]] = patlen - 1 - i;
    }
}

bool is_prefix(const string& word, size_t wordlen, ptrdiff_t pos) {
    size_t suffixlen = wordlen - pos;
    for (size_t i = 0; i < suffixlen; i++) {
        if (word[i] != word[pos + i]) {
            return false;
        }
    }
    return true;
}

size_t suffix_length(const string& word, size_t wordlen, ptrdiff_t pos) {
    size_t i;
    for (i = 0; (word[pos - i] == word[wordlen - 1 - i]) && (i <= pos); i++);
    return i;
}

void make_delta2(vector<ptrdiff_t>& delta2, const string& pat) {
    size_t patlen = pat.length();
    ssize_t p;
    size_t last_prefix_index = 1;

    for (p = patlen - 1; p >= 0; p--) {
        if (is_prefix(pat, patlen, p + 1)) {
            last_prefix_index = p + 1;
        }
        delta2[p] = last_prefix_index + (patlen - 1 - p);
    }

    for (p = 0; p < patlen - 1; p++) {
        size_t slen = suffix_length(pat, patlen, p);
        if (pat[p - slen] != pat[patlen - 1 - slen]) {
            delta2[patlen - 1 - slen] = patlen - 1 - p + slen;
        }
    }
}

uint8_t* boyer_moore(uint8_t* string, size_t stringlen, uint8_t* pat, size_t patlen) {
    vector<ptrdiff_t> delta1(ALPHABET_LEN);
    vector<ptrdiff_t> delta2(patlen);
    make_delta1(delta1, reinterpret_cast<char*>(pat));
    make_delta2(delta2, reinterpret_cast<char*>(pat));

    if (patlen == 0) {
        return string;
    }

    size_t i = patlen - 1;
    while (i < stringlen) {
        ptrdiff_t j = patlen - 1;
        while (j >= 0 && (string[i] == pat[j])) {
            --i;
            --j;
        }
        if (j < 0) {
            return &string[i + 1];
        }

        ptrdiff_t shift = max_(delta1[string[i]], delta2[j]);
        i += shift;
    }
    return nullptr;
}

#endif