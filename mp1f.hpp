#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <vector>

#include "basen.h"

// #define LL long long
#define ULL unsigned long long
#define B unsigned char

inline void throws(const char *s) {
    throw std::runtime_error(std::string(s));
}

const int MAX_N = 255;

class Keys {
private:
    int n, num = 0, loop_size = 0;
    int n_0, n_1;
    B *map, *inverse_map;

    inline int convert(B *s, int size) {
        const int quotient = size / n_1, remainder = size % n_1;
        const int ls2 = (quotient + (remainder ? 1 : 0)) * n_0;

        // printf("[%d/%d %d/%d=%d...%d]\n", ls2, n_0, size, n_1, quotient, remainder);

        if (!num && loop_size <= 0) {
            set_loop_size(ls2);
        } else {
            if (ls2 < loop_size) {
                return -1;
            }
        }
        int st = 0, mt = 0;
        for (int i = 0; i < quotient; i++) {
            ULL reg = 0;
            for (int j = 0; j < n_1; j++) {
                reg = reg << 8 | (ULL)s[st++];
            }
            for (int j = mt + (n_0 - 1) * n; j >= mt; j -= n) {
                const int x = reg % n;
                if (inverse_map[x] < num) {
                    return 1;
                }
                inverse_map[j + x] = num;
                map[j + num] = x;
                reg /= n;
            }
            mt += n_0 * n;
        }
        if (remainder) {
            ULL reg = 0;
            int j = 0;
            for (; j < remainder; j++) {
                reg = reg << 8 | (ULL)s[st++];
            }
            for (; j < n_1; j++) {
                reg <<= 8;
            }
            for (int j = mt + (n_0 - 1) * n; j >= mt; j -= n) {
                const int x = reg % n;
                if (inverse_map[x] < num) {
                    return 1;
                }
                inverse_map[j + x] = num;
                map[j + num] = x;
                reg /= n;
            }
        }
        num++;
        return 0;
    }

public:
    int ap = 0;

    Keys(int n, int loop_size = 0) {
        this->n = n;
        n_0 = basen[n][0];
        n_1 = basen[n][1];
        if (loop_size > 0) {
            set_loop_size(loop_size);
        }
    }
    inline int set_loop_size(int loop_size) {
        if (num) {
            return -1;
        }
        if (loop_size <= 0) {
            return loop_size;
        }
        const int map_size = loop_size * n;
        if (this->loop_size) {
            realloc(map, map_size);
            realloc(inverse_map, map_size);
        } else {
            map = (B *)malloc(map_size);
            inverse_map = (B *)malloc(map_size);
        }
        memset(inverse_map, -1, map_size);
        return this->loop_size = loop_size;
    }

    inline int add(B *k) {
        return convert(k, strlen((char *)k));
    }
    inline int add(const char *k) {
        return add((B *)k);
    }
    inline int add(std::string k) {
        return add(k.c_str());
    }

    inline int read(const char *fname) {
        FILE *fp = fopen(fname, "rb");
        if (!fp) {
            return -1;
        }
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);
        rewind(fp);
        if (!size) {
            return -1;
        }
        B *s = (B *)malloc(size);
        fread(s, 1, size, fp);
        return convert(s, size);
    }
    inline int read(std::string fname) {
        return read(fname.c_str());
    }

    inline int random() {
        if (loop_size <= 0) {
            return -1;
        }
        srand(time(NULL));
        const int range = n - num;

        for (int j = 0; j < loop_size * n; j += n) {
            int x = 0, y = range * rand() / (RAND_MAX + 1);
            for (; x < n; x++) {
                if (inverse_map[j + x] >= num) {
                    if (!y--) {
                        break;
                    }
                }
            }
            if (x >= n) {
                throws("Keys.random: x >= n");
            }
            inverse_map[j + x] = num;
            map[j + num] = x;
        }

        num++;
        return 0;
    }

    inline int release() {
        while (num < n) {
            int z = random();
            if (z) {
                return z;
            }
        }
        return 0;
    }

    inline void pt(int x) const {
        for (int i = 0; i < loop_size; i++) {
            printf("%d", map[i * n + x]);
        }
        printf("\n");
    }

    inline void restart() {
        ap = 0;
    }

    // inline int get_next() {
    //     if (ap >= loop_size) {
    //         ap = 0;
    //     }
    //     return map[ap++];
    // }

    inline int get_length() const {
        return loop_size;
    }
};
