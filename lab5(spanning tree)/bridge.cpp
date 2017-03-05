#include "bridge.h"
#include <sstream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <map>

struct config_msg
{
    string root_id;
    int dist; // Stores distance of sender bridge from the root bridge
    string sender_id;

    config_msg(string rid, int d, string sid)
    {
        root_id = rid;
        dist = d;
        sender_id =sid;
    }

    config_msg()
    {
        root_id = sender_id = "";
        dist = 0;
    }

    void update_msg(config_msg updated) // Update Message for a bridge
    {
        root_id = updated.root_id;
        dist = updated.dist;
        sender_id = updated.sender_id;
    }
};


struct bridge;

unordered_map<string, bridge*> all_bridges;

struct lan
{
    string id;
    vector<string> bridges;
    vector<string> designated_bridges;
    vector<string> hosts;

    lan()
    {
        id = "";
    }

    void send_data()
    {
        ;
    }

    bool operator<(lan l)
    {
        return id < l.id;
    }
};


struct bridge
{
    string id;
    unordered_map<lan*, string> port; // LANs v/s ports(RP, NP, DP)
    lan* root_lan;
    unordered_map<string, lan*> forwarding_table; // forwarding table
    bool is_root; // True when bridge is root. True by default
    config_msg msg;
    vector< tuple<config_msg, lan*> > new_queue; // Messages that need to be processes and forwarded in next second
    vector< tuple<config_msg, lan*> > old_queue; // Messages to be forwarded on this second


    bridge()
    {
        id = "";
        is_root = true;
        root_lan = NULL;
    }

    bridge(string id, bool is_root, config_msg msg)
    {
        this->id = id;
        this->is_root = is_root;
        this->msg = msg;
    }

    void update_msg(tuple<config_msg, lan*> mes)
    {
        config_msg m = get<0>(mes);
        config_msg new_msg;
        if(m.root_id < msg.root_id or (m.root_id == msg.root_id and m.dist + 1 < msg.dist))
        {
            new_msg.root_id = m.root_id;
            new_msg.dist = m.dist + 1;
            new_msg.sender_id = this->id;
            msg.update_msg(new_msg);
            port[get<1>(mes)] = "RP";
            get<1>(mes)->designated_bridges.erase(remove(get<1>(mes)->designated_bridges.begin(),
                                                         get<1>(mes)->designated_bridges.end(), this->id),
                                                  get<1>(mes)->designated_bridges.end());
            if(root_lan->id != "")
            {
                port[root_lan] = "DP";
                get<1>(mes)->designated_bridges.push_back(this->id);
            }

            root_lan = get<1>(mes);

            is_root = false;

        }

        if(m.sender_id < id and port[get<1>(mes)] == "DP"){
            port[get<1>(mes)] = "NP";
            get<1>(mes)->designated_bridges.erase(remove(get<1>(mes)->designated_bridges.begin(),
                                                         get<1>(mes)->designated_bridges.end(), this->id),
                                                  get<1>(mes)->designated_bridges.end());
        }
    }

    void forward(tuple<config_msg, lan*> mes)
    {
        for(auto r: port)
        {
            if(r.first != get<1>(mes) and r.second != "NP")
            {
                lan* l = r.first;
                for(auto s: l->bridges) // TODO Take care might need to store active bridges only
                {
                    bridge* b = all_bridges[s];
                    if(b->port[l] != "NP") {
                        tuple < config_msg, lan * > tup(get<0>(mes), l);
                        b->new_queue.push_back(tup);
                    }
                }
            }
        }
    }
};


vector<lan*> stop_cond;

class network
{
    unordered_map<string, lan*> all_lan; // lan-id vs lan
    bridge* bridges;
    int num_bridges;
    int trace;

public:

    void input()
    {
        cin >> trace;
        cin >> num_bridges;
        bridges = new bridge[num_bridges];
        cin.ignore();
        for(int i = 0; i < num_bridges; i++)
        {
            string line;
            getline(cin, line);
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;

            std::vector<std::string> tokens(beg, end); // done!
            string bridge_name;
            int j=0;
            for(auto& s: tokens) {
                if(j == 0)
                {
                    bridge_name = s;
                    bridge_name.pop_back();
                    config_msg* msg = new config_msg(bridge_name, 0, bridge_name);
                    bridges[i] = bridge(bridge_name, true, *msg);
                    all_bridges[bridge_name] = &bridges[i];
                }
                else
                {

                    if(all_lan.find(s) == all_lan.end())
                    {
                        lan* l = new lan();
                        l->id = s;
                        l->bridges.push_back(bridge_name);
                        l->designated_bridges.push_back(bridge_name);
                        all_lan[l->id] = l;
                        bridges[i].port[l] = "DP";
                        stop_cond.push_back(l);
                    }
                    else
                    {
                        lan* l = all_lan[s];
                        l->bridges.push_back(bridge_name);
                        l->designated_bridges.push_back(bridge_name);
                        bridges[i].port[all_lan[s]] = "DP";
                    }
                }
                j++;
            }
        }


        for(int i = 0; i < all_lan.size(); i++)
        {
            string line;
            getline(cin, line);
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;

            std::vector<std::string> tokens(beg, end); // done!
            string lan_name;
            int j=0;
            for(auto& s: tokens) {
                if(j == 0)
                {
                    lan_name = s;
                    lan_name.pop_back();
                }
                else
                {
                    lan* current = all_lan[lan_name];
                    current->hosts.push_back(s);
                }
                j++;
            }
        }

        init_stp();



    // printing for checking bridge structure
        for(int i = 0; i < num_bridges; i++)
        {
            cout << bridges[i].id << ": ";
            map<string, string> ordered;

            for(auto r: bridges[i].port) {
                ordered[r.first->id] = r.second;
            }

            for(auto it = ordered.cbegin(); it != ordered.cend(); ++it)
            {
                string s = it->first;
                cout << s << "-" << ordered[s] << " ";
            }
            cout << endl;
            cout << bridges[i].is_root << endl;
        }

        // printing for checking LAN structure
        for(auto s: all_lan)
        {
            cout << s.first << endl;
            cout << "Bridges: ";
            for(auto r: s.second->bridges)
            {
                cout << r << " ";
            }

            cout << endl << "Designated Bridges: ";

            for(auto r: s.second->designated_bridges)
            {
                cout << r << " ";
            }

            cout << endl << "Hosts: ";

            for(auto r: s.second->hosts)
            {
                cout << r << " ";
            }
            cout << endl;
        }
    }

    void init_stp()
    {
        ofstream trace_file;
        trace_file.open("trace.txt");
        int time = 0;
        while(true)
        {
            for(int i = 0; i < num_bridges; i++)
            {
                for(auto m: bridges[i].old_queue)
                {
                    bridges[i].update_msg(m);
                    tuple<config_msg, lan*> tup(bridges[i].msg, get<1>(m));
                    bridges[i].forward(tup);
                }

                if(bridges[i].is_root)
                    bridges[i].forward(bridges[i].msg);
            }

            for(int i = 0; i < num_bridges; i++)
            {
                bridges[i].old_queue = bridges[i].new_queue;
                bridges[i].new_queue.clear();
            }

            for(vector<lan*>::iterator it = stop_cond.begin();it != stop_cond.end();)
            {
                if((*it)->designated_bridges.size() == 1)
                    it = stop_cond.erase(it);
                else
                    ++it;
            }

            if(stop_cond.size() == 0)
                break;

            time++;
        }
        trace_file.close();
    }
};

int main()
{
    network N;
    N.input();
//    N.init_stp();
}