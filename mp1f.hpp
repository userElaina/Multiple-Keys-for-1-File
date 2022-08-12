#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <vector>

#include "basen.h"

#define LL long long
#define ULL unsigned long long
#define B unsigned char

const int MAX_N = 255;
int flg_srand_time = 0;
const int BUFFER_SIZE = 1024;

inline void throws(const char *s) {
    throw std::runtime_error(std::string(s));
}

inline int size_of_file(const char *pth) {
    FILE *fp = fopen(pth, "rb");
    if (!fp) {
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fclose(fp);
    return size;
}

class Reader {
private:
    FILE *fp;
    B *buffer;
    ULL t = 0, size;
    int t1 = 0, t2 = 0;

public:
    int flg_rand = 1, flg_head = 1;
    Reader(const char *pth, int flg_head);
    inline int read_bit();
    inline ULL read_bits(int x);
    inline void release();
};

Reader::Reader(const char *pth, int flg_head = 1) {
    buffer = (B *)malloc(BUFFER_SIZE);
    if (!flg_srand_time) {
        srand(time(NULL));
        flg_srand_time = 1;
    }
    size = size_of_file(pth);
    flg_rand = 0;
    fp = fopen(pth, "rb");
    if (!fp) {
        throws("FileNotExist");
    }
    if (flg_head) {
        this->flg_head = 1;
        ULL a = size;
        for (int i = 7; i >= 0; i--) {
            buffer[i] = (a & 255) ^ (buffer[i + 8] = rand() & 255) ^ (buffer[i + 16] = rand() & 255);
            a >>= 8;
        }
    } else {
        this->flg_head = 0;
        t1 = 0;
        fread(buffer, 1, BUFFER_SIZE, fp);
    }
}

inline int Reader::read_bit() {
    if (flg_rand) {
        return rand() & 1;
    }
    int ans = (buffer[t1] >> (7 - t2)) & 1;
    t2++;
    if (t2 == 8) {
        t2 = 0;
        t1++;
        if (flg_head) {
            if (t1 >= 24) {
                flg_head = 0;
                t1 = 0;
                fread(buffer, 1, BUFFER_SIZE, fp);
            }
        } else {
            t++;
            if (t >= size) {
                fclose(fp);
                flg_rand = 1;
            }
            if (t1 == BUFFER_SIZE) {
                t1 = 0;
                fread(buffer, 1, BUFFER_SIZE, fp);
            }
        }
    }
    return ans;
}

inline ULL Reader::read_bits(int x) {
    if (x > 64) {
        throws("Reader.read_bits: x out of range");
    }
    ULL ans = 0;
    for (int i = 0; i < x; i++) {
        ans = ans << 1 | read_bit();
    }
    return ans;
}

inline void Reader::release() {
    fclose(fp);
    t = t1 = t2 = 0;
}

class Writer {
private:
    FILE *fp;
    ULL t = 0;
    int t1 = 0, t2 = 0;
    B *buffer;

public:
    Writer(const char *pth);
    inline void write_bit(int b);
    inline void flush();
    inline void release();
};

Writer::Writer(const char *pth) {
    buffer = (B *)malloc(BUFFER_SIZE);
    memset(buffer, 0, BUFFER_SIZE);
    fp = fopen(pth, "wb");
    if (!fp) {
        throws("CannotOpenFile");
    }
}

inline void Writer::write_bit(int b) {
    buffer[t1] = buffer[t1] << 1 | b;
    t2++;
    if (t2 == 8) {
        t2 = 0;
        t1++;
        t++;
        if (t1 == BUFFER_SIZE) {
            fwrite(buffer, 1, BUFFER_SIZE, fp);
            memset(buffer, 0, BUFFER_SIZE);
            t1 = 0;
        }
    }
}

inline void Writer::flush() {
    fwrite(buffer, 1, t1 + (t2 ? 1 : 0), fp);
}

inline void Writer::release() {
    flush();
    fclose(fp);
    t = t1 = t2 = 0;
}

class Keys {
private:
    int n, num = 0;
    int blocks = 0;
    int n_0, n_1;
    B *map, *inverse_map;
    int ap = 0;
    inline int convert(B *s, int size);

public:
    Keys(int n, int loop_size);
    inline int add(B *k);
    inline int add(const char *k);
    inline int read(const char *pth);
    inline int random();
    inline int release();
    inline void restart();

    inline int set_blocks(int blocks);
    inline int set_loop_size(int loop_size);
    inline int set_key_size(int key_size);

    inline B get_map(int x);
    inline B *get_inverse_map();
    inline B *get_key(int x) const;
    inline int save_key(int x, const char *pth) const;
    inline void pt(int x) const;
    inline int get_n() const;
    inline int get_num() const;
    inline int get_blocks() const;
    inline int get_loop_size() const;
    inline int get_key_size() const;
};

inline int Keys::convert(B *s, int size) {
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
Keys::Keys(int n, int loop_size = 0) {
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

inline int Keys::add(B *k) {
    return convert(k, strlen((char *)k));
}
inline int Keys::add(const char *k) {
    return add((B *)k);
}

inline int Keys::read(const char *pth) {
    int size = size_of_file(pth);
    if (size <= 0) {
        return -1;
    }
    FILE *fp = fopen(pth, "rb");
    B *s = (B *)malloc(size);
    fread(s, 1, size, fp);
    return convert(s, size);
}

inline int Keys::random() {
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
            throws("Keys.random: x out of range");
        }
        inverse_map[j + x] = num;
        map[j + num] = x;
    }

    num++;
    return 0;
}

inline int Keys::release() {
    while (num < n) {
        int z = random();
        if (z) {
            return z;
        }
    }
    return 0;
}

inline void Keys::restart() {
    ap = 0;
}

inline int Keys::set_blocks(int blocks) {
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
inline int Keys::set_loop_size(int loop_size) {
    return set_blocks((loop_size - 1) / n_0 + 1);
}
inline int Keys::set_key_size(int key_size) {
    return set_blocks((key_size - 1) / n_1 + 1);
}

inline B Keys::get_map(int x = 0) {
    B ans = *(map + ap * n + x);
    ap++;
    if (ap >= get_loop_size()) {
        ap = 0;
    }
    return ans;
}

inline B *Keys::get_inverse_map() {
    B *ans = (B *)malloc(n);
    std::memcpy(ans, inverse_map + ap * n, n);
    ap++;
    if (ap >= get_loop_size()) {
        ap = 0;
    }
    return ans;
}

inline void Keys::pt(int x) const {
    for (int i = 0; i < get_loop_size() * n; i += n) {
        printf("%x", map[i + x]);
    }
    printf("\n");
}

inline B *Keys::get_key(int x) const {
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

inline int Keys::save_key(int x, const char *pth) const {
    FILE *fp = fopen(pth, "wb");
    fwrite(get_key(x), 1, get_key_size(), fp);
    fclose(fp);
    return 0;
}

inline int Keys::get_n() const {
    return n;
}
inline int Keys::get_num() const {
    return num;
}
inline int Keys::get_blocks() const {
    return blocks;
}
inline int Keys::get_loop_size() const {
    return blocks * n_0;
}
inline int Keys::get_key_size() const {
    return blocks * n_1;
}
