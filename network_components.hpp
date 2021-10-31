#include <bits/stdc++.h>
#include <unistd.h>
#include "dataframe.hpp"
using namespace std;

class Node;

class Link
{
    vector<Node *> node_list;
    unordered_map<string, int> mac_to_id;
    bool direction;
    int index;

public:
    static int assign_link_id;
    int link_id;

    Link(vector<Node *> &node_list, bool direction = false)
    {
        this->node_list = node_list;
        this->direction = direction;
        link_id = ++assign_link_id;
        index = 0;
    }

    void send_in_link(string, Dataframe *);

    int get_next_node(int curr_ind)
    {
        return direction ? (curr_ind + 1) % node_list.size() : (curr_ind - 1 + node_list.size()) % node_list.size();
    }

    void transfer_token()
    {
        index = get_next_node(index);
    }
};

class Node
{

private:
    string mac_addr;
    vector<Link *> links;
    unordered_map<string, Link *> routing_table;

public:
    static int assign_node_id;
    int node_id;

    Node(std::string mac_addr, vector<Link *> &links, unordered_map<string, Link *> &routing_table)
    {
        this->mac_addr = mac_addr;
        node_id = ++assign_node_id;
        this->links = links;
        this->routing_table = routing_table;
        //this->token = token;
    }

    Dataframe *create_dataframe(string msg, string dest_mac)
    {
        // NOTE: change seq_no if required
        return new Dataframe(msg, 0, this->mac_addr, dest_mac, node_id);
    }

    void send(string destination_mac, string msg)
    {
        // Construct dataframe
        Dataframe *df = create_dataframe(msg, destination_mac);

        if (destination_mac == "00:00:00:00")
        {
            df->broadcast_sender = node_id;
            for (int i = 0; i < links.size(); i++)
            {
                links[i]->send_in_link(mac_addr, df);
            }
        }
        else
        {
            routing_table.at(destination_mac)->send_in_link(mac_addr, df);
        }
        // Check token

        // Release token and then transfer it to the neighbour
        //release_token(destination_mac);
    }

    void recv(Dataframe *frame, int curr_link_id)
    {
        // Check if current node is destination
        if (mac_addr == frame->dest_mac)
        {
            // send_to_transport_layer();
            cout << "Frame from sender : " << frame->sender_id << " accepted by node : " << node_id << endl;
        }
        else if (frame->dest_mac == "00:00:00:00")
        {
            cout << "Broadcast message received successfully by node : " << node_id << endl;
            int original_sender = frame->broadcast_sender;
            frame->broadcast_sender = node_id;
            for (int i = 0; i < links.size(); ++i)
            {
                if (links[i]->link_id != curr_link_id)
                {
                    links[i]->send_in_link(mac_addr, frame);
                }
            }
            frame->broadcast_sender = original_sender;

            links[curr_link_id]->send_in_link(mac_addr, frame);
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

   
};

void Link:: send_in_link(string curr_mac, Dataframe *df)
{

    int next_node = get_next_node(mac_to_id.at(curr_mac));

    if (df->src_mac == curr_mac)
    {
        if (index != mac_to_id.at(curr_mac))
        {
            for (int i = index; i != next_node; i = get_next_node(i))
            {
                cout << "Token received by node " << i << endl;
                usleep(200000);
            }
        }
        index = mac_to_id.at(curr_mac);
    }

    if (node_list[next_node]->node_id == df->sender_id)
    {
        return;
    }

    node_list[next_node]->recv(df, link_id);
}
