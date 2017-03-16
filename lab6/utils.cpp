#include "utils.h"
#include <bitset>
#include <sstream>

string iptobin(string ip)
{
    string binary;
    istringstream iss(ip);
    string token;

    while(getline(iss, token, '.'))
    {
        if(!token.empty())
            binary += bitset<8>(stoi(token)).to_string();

    }

    return binary;
}

int mask_len(string mask)
{
    string binary = iptobin(mask);
    int len = 0;

    for (int i = 0; i < binary.size(); ++i) {
        if(binary[i] == '1')
            len++;
        else
            break;
    }

    return len;
}
