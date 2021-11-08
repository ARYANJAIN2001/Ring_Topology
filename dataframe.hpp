#include <bits/stdc++.h>

using namespace std;

class Dataframe
{
public:
    string data;
    int seq_no;
    string src_mac, dest_mac;
    int sender_id;
    int broadcast_sender;
    unordered_set<int>links_visited;
    Dataframe(string data, int seq_no, string src_mac, string dest_mac,int sender_id )
    {
        this->data = data;
        this->seq_no = seq_no;
        this->src_mac = src_mac;
        this->dest_mac = dest_mac;
        this->sender_id = sender_id;
        broadcast_sender = -1;
        links_visited.clear();
    }
};
