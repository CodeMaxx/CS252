#include "trie.h"

Trie::Trie() {
    root = new node();
}

void Trie::insert(string ip, string mask) {
    node* trav = root;
    int len = mask_len(mask);
    string ip_bin = iptobin(ip);

    for (int i = 0; i < len; ++i) {
        if(trav->next[atoi(ip_bin[i])] == NULL)
            trav->next[atoi(ip_bin[i])] = new node();

        trav = trav->next[atoi(ip_bin[i])];
    }

    trav->ip = ip_bin;
    trav->mask = mask;
}

node* Trie::search(string ip) {
    node* trav = root;
    node* match = NULL;
    string ip_bin = iptobin(ip);

    for (int i = 0; i < ip_bin.size(); ++i) {
        if(trav->ip != "")
            match = trav;

        if(trav->next[atoi(ip_bin[i])] == NULL)
            break;
        else {
            trav = trav->next[atoi(ip_bin[i])];
        }
    }
    return match;
}
