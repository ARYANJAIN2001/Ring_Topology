#include <bits/stdc++.h>
#include "network_components.hpp"

int Node::assign_node_id = -1;
int Link::assign_link_id = -1;

using namespace std;

int main()
{
    cout << "Ring Topology" << endl;

    int number_of_links = 0, number_of_nodes = 0;
    cout << "Please enter the number of links and the number of nodes in the network: ";
    cin >> number_of_links >> number_of_nodes;

    cout << "Please enter the directions of the links: " << endl;

    vector<Node *> nodes;
    vector<Link *> links(number_of_links);

    int x;
    bool dir;
    cout << "Please enter the directions of the links." << endl;
    for (int i = 0; i < number_of_links; i++)
    {
        cin >> dir;
        links[i] = new Link(nodes, dir);
    }

    string mac_addr;
    vector<Node *> nodes;
    for (int i = 0; i < number_of_nodes; i++)
    {
        cin >> mac_addr;
        nodes.push_back(new Node(mac_addr));
    }

    cout << "Here is a list of nodes: " << endl;
    for (int i = 0; i < number_of_nodes; i++)
    {
        cout << "1 - ";
        nodes[i]->print_mac_address();
        cout << '\n';
    }
    cout << flush;

    cout << "Please specify the structure of " << number_of_links << " links by specifying the ids nodes in the links in circular order." << endl;

    vector<Node *> nodes_in_link;

    for (int i = 0; i < number_of_links; i++)
    {
        nodes_in_link.clear();
        cout << "Number of nodes in link " << i << ": ";
        cin >> x;
        int y;

        for (int j = 0; j < x; j++)
        {
            cin >> y;
            nodes[y]->register_node_to_link(links[i]);
            nodes_in_link.push_back(nodes[y]);
        }

        for (int j = 0; j < x; j++)
        {
            for (int k = 0; k < x; k++)
            {
                if (nodes_in_link[j] != nodes_in_link[k])
                {
                    nodes_in_link[j]->routing_table[nodes_in_link[k]->mac_addr] = links[i];
                }
            }
        }
    }

    return 0;
}