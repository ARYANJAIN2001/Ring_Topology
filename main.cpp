#include <bits/stdc++.h>
#include "network_components.hpp"

int Node::assign_node_id = -1;
int Link::assign_link_id = -1;

using namespace std;

int main()
{
    cout << "Ring Topology" << endl;

    Node *A = new Node("12:34:56:78");
    Node *B = new Node("12:34:65:78");
    Node *C = new Node("12:34:56:87");
    Node *D = new Node("12:34:65:87");
    Node *E = new Node("12:43:56:78");
    Node *F = new Node("21:34:65:78");
    Node *G = new Node("12:43:65:78");

    vector<Node *> n1 = {A, C, F};
    vector<Node *> n2 = {A, D, G, B, E};
    Link *L1 = new Link(n1, true);
    Link *L2 = new Link(n2, false);

    L1->make_mapping();
    L2->make_mapping();

    A->links = {L1, L2};
    B->links = {L2};
    C->links = {L1};
    F->links = {L1};
    D->links = {L2};
    E->links = {L2};
    G->links = {L2};

    A->routing_table[B->mac_addr] = L2;
    A->routing_table[G->mac_addr] = L2;
    A->routing_table[E->mac_addr] = L2;
    A->routing_table[D->mac_addr] = L2;
    A->routing_table[C->mac_addr] = L1;
    A->routing_table[F->mac_addr] = L1;

    B->routing_table[G->mac_addr] = L2;
    B->routing_table[E->mac_addr] = L2;
    B->routing_table[D->mac_addr] = L2;
    B->routing_table[A->mac_addr] = L2;
    B->routing_table[C->mac_addr] = L2;
    B->routing_table[F->mac_addr] = L2;

    C->routing_table[B->mac_addr] = L1;
    C->routing_table[G->mac_addr] = L1;
    C->routing_table[E->mac_addr] = L1;
    C->routing_table[D->mac_addr] = L1;
    C->routing_table[A->mac_addr] = L1;
    C->routing_table[F->mac_addr] = L1;

    D->routing_table[B->mac_addr] = L2;
    D->routing_table[G->mac_addr] = L2;
    D->routing_table[E->mac_addr] = L2;
    D->routing_table[A->mac_addr] = L2;
    D->routing_table[C->mac_addr] = L2;
    D->routing_table[F->mac_addr] = L2;

    E->routing_table[B->mac_addr] = L2;
    E->routing_table[G->mac_addr] = L2;
    E->routing_table[A->mac_addr] = L2;
    E->routing_table[D->mac_addr] = L2;
    E->routing_table[C->mac_addr] = L2;
    E->routing_table[F->mac_addr] = L2;

    F->routing_table[B->mac_addr] = L1;
    F->routing_table[G->mac_addr] = L1;
    F->routing_table[E->mac_addr] = L1;
    F->routing_table[D->mac_addr] = L1;
    F->routing_table[C->mac_addr] = L1;
    F->routing_table[A->mac_addr] = L1;

    G->routing_table[B->mac_addr] = L2;
    G->routing_table[A->mac_addr] = L2;
    G->routing_table[E->mac_addr] = L2;
    G->routing_table[D->mac_addr] = L2;
    G->routing_table[C->mac_addr] = L2;
    G->routing_table[F->mac_addr] = L2;

    //E->send(D->mac_addr,"Ayushman, thanks bhai for debugging!");
    cout << endl
         << "SIM 1: Sending message from Node B to Node D in same ring." << endl;
        
        B->send(D->mac_addr, "INTRA RING");

    cout<<endl<<"................................................................................................................"<<endl;
    usleep(1000000);

    cout << endl
         << "SIM 2: Sending message from Node B to Node C in a different ring." << endl;
        
        B->send(C->mac_addr, "TRANS RING");

    cout<<endl<<"................................................................................................................"<<endl;
    usleep(1000000);

    cout << endl
         << "SIM 3 (Node Failure): Sending message as in the above two simulations (from Node B to Node D and C) after Node A has failed." << endl;
        
        A->failure = true;
        B->send(D->mac_addr, "INTRA RING");
        B->send(C->mac_addr, "TRANS RING"); // this will fail
        A->failure = false;

    cout<<endl<<"................................................................................................................"<<endl;
    usleep(1000000);

    cout << endl
         << "SIM 4 (Link Failure): Sending message as in the above two simulations (from Node B to Node D and C) after Link L2 has failed." << endl;
        
        L2->link_failure[4] = true;
        B->send(D->mac_addr, "INTRA RING");
        B->send(C->mac_addr, "TRANS RING");

    cout<<endl<<"................................................................................................................"<<endl;
    usleep(1000000);

    cout << endl
         << "SIM 5 (Link Failure): Sending message from Node A to C after Link L2 has failed." << endl;
        
        A->send(C->mac_addr, "Sending A to C");

    cout<<endl<<"................................................................................................................"<<endl;
    usleep(1000000);

    cout << endl
         << "SIM 6 (Link Failure): Sending message from Node C to D after Link L2 has failed. (Routing through a damaged link)" << endl;

        C->send(D->mac_addr, "Routing through a damaged link");
        L2->link_failure[4] = false;

    cout<<endl<<"................................................................................................................"<<endl;
    usleep(1000000);

    cout << endl
         << "SIM 7 (Node Failure): Sending message from Node B to Node C after Node D has failed." << endl;
        
        D->failure = true;
        B->send(C->mac_addr, "Hii C");
        D->failure = false;

    cout<<endl<<"................................................................................................................"<<endl;
    usleep(1000000);

    cout << endl
         << endl
         << "Now simulating for broadcast messages." << endl;

    cout << endl
         << "SIM 8: Broadcast message from B to all nodes." << endl;
        
        B->send("00:00:00:00", "Hii All");

    cout<<endl<<"................................................................................................................"<<endl;
    usleep(2000000);
    cout << endl
         << "SIM 9: Broadcast message from B after Node A fails." << endl;
        
        A->failure = true;
        B->send("00:00:00:00", "Hii everyone");
        A->failure = false;

    cout<<endl<<"................................................................................................................"<<endl;
    usleep(2000000);

    cout << endl
         << "SIM 10: Broadcast message from B after Node E fails." << endl;
        
        E->failure = true;
        B->send("00:00:00:00", "Hii everyone");
        E->failure = false;

    usleep(2000000);
    cout<<endl;
    cout<<"------------------------------------------------- END OF SIMULATIONS ------------------------------------------------"<<endl;
    

    return 0;
}  