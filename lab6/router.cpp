#include "trie.h"
#include <string>

int main()
{
    Trie router;

    string address, mask;

    cin >> address >> mask; // Read the table heading
    cin >> address >> mask; // Read the first entry in the router table

    while(address != "default")
    {
        router.insert(address, mask);
        cin >> address >> mask;
    }

    int k = atoi(mask.c_str()); // Number of queries

    for (int i = 0; i < k; ++i) {
        cin >> address;
        node* match = router.search(address);

        if(match == NULL)
            cout << address << " " << "default" << endl;
        else
            cout << address << " " << match->ip << " " << match->mask << endl;
    }
}
