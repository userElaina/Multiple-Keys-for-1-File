#include "mp1f.hpp"

int main() {
    int n = 10;
    Keys* p = new Keys(n);
    p->read("rd.key");
    p->release();

    Keys* p2 = new Keys(n);
    for (int i = 0; i < n; i++) {
        char s[512];
        sprintf(s, "%d.key", i);
        p->save_key(i,s);
        printf("%d: %d \n", i, p2->read(s));
        p->pt(i);
        p2->pt(i);
    }

    return 0;
}
