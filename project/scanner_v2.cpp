#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<map>
using namespace std;

const int N = 50;

vector<string> end_signs{"<=", ">=", "<", ">", "==", "!=", "=", "+", "-", "*", "/", "!", "||", "&&", "(", ")", "[", "]","{", "}", ",", ";"};

struct Node {
    string id;
    vector<pair<vector<string>, int> > next;
    Node(string id) {
        this->id = id;
    }
    Node() {
        this->id = "";
    }
    void update(vector<string> vi, int x) {
        next.push_back({vi, x});
    }
};

vector<Node> nodes;
map<string, int> mp;

string get_first_substring(string str) {
    string result = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ') break;
        result = result + str[i];
    }
    return result;
}
string trim_first_substring(string str) {
    while (str.size() > 0 && str[0] != ' ') str.erase(0, 1);
    while (str.size() > 0 && str[0] == ' ') str.erase(0, 1);
    return str;
}
string get_substring(string str, int start, int end) {
    string result = "";
    for (int i = start; i <= end; i++) {
        if (0 <= i && i < str.length()) result = result + str[i];
    }
    return str;
}
string get_substring(string str, int start) {
    string result = "";
    for (int i = start; i < str.size(); i++) if (i >= 0) result = result + str[i];
    return result;
}
string trim_string(string str) {
    while (str[0] == ' ') str.erase(0,1);
    while (str.back() == ' ') str.pop_back();
    return str;
}

int main() {
    ifstream rule_reader;
    rule_reader.open("rules.txt");
    string s;
    nodes.push_back(Node(""));
    int num_rules = 0;
    while (getline(rule_reader, s)) {
        num_rules++;
        s = trim_string(s);
        string id = get_first_substring(s);
        s = trim_first_substring(s);
        if (!mp[id]) {
            Node newNode(id);
            mp[id] = nodes.size();
            nodes.push_back(newNode);
        }

        int pos = mp[id];
        vector<string> road;
        while (s.size() > 0) {
            string si = get_first_substring(s);
            s = trim_first_substring(s);
            road.push_back(si);
        }
        nodes[pos].update(road, num_rules);
    }
    rule_reader.close();
    if (!mp["35"]) {
        Node newNode("35");
        mp["35"] = nodes.size();
        nodes.push_back(newNode);
    }
    int pos_letter = mp["35"];
    vector<string> vsi;
    for (char c = 'a'; c <= 'z'; c++) {
        num_rules++;
        string temp_vsi = "";
        temp_vsi = temp_vsi + c; 
        vsi.push_back(temp_vsi);
        nodes[pos_letter].update(vsi, num_rules);
        vsi.pop_back();
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        num_rules++;
        string temp_vsi = "";
        temp_vsi = temp_vsi + c;
        vsi.push_back(temp_vsi);
        nodes[pos_letter].update(vsi, num_rules);
        vsi.pop_back();
    }
    num_rules++;
    
    vsi.push_back("_");
    nodes[pos_letter].update(vsi, num_rules);
    vsi.pop_back();

    if (!mp["36"]) {
        Node newNode("36");
        mp["36"] = nodes.size();
        nodes.push_back(newNode);
    }
    pos_letter = mp["36"];
    for (char c = '0'; c <= '9'; c++) {
        num_rules++;
        string temp_vsi = "";
        temp_vsi = temp_vsi + c;
        vsi.push_back(temp_vsi);
        nodes[pos_letter].update(vsi, num_rules);
        vsi.pop_back();
    }

    // for (int i = 0; i < nodes.size(); i++) {
    //     cout << nodes[i].id << "\n";
    //     for (int j = 0; j < nodes[i].next.size(); j++) {
    //         cout << nodes[i].next[j].second << " ";
    //         for (int it = 0; it < nodes[i].next[j].first.size(); it++) cout << nodes[i].next[j].first[it] << " ";
    //         cout << "\n";
    //     }
    //     cout << "\n";
    // }
}