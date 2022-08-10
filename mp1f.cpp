#include "mp1f.hpp"

int main() {
    int n=10;
    Keys* p = new Keys(n);
    p->read("rd.key");
    p->pt(0);
    p->release();
    for(int i=0;i<n;i++){
        p->pt(i);
    }
}
