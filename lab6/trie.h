#include <iostream>
#include "utils.h"

using namespace std;

struct node
{
    string ip;
    string mask;

    node* next[2];

    node()
    {
        ip = mask = "";
        next[0] = NULL;
        next[1] = NULL;
    }

    node(string ip_add, string sub_mask)
    {
        ip = ip_add;
        mask = sub_mask;
    }

};

class Trie
{
    node* root;

public:
    Trie();
    void insert(string ip, string mask);
    node* search(string ip);
};
