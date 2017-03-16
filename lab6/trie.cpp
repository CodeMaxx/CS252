#include "trie.h"

Trie::Trie() {
    root = new node();
}

void Trie::insert(string ip, string mask) {
    node* trav = root;
    int len = mask_len(mask);
    string ip_bin = iptobin(ip);

    for (int i = 0; i < len; ++i) {
        string bit(1, ip_bin[i]);
        if(trav->next[atoi(bit.c_str())] == NULL)
            trav->next[atoi(bit.c_str())] = new node();

        trav = trav->next[atoi(bit.c_str())];
    }

    trav->ip = ip;
    trav->mask = mask;
}

node* Trie::search(string ip) {
    node* trav = root;
    node* match = NULL;
    string ip_bin = iptobin(ip);

    for (int i = 0; i <= ip_bin.size(); ++i) {
        if(trav->ip != "")
            match = trav;

        string bit(1, ip_bin[i]);
        
        if(trav->next[atoi(bit.c_str())] == NULL)
            break;
        else {
            trav = trav->next[atoi(bit.c_str())];
        }
    }
    return match;
}
