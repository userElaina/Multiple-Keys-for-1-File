#include "mk1f.hpp"

int main() {
    using namespace Mk1f;
    int n = 10;

    char* fs[] = {
        "archive/0.7z",
        "archive/1.7z",
        "archive/2.7z",
        "archive/3.7z",
        "archive/4.7z",
        "archive/5.7z",
        "archive/6.7z",
        "archive/7.7z",
        "archive/8.7z",
        "archive/9.7z"};

    int flg_enc = 1;
    if (flg_enc) {
        Keys* p = new Keys(n);
        p->read("archive/rd.key");
        p->release();
        printf("save keys: %d\n", p->save_keys());
        printf("Keys.encrypt: %d\n", p->encrypt(fs, "archive/kbig.bin"));

        Keys* p2 = p->clone(6);
        printf("Keys.decrypt %d: %d\n", 5, p2->decrypt(5, "archive/kbig.bin", "archive/05.7z"));
    } else {
        printf("encrypt: %d\n", encrypt(n, 64, fs, "archive/big.bin"));
        printf("decrypt %d: %d\n", 8, decrypt("keys/8.key", "archive/big.bin", "archive/08.7z"));
    }

    return 0;
}
