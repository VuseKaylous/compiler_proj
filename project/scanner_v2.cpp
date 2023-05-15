#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<map>
#include<regex>
using namespace std;

const int N = 50;

vector<string> end_signs{"<=", ">=", "<", ">", "==", "!=", "=", "+", "-", "*", "/", "!", "||", "&&", "(", ")", "[", "]","{", "}", ",", ";"};
vector<string> keywords{"int", "float", "void", "boolean", "if", "else", "for", "while", "break", "continue", "return"};
vector<string> types{"STRINGLITERAL", "BOOLLITERAL", "INTLITERAL", "FLOATLITERAL", "ID", ""};

struct Node {
    string id;
    vector<vector<string> > next;
    bool is_end_node = false;
    bool has_checked_end_node = false;
    bool has_dfs = false;
    map<string, int> child;
    vector<string> children;
    Node(string id) {
        this->id = id;
        for (int i = 0; i < end_signs.size(); i++) {
            if (id == end_signs[i]) {
                is_end_node = true;
                children.push_back(end_signs[i]);
                break;
            }
        }
        if (!is_end_node) {
            for (int i = 0; i < keywords.size(); i++) {
                if (id == keywords[i]) {
                    is_end_node = true;
                    children.push_back(keywords[i]);
                    break;
                }
            }
        }
        for (int i = 0; i < types.size(); i++) {
            if (id == types[i]) {
                is_end_node = true;
                children.push_back(types[i]);
                break;
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
string parser;

int file_pos = 0;

bool check_end_node(int id_node) {
    if (id_node == 0) return true;
    // cout << "test check_end_node " << id_node << " " << nodes[id_node].has_checked_end_node << " " << nodes[id_node].is_end_node << "\n";
    if (nodes[id_node].has_checked_end_node) return nodes[id_node].is_end_node;
    nodes[id_node].has_checked_end_node = true;
    for (int i = 0; i < end_signs.size(); i++) {
        if (nodes[id_node].id == end_signs[i]) {
            nodes[id_node].is_end_node = true;
            nodes[id_node].children.push_back(end_signs[i]);
            return true;
        }
    }
    for (int i = 0; i < keywords.size(); i++) {
        if (nodes[id_node].id == keywords[i]) {
            nodes[id_node].is_end_node = true;
            nodes[id_node].children.push_back(keywords[i]);
            return true;
        }
    }
    for (int i = 0; i < types.size(); i++) {
        if (nodes[id_node].id == types[i]) {
            nodes[id_node].is_end_node = true;
            nodes[id_node].children.push_back(types[i]);
            return true;
        }
    }
    
    return false;
}

void dfs(int id_node) {
    if (id_node == 0) return;
    if (nodes[id_node].has_dfs) return;
    nodes[id_node].has_dfs = true;
    // cout << "fuck\n";
    if (!check_end_node(id_node)) {
        for (int i = 0; i < nodes[id_node].next.size(); i++) {
            int u = mp[nodes[id_node].next[i][0]];
            // cout << "test dfs: " << nodes[id_node].id << " " << id_node << " " << nodes[id_node].next[i][0] << " " << u  << "\n";
            if (u == id_node) continue;
            if (nodes[id_node].next[i][0] == "0") {
                nodes[id_node].children.push_back("");
                nodes[id_node].child[""] = i + 1;
                continue;
            }
            dfs(u);
            for (int j = 0; j < nodes[u].children.size(); j++) {
                // cout << "test dfs part 2: " << nodes[u].id << " " << nodes[u].children[j] << "\n";
                nodes[id_node].children.push_back(nodes[u].children[j]);
                nodes[id_node].child[nodes[u].children[j]] = i + 1;
            }
        }
    }
}

string word_process(string si) {
    for (int i = 0; i < end_signs.size(); i++) if (end_signs[i] == si) return si;
    for (int i = 0; i < keywords.size(); i++) if (keywords[i] == si) return si;
    
    if (si == "true" || si == "false") return "BOOLLITERAL";
    if (si[0] == '"' && si.back() == '"') return "STRINGLITERAL";
    if (regex_match(si, regex("[0-9]+"))) return "INTLITERAL";
    if (regex_match(si, regex("[0-9]+\\."))) return "FLOATLITERAL";
    if (regex_match(si, regex("[0-9]+\\.?(E|e)(\\+|-)?[0-9]+"))) return "FLOATLITERAL";
    if (regex_match(si, regex("[0-9]*\\.[0-9]+((E|e)(\\+|-)?[0-9]+)?"))) return "FLOATLITERAL";
    return "ID";
}

int process() {
    vector<string> st;
    st.push_back("$"); st.push_back("1");
    file_pos = 0;
    while (file_pos < file.size()) {
        if (file[file_pos] == "$") {
            break;
        }
        Node node = nodes[mp[st.back()]];
        // cout << file[file_pos] << " " << st.back() << endl;
        if (node.child[word_process(file[file_pos])] == 0 &&
            st.back() != word_process(file[file_pos]) && 
            node.child[""] == 0) {
            cout << "Compilation error: can't find next node\n" ;
            return -1;
        }
        while (st.back() != word_process(file[file_pos])) {
            // cout << "fuck\n" ;
            // cout << "process: " << file[file_pos] << " " << st.back() << " " << word_process(file[file_pos]) << endl;
            node = nodes[mp[st.back()]];
            int nextChild = node.child[word_process(file[file_pos])] - 1;
            if (nextChild == -1) {
                nextChild = node.child[""] - 1;
                if (nextChild == -1) {
                    cout << "Compilation error: out of bound\n" << nextChild;
                    return -1;
                }
                st.pop_back();
                for (int i = node.next[nextChild].size() - 1; i > 0; i--) {
                    st.push_back(node.next[nextChild][i]);
                }
                continue;
            }
            st.pop_back();
            for (int i = node.next[nextChild].size() - 1; i >= 0; i--) {
                st.push_back(node.next[nextChild][i]);
            }
        }
        st.pop_back();
        file_pos++;
    }
    return 0;
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
    freopen("test.vcps", "w", stdout);
    ifstream rule_reader;
    rule_reader.open("rules.txt");
    string s;
    nodes.push_back(Node("0"));
    // mp["0"] = 0;
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
            if (!mp[si]) {
                Node newNode(si);
                mp[si] = nodes.size();
                nodes.push_back(newNode);
            }
        }

        nodes[pos].update(road);
    }
    rule_reader.close();

    // testing the rules-handling

    // for (int i = 0; i < nodes.size(); i++) {
    //     cout << nodes[i].id << "\n";
    //     for (int j = 0; j < nodes[i].next.size(); j++) {
    //         for (int it = 0; it < nodes[i].next[j].size(); it++) cout << nodes[i].next[j][it] << " ";
    //         cout << "\n";
    //     }
    //     cout << "\n";
    // }

    //------------------------------------------------------------------------------------------------------

    for (int i = 1; i < nodes.size(); i++) dfs(i);

    // testing the rule-process-handling

    // for (int i = 0; i < nodes.size(); i++) {
    //     cout << nodes[i].id << "\n";
    //     for (int j = 0; j < nodes[i].children.size(); j++) {
    //         cout << nodes[i].children[j] << " " << nodes[i].child[nodes[i].children[j]] << "\n";
    //     }
    // }

    //------------------------------------------------------------------------------------------------------

    ifstream input;
    input.open("test.vctok");
    file.clear();
    
    bool comment = false;
    int pos = 0;
    while (getline(input, s)) {
        file.push_back(s);
        if (s == "$") break;
    }
    input.close();

    // testing the input-handling
    
    // for (int i = 0; i < file.size(); i++) {
    //     cout << file[i] << " " << word_process(i) << endl;
    // }

    //------------------------------------------------------------------------------------------------------

    if (process() == -1) return 0;
    cout << "Compilation successful.";

}