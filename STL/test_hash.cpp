#include "hash_map.h"

int main() {
    HashMap<int, int> map(5);
    for(int i = 1; i < 30; i++) {
        map.insert(i, i + 1);
    }
    int val;
    bool find = map.find(4, val);
    if(!find || val != 5) {
        abort();
    }
    int val1 = map[7];
    if(val1 != 8) {
        abort();
    }
    printf("success");
}