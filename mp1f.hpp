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
int flg_srand_time = 0;

class Keys {
private:
    int n, num = 0;
    int blocks = 0;
    int n_0, n_1;
    B *map, *inverse_map;

    inline int convert(B *s, int size) {
        if (!num && blocks <= 0) {
            set_key_size(size);
        } else {
            if (size + n_1 <= get_key_size()) {
                return -1;
            }
        }
        int st = 0;
        for (int i = 0; i < get_loop_size() * n; i += n_0 * n) {
            ULL reg = 0;
            for (int j = 0; j < n_1; j++) {
                reg <<= 8;
                if (st < size) {
                    reg |= s[st++];
                }
            }
            for (int j = i + (n_0 - 1) * n; j >= i; j -= n) {
                const int x = reg % n;
                if (inverse_map[j + x] < num) {
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
        if (!flg_srand_time) {
            srand(time(NULL));
            flg_srand_time = 1;
        }
    }

    inline int set_blocks(int blocks) {
        if (num) {
            return -1;
        }
        if (blocks <= 0) {
            return blocks;
        }
        const int map_size = blocks * n_0 * n;
        if (this->blocks) {
            realloc(map, map_size);
            realloc(inverse_map, map_size);
        } else {
            map = (B *)malloc(map_size);
            inverse_map = (B *)malloc(map_size);
        }
        memset(inverse_map, -1, map_size);
        return this->blocks = blocks;
    }
    inline int set_loop_size(int loop_size) {
        return set_blocks((loop_size - 1) / n_0 + 1);
    }
    inline int set_key_size(int key_size) {
        return set_blocks((key_size - 1) / n_1 + 1);
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
        if (blocks <= 0) {
            return -1;
        }
        const int range = n - num;

        for (int j = 0; j < get_loop_size() * n; j += n) {
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

    inline void restart() {
        ap = 0;
    }

    // inline int get_next() {
    //     if (ap >= loop_size) {
    //         ap = 0;
    //     }
    //     return map[ap++];
    // }

    inline void pt(int x) const {
        for (int i = 0; i < get_loop_size() * n; i += n) {
            printf("%x", map[i + x]);
        }
        printf("\n");
    }

    inline B *get_key(int x) const {
        B *ans = (B *)malloc(get_key_size());
        int st = 0;
        for (int i = 0; i < get_loop_size() * n; i += n_0 * n) {
            ULL reg = 0;
            for (int j = i; j < i + n_0 * n; j += n) {
                reg *= n;
                reg += map[j + x];
            }
            for (int j = n_1 - 1 + st; j >= st; j--) {
                ans[j] = reg & (ULL)255;
                reg >>= 8;
            }
            st += n_1;
        }
        return ans;
    }

    inline int save_key(int x, const char *fname) const {
        FILE *fp = fopen(fname, "wb");
        fwrite(get_key(x), 1, get_key_size(), fp);
        fclose(fp);
        return 0;
    }
    inline int save_key(int x, std::string fname) const {
        return save_key(x, fname.c_str());
    }

    inline int get_n() const {
        return n;
    }
    inline int get_num() const {
        return num;
    }
    inline int get_blocks() const {
        return blocks;
    }
    inline int get_loop_size() const {
        return blocks * n_0;
    }
    inline int get_key_size() const {
        return blocks * n_1;
    }
};
