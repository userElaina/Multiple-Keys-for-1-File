#include "mp1f.hpp"

int main() {
    int n = 10;
    Keys* p = new Keys(n);
    p->read("rd.key");
    p->release();

    Keys* p2 = new Keys(n);
    Reader** rp = (Reader**)malloc(sizeof(Reader*) * n);

    for (int i = 0; i < n; i++) {
        char s[512];
        sprintf(s, "%d.key", i);
        p->save_key(i, s);
        printf("%d: %d \n", i, p2->read(s));
        // p->pt(i);
        // p2->pt(i);

        rp[i] = new Reader(s);
    }

    Writer* wp = new Writer("all.bin");
    p->restart();
    int flg = 1;
    while (flg) {
        B* imp = p->get_inverse_map();
        for (int i = 0; i < n; i++) {
            int x = rp[imp[i]]->read_bit();
            printf("%d", x);
            if (!imp[i]) printf("|");
            wp->write_bit(x);
        }
        printf("\n");
        // wp->write_bit(rp[0]->read_bit());
        flg = 0;
        for (int i = 0; i < n; i++) {
            if (!rp[i]->flg_rand) {
                flg = 1;
                break;
            }
        }
    }
    wp->release();
    printf("\n");

    p2->restart();
    Reader* r = new Reader("all.bin", 0);
    Writer* w = new Writer("re.bin");

    ULL a = 0;
    for (int i = 0; i < 3; i++) {
        ULL b = 0;
        for (int j = 0; j < 64; j++) {
            int x = p2->get_map();
            int c;
            int k = 0;
            for (; k <= x; k++) {
                c = r->read_bit();
                printf("%d", c);
            }
            printf("|");
            b = b << 1 | c;
            for (; k < n; k++) {
                c = r->read_bit();
                printf("%d", c);
            }
            printf("\n");
        }
        printf(" %llu ", b);
        a ^= b;
    }
    printf("%llx", a);
    for (int i = 0; i < a * 8; i++) {
        int x = p2->get_map();
        int c;
        int k = 0;
        for (; k <= x; k++) {
            c = r->read_bit();
        }
        w->write_bit(c);
        for (; k < n; k++) {
            r->read_bit();
        }
    }
    w->release();

    return 0;
}
