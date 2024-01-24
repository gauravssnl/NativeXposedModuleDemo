//
// Created by gauravssnl.
//

#include <cstring>
#include <string>

FILE *(*original_fopen)(const char *filename, const char *mode);

FILE *fake_fopen(const char *filename, const char *mode);

// String ends with function
bool ends_with(std::string const &a, std::string const &b) {
    auto len = b.length();
    auto pos = a.length() - len;
    auto pos_a = &a[pos];
    auto pos_b = &b[0];
    while (*pos_a)
        if (*pos_a++ != *pos_b++)
            return false;
    return true;
}
