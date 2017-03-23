#include <iostream>
#include "utils.h"

using namespace std;

struct node
{
    // Each trie node will store the IP Address and the corresponding Mask so that it is easy to print
    string ip;
    string mask;

    // Go to next[0] when 0 is encountered in the binary form of IP... similarly for next[1]
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
    node* root; // Root of the Trie

public:
    Trie();
    void insert(string ip, string mask); // Insert an IP/Mask combination into the trie
    node* search(string ip); // Searches the best IP/Mask combination for an IP Address query
};
