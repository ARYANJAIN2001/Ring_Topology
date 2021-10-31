#include <bits/stdc++.h>

using namespace std;

class Dataframe
{
public:
    string data;
    int seq_no;
    string src_mac, dest_mac;
    int sender_id;

    Dataframe(string data, int seq_no, string src_mac, string dest_mac,int sender_id)
    {
        this->data = data;
        this->seq_no = seq_no;
        this->src_mac = src_mac;
        this->dest_mac = dest_mac;
        this->sender_id = sender_id;
    }
};
