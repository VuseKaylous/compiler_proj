#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<map>
using namespace std;

const int N = 50;

vector<string> end_signs{"<=", ">=", "<", ">", "==", "!=", "=", "+", "-", "*", "/", "!", "||", "&&", "(", ")", "[", "]","{", "}", ",", ";"};
vector<string> keywords{"int", "float", "void", "boolean", "if", "else", "for", "while", "break", "continue", "return"};

struct Node {
    string id;
    vector<vector<string> > next;
    bool is_end_node = false;
    bool has_checked_end_node = false;
    bool has_dfs = false;
    map<char, int> child;
    vector<char> children;
    Node(string id) {
        this->id = id;
        for (int i = 0; i < end_signs.size(); i++) {
            if (id == end_signs[i]) {
                is_end_node = true;
                children.push_back(end_signs[i][0]);
                break;
            }
        }
        if (!is_end_node) {
            for (int i = 0; i < keywords.size(); i++) {
                if (id == keywords[i]) {
                    is_end_node = true;
                    children.push_back(keywords[i][0]);
                    break;
                }
            }
        }
        has_checked_end_node = true;
    }
    Node() {
        this->id = "";
        has_checked_end_node = false;
        is_end_node = false;
    }
    void update(vector<string> vi) {
        next.push_back(vi);
    }
};

vector<Node> nodes;
map<string, int> mp;
vector<string> file;

int file_pos = 0;

bool check_end_node(int id_node) {
    if (nodes[id_node].has_checked_end_node) return nodes[id_node].is_end_node;
    nodes[id_node].has_checked_end_node = true;
    for (int i = 0; i < end_signs.size(); i++) {
        if (nodes[id_node].id == end_signs[i]) {
            nodes[id_node].is_end_node = true;
            nodes[id_node].children.push_back(end_signs[i][0]);
            return true;
        }
    }
    for (int i = 0; i < keywords.size(); i++) {
        if (nodes[id_node].id == keywords[i]) {
            nodes[id_node].is_end_node = true;
            nodes[id_node].children.push_back(keywords[i][0]);
            return true;
        }
    }
    return mp[nodes[id_node].id];
}

void dfs(int id_node) {
    if (nodes[id_node].has_dfs) return;
    nodes[id_node].has_dfs = true;
    if (!check_end_node(id_node)) {
        for (int i = 0; i < nodes[id_node].next.size(); i++) {
            int u = mp[nodes[id_node].next[i][0]];
            dfs(u);
            for (int j = 0; j < nodes[u].children.size(); j++) {
                nodes[i].children.push_back(nodes[u].children[j]);
                nodes[i].child[nodes[u].children[j]] = i + 1;
            }
        }
    }
}

void process(int id_node) {
    Node node = nodes[id_node];
    if (file_pos[i])
}

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
string get_substring(string str, int start, int gap) {
    string result = "";
    for (int i = start; i <= start + gap - 1; i++) {
        if (0 <= i && i < str.length()) result = result + str[i];
    }
    return result;
}
string get_substring(string str, int start) {
    if (start >= str.length()) return "";
    if (start == 0) return str;
    string result = "";
    for (int i = start; i < str.size(); i++) if (i >= 0) result = result + str[i];
    return result;
}
string trim_string(string str) {
    while (str[0] == ' ' || str[0] == '\t') str.erase(0,1);
    while (str.back() == ' ' || str[0] == '\t') str.pop_back();
    return str;
}



int main() {
    ifstream rule_reader;
    rule_reader.open("rules.txt");
    string s;
    nodes.push_back(Node(""));
    while (getline(rule_reader, s)) {
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

        nodes[pos].update(road);
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
        string temp_vsi = "";
        temp_vsi = temp_vsi + c; 
        vsi.push_back(temp_vsi);
        nodes[pos_letter].update(vsi);
        vsi.pop_back();
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        string temp_vsi = "";
        temp_vsi = temp_vsi + c;
        vsi.push_back(temp_vsi);
        nodes[pos_letter].update(vsi);
        vsi.pop_back();
    }
    
    vsi.push_back("_");
    nodes[pos_letter].update(vsi);
    vsi.pop_back();

    if (!mp["36"]) {
        Node newNode("36");
        mp["36"] = nodes.size();
        nodes.push_back(newNode);
    }
    pos_letter = mp["36"];
    for (char c = '0'; c <= '9'; c++) {
        string temp_vsi = "";
        temp_vsi = temp_vsi + c;
        vsi.push_back(temp_vsi);
        nodes[pos_letter].update(vsi);
        vsi.pop_back();
    }

    //------------------------------------------------------------------------------------------------------



    //------------------------------------------------------------------------------------------------------

    ifstream input;
    input.open("test.vc");
    file.clear();
    bool comment = false;
    int pos = 0;
    while (getline(input, s)) {
        if (comment) pos = s.size();
        else pos = 0;
        for (int i = 0; i < s.size(); i++) {
            if (!comment && s[i] == '/' && i + 1 < s.size() && s[i+1] == '*') {
                comment = true;
                string si = get_substring(s, pos, i - pos);
                si = trim_string(si);
                // cout << i << " test: " << si << endl;
                if (si != "") file.push_back(si);
                pos = i;
                i++;
            }
            else if (comment && s[i] == '*' && i + 1 < s.size() && s[i+1] == '/') {
                comment = false;
                pos = i+2;
                i++;
            }
            if (!comment && s[i] == '/' && i + 1 < s.size() && s[i+1] == '/') {
                string si = get_substring(s, pos, i-pos);
                si = trim_string(si);
                if (si != "") file.push_back(si);
                s = "";
                break;
            }
        }
        if (s == "") continue;
        // s = get_substr(s, pos);
        // cout << s << "\n";
        if (!comment) {
            s = trim_string(get_substring(s, pos));
            if (s != "") file.push_back(s);
        }
    }
    // cout << "\n";
    input.close();
    

    // for (int i = 0; i < nodes.size(); i++) {
    //     cout << nodes[i].id << "\n";
    //     for (int j = 0; j < nodes[i].next.size(); j++) {
    //         for (int it = 0; it < nodes[i].next[j].size(); it++) cout << nodes[i].next[j][it] << " ";
    //         cout << "\n";
    //     }
    //     cout << "\n";
    // }

    for (int i = 0; i < file.size(); i++) {
        cout << file[i] << "\n";
    }
}