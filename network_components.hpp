#include <bits/stdc++.h>
#include <unistd.h>
#include "dataframe.hpp"
using namespace std;

class Node;

class Link
{
public:
    vector<Node *> node_list;
    unordered_map<string, int> mac_to_id;
    bool direction;
    int index;

    static int assign_link_id;
    int link_id;
    vector<bool>link_failure;

    Link(vector<Node *> &node_list, bool direction = false)
    {
        this->node_list = node_list;
        this->direction = direction;
        link_id = ++assign_link_id;
        index = 0;
        
    }

    void send_in_link(string, Dataframe *);

    void make_mapping();

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

public:
    string mac_addr;
    vector<Link *> links;
    unordered_map<string, Link *> routing_table;

    static int assign_node_id;
    int node_id;
    bool failure;
    Node(std::string mac_addr)
    {
        this->mac_addr = mac_addr;
        node_id = ++assign_node_id;
        this->links = *(new vector<Link *>());
        this->routing_table = *(new unordered_map<string, Link *>());
        failure = false;
        //this->token = token;
    }


    void print_mac_address()
    {
        cout << this->mac_addr;
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

    void recv(Dataframe *frame, Link* curr_link_id)
    {
        if(failure==true){
           cout<<"Node failure for node "<<(char)(node_id+'A')<<", message  can't be transmitted further"<<endl;
        }
        else if (mac_addr == frame->dest_mac) // Check if current node is destination
        {
            // send_to_transport_layer();
            cout << "Frame from sender : " <<(char)(frame->sender_id+'A') << " accepted by node : " << (char)(node_id+'A') <<". Message: "<<frame->data<<endl;
        }
        else if (frame->dest_mac == "00:00:00:00")
        {
            cout << "Broadcast message received successfully by node : " <<(char)(node_id+'A') <<". Message: "<<frame->data<< endl;
            int original_sender = frame->broadcast_sender;
            frame->broadcast_sender = node_id;
            for (int i = 0; i < links.size(); ++i)
            {
                if (frame->links_visited.find(links[i]->link_id)==frame->links_visited.end())
                {
                    links[i]->send_in_link(mac_addr, frame);
                }
            }
            frame->broadcast_sender = original_sender;
            usleep(500000);
            curr_link_id->send_in_link(mac_addr, frame);
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

void Link:: make_mapping()
{
    for(int i=0;i<node_list.size();i++){
            mac_to_id[node_list[i]->mac_addr] = i;
            link_failure.push_back(false);
    }
}

void Link::send_in_link(string curr_mac, Dataframe *df)
{
    
    int next_node = get_next_node(mac_to_id.at(curr_mac));
    df->links_visited.insert(link_id);
    if(link_failure[mac_to_id.at(curr_mac)]&& direction )
    {
       cout<<"Link failed in clockwise direction at node "<< (node_list[mac_to_id.at(curr_mac)]->node_id +'A') <<". Messsage can't be transmitted further."<<endl; //Ayushman
       return;
    }
    else if(direction== false &&link_failure[next_node])
    {
       cout<<"Link failed in anticlockwise direction at node "<< (char)(node_list[next_node]->node_id+'a') <<". Messsage can't be transmitted further."<<endl; //Ayushman
       return;
    }
    if (df->src_mac == curr_mac)
    {
        if (index != mac_to_id.at(curr_mac))
        {
            
            for (int i = index; i != next_node; i = get_next_node(i))
            {
                cout << "Token received by node " <<(char)(node_list[i]->node_id+'A') << endl;
                usleep(200000);
            }
        }
        
       index  = get_next_node(next_node);
        
    }

    if (node_list[next_node]->node_id == df->broadcast_sender)
    {
        return;
    }

    node_list[next_node]->recv(df, this);
}
