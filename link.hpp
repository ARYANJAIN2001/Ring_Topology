#include <bits/stdc++.h>
#include <unistd.h>
#include <dataframe.hpp>
using namespace std;

class Link;

class Node
{

private:
    string mac_addr;
    vector<Link *> links;
    unordered_map<string, Link *> routing_table;
    
    int node_id;

public:
    Node(std::string mac_addr, int node_id, vector<Link *> &links, unordered_map<string, Link *> &routing_table)
    {
        this->mac_addr = mac_addr;
        this->node_id = node_id;
        this->links = links;
        this->routing_table = routing_table;
        //this->token = token;
    }

    Dataframe *create_dataframe(string msg, string dest_mac)
    {
        // NOTE: change seq_no if required
        return new Dataframe(msg, 0, this->mac_addr, dest_mac,node_id);
    }

    void send(string destination_mac, string msg)
    {
        // Construct dataframe
        Dataframe *df = create_dataframe(msg, destination_mac);

        // Check token
            routing_table.at(destination_mac)->send_in_link(mac_addr, df);

            // Release token and then transfer it to the neighbour
            //release_token(destination_mac);
        
    }

    void recv(Dataframe *frame)
    {
        // Check if current node is destination
        if (mac_addr == frame->dest_mac)
        {
            // send_to_transport_layer();
            cout<<"Frame from sender : "<<frame->sender_id<<" accepted by node : "<< node_id<<endl;
        }
        else if(mac_addr == "00:00:00:00")
        {
           
        }
        else
        {
            relay(frame);
        }
    }

    void relay(Dataframe *frame)
    {
        // Called if current node is not destination
        routing_table.at(frame->dest_mac)->send_in_link(mac_addr, frame);
    }

    // void set_token()
    // {
    //     this->token = true;
    // }

    // void release_token(string destination_mac)
    // {
    //     this->token = false;
    //     // Give token to neighbour
    //     routing_table.at(destination_mac)->transfer_token();
    // }
};

class Link
{
    vector<Node *> node_list;
    unordered_map<string, int> mac_to_id;
    bool direction;
    int index;

public:
    Link(vector<Node *> &node_list, bool direction = false)
    {
        this->node_list = node_list;
        this->direction = direction;
        index = 0;
    }

    int get_next_node (int curr_ind) 
    {
        return direction ? (curr_ind + 1) % node_list.size() : (curr_ind - 1 + node_list.size()) % node_list.size();
    }

    void send_in_link(string curr_mac, Dataframe *df)
    {

        int next_node = get_next_node(mac_to_id.at(curr_mac));
        if(df->src_mac==curr_mac)
        {
            if (index != mac_to_id.at(curr_mac))
            {
                for (int i = index; i != next_node;  i = get_next_node(i))
                {
                    cout << "Token received by node " << i << endl;
                    usleep(200000);
                }
            }
            index = mac_to_id.at(curr_mac);
        }
        
        node_list[next_node]->recv(df);
         
    }

    void transfer_token()
    {
        index = get_next_node(index);
    }
};
