#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#include<map>
#include<regex>
using namespace std;

const int N = 50;

// the terminal characters
vector<string> end_signs{"<=", ">=", "<", ">", "==", "!=", "=", "+", "-", "*", "/", "!", "||", "&&", ",", ")", "]", "(", "[","{", "}", ";"};
vector<string> keywords{"int", "float", "void", "boolean", "if", "else", "for", "while", "break", "continue", "return"};
vector<string> types{"STRINGLITERAL", "BOOLLITERAL", "INTLITERAL", "FLOATLITERAL", "ID", ""};

// a Node represents a state
struct Node {
    string id; // each state differ by this id.
    vector<vector<string> > next; // this is the list of next states
    bool is_end_node = false;
    bool has_checked_end_node = false; // these 2 bools is used to check if this is the terminal state.
    bool has_dfs = false;
    map<string, int> child; // this is used to find the FIRST states
    vector<string> children; // list of FIRST states
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

vector<Node> nodes; // list of the states
map<string, int> mp; // this is to find the state we need
vector<string> file; // this is to save the input
string parser; // this is the answer

int file_pos = 0;

// this function check whether the node in question is the end node
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

// this is used to find the FIRST states of the nodes
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

// this function is used to determine which end-state is the present node
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

// this function is used to update the answer string
string newLine = "\n";
void update_parser(string si) {
    if (si == "{") {
        parser = parser + newLine + si;
        newLine = newLine + "\t";
        parser = parser + newLine;
        return;
    }
    if (si == ";") {
        parser = parser + si + newLine;
        return;
    }
    if (si == "}") {
        parser.pop_back();
        parser = parser + si;
        newLine.pop_back();
        parser = parser + newLine;
        return;
    }
    for (int i = 0; i < keywords.size(); i++) {
        if (si == keywords[i]) {
            if (i + 3 >= keywords.size()) {
                parser = parser + si;
                return;
            }
            parser = parser + si + " ";
            return;
        }
    }
    for (int i = 0; i < end_signs.size(); i++) {
        if (si == end_signs[i]) {
            if (i + 5 >= end_signs.size()) {
                parser = parser + si;
                return;
            }
            parser = parser + si + " ";
            return;
        }
    }
    parser = parser + si + " ";
}

// this function is used to parse the input
int process() {
    vector<pair<string, bool> > st;
    st.push_back({"$", true}); st.push_back({"1", true});
    file_pos = 0;
    while (file_pos < file.size()) {
        if (file[file_pos] == "$") {
            break;
        }
        while (!st.back().second) {
            // cout << "erase: " << st.back().first << " " << st.back().second << endl;
            parser = parser + st.back().first;
            st.pop_back();
        }
        Node node = nodes[mp[st.back().first]];
        // cout << file[file_pos] << " " << st.back().first << " " << st.back().second << endl;
        if (node.child[word_process(file[file_pos])] == 0 &&
            st.back().first != word_process(file[file_pos]) && 
            node.child[""] == 0) {
            cout << "Compilation error: can't find next node\n" ;
            return -1;
        }
        while (st.back().first != word_process(file[file_pos])) {
            while (!st.back().second) {
                // cout << "erase: " << st.back().first << " " << st.back().second << endl;
                parser = parser + st.back().first;
                st.pop_back();
            }
            // cout << "process: " << file[file_pos] << " " << st.back().first << " " << st.back().second << " " << word_process(file[file_pos]) << endl;
            node = nodes[mp[st.back().first]];
            int nextChild = node.child[word_process(file[file_pos])] - 1;
            if (nextChild == -1) {
                nextChild = node.child[""] - 1;
                if (nextChild == -1) {
                    cout << "Compilation error: out of bound\n" << file[file_pos] << " " << st.back().first << " " << st.back().second << " " << mp[st.back().first] << " " << node.id;
                    return -1;
                }
                st.pop_back();
                st.push_back({")", false});
                for (int i = node.next[nextChild].size() - 1; i > 0; i--) {
                    st.push_back({node.next[nextChild][i], true});
                }
                if (st.back().second) st.push_back({"(", false});
                else st.pop_back();
                while (!st.back().second) {
                    // cout << "erase: " << st.back().first << " " << st.back().second << endl;
                    parser = parser + st.back().first;
                    st.pop_back();
                }
                continue;
            }
            st.pop_back();
            st.push_back({")", false});
            for (int i = node.next[nextChild].size() - 1; i >= 0; i--) {
                st.push_back({node.next[nextChild][i], true});
            }
            if (st.back().second) st.push_back({"(", false});
            else st.pop_back();
            while (!st.back().second) {
                // cout << "erase: " << st.back().first << " " << st.back().second << endl;
                parser = parser + st.back().first;
                st.pop_back();
            }
        }
        update_parser(file[file_pos]);
        st.pop_back();
        while (!st.back().second) {
            // cout << "erase: " << st.back().first << " " << st.back().second << endl;
            parser = parser + st.back().first;
            st.pop_back();
        }
        file_pos++;
    }
    return 0;
}

// these 5 functions are used for string manipulation
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

    //------------------------------------------ read the rules -----------------------------------------------
    ifstream rule_reader;
    rule_reader.open("rules.txt");
    string s;
    nodes.push_back(Node("0"));
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

    // testing the read rules

    // for (int i = 0; i < nodes.size(); i++) {
    //     cout << nodes[i].id << "\n";
    //     for (int j = 0; j < nodes[i].next.size(); j++) {
    //         for (int it = 0; it < nodes[i].next[j].size(); it++) cout << nodes[i].next[j][it] << " ";
    //         cout << "\n";
    //     }
    //     cout << "\n";
    // }

    //------------------------------------------ handling the rules ---------------------------------------------------

    for (int i = 1; i < nodes.size(); i++) dfs(i);

    // testing the rule-handling process

    // for (int i = 0; i < nodes.size(); i++) {
    //     cout << nodes[i].id << "\n";
    //     for (int j = 0; j < nodes[i].children.size(); j++) {
    //         cout << nodes[i].children[j] << " " << nodes[i].child[nodes[i].children[j]] << "\n";
    //     }
    // }

    //------------------------------------------- read the input .vc file ---------------------------------------------

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

    // testing the input
    
    // for (int i = 0; i < file.size(); i++) {
    //     cout << file[i] << " " << word_process(i) << endl;
    // }

    //----------------------------------------------- parse the input -------------------------------------------------

    if (process() == -1) return 0;
    // cout << "Compilation successful.\n";
    cout << parser ;

}