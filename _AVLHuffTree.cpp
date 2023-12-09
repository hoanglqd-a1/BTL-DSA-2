#include <bits/stdc++.h>
using namespace std;

class customer;
class AVLHuffTree;

int MAXSIZE = 1;
string Hand;
int CUSTOMERCOUNT = 0;

char ceasarEncode(char ch, int t);
bool comp1(AVLHuffTree a, AVLHuffTree b);
bool comp2(pair<char, int> a, pair<char, int> b);
int binaryToInt(string& binary);

class HuffNode{
public:
    HuffNode *left=nullptr, *right=nullptr;
public:
    virtual ~HuffNode() {}
    virtual bool isLeaf() = 0;
    virtual int getWeight() = 0;
    virtual int getHeight() = 0;
    virtual void updateHeight() = 0;
    virtual int balanceValue() = 0;
    virtual char getValue() = 0;
};

class LeafNode : public HuffNode{
public: 
    char val;
    int wgt;
    int height = 0;
public:
    LeafNode(pair<char, int>p) : val{p.first}, wgt{p.second} {}
    ~LeafNode() {}
    bool isLeaf() {return true;}
    int getWeight() {return wgt;}
    int getHeight() {return height;}
    void updateHeight() {height = 0;}
    int balanceValue() {return 0;}
    char getValue() {return val;}
};

class InnerNode : public HuffNode{
public:
    int wgt;
    int height;
public:
    InnerNode() {};
    InnerNode(HuffNode* l, HuffNode* r) {
        left = l;
        right = r;
        wgt = l->getWeight() + r->getWeight();
        height = max(l->getHeight(), r->getHeight()) + 1;
    }
    ~InnerNode(){
        delete left;
        delete right;
    }
    bool isLeaf() {return false;}
    int getWeight() {return wgt;}
    int getHeight() {return height;}
    void updateHeight(){
        int l = (left) ? left->getHeight() : 0;
        int r = (left) ? right->getHeight(): 0;
        height = max(l, r) + 1;
    }
    int balanceValue() {return left->getHeight() - right->getHeight();}
    char getValue() {return '!';}
};

class AVLHuffTree{
public:
    HuffNode* root = nullptr;
    int nthTree;
public:
    AVLHuffTree() {}
    AVLHuffTree(HuffNode* node, int nthTree) : root{node}, nthTree{nthTree} {}
    AVLHuffTree(AVLHuffTree l, AVLHuffTree r, int nthTree) : nthTree{nthTree} {
        root = new InnerNode(l.root, r.root);
        root = balance(root);
    }
    int weight() {return root->getWeight();}
    HuffNode* rotateLeft(HuffNode* p){
        if(!p->right) return p;
        if(root==p) root = p->right;
        HuffNode* tmp = p->right;
        p->right = tmp->left;
        tmp->left = p;
        p->updateHeight();
        tmp->updateHeight();
        return tmp;
    }
    HuffNode* rotateRight(HuffNode* p) {
        if(!root->left) return p;
        if(root==p) root = p->left;
        HuffNode* tmp = p->left;
        p->left = tmp->right;
        tmp->right = p;
        p->updateHeight();
        tmp->updateHeight();
        return tmp;
    }
    HuffNode* balance(HuffNode* p){
        for(int i=0;i<3;++i){
            if(p->balanceValue() < -1){
                if(p->right->balanceValue() > 0){
                    p->right = rotateRight(p->right);
                }
                p = rotateLeft(p);
            }
            else if(p->balanceValue() > 1){
                if(p->left->balanceValue() < 0){
                    p->left = rotateRight(p->left);
                }
                p = rotateRight(p);
            }
            else break;
        }
        return p;
    }
    void printTree(){
        cout << "print Tree: " <<endl;
        HuffNode* t = root;
        stack<HuffNode*> st;
        st.push(t);
        while(!st.empty()){
            HuffNode* t = st.top();
            cout << t->getHeight() <<" ";
            st.pop();
            if(t->right) st.push(t->right);
            if(t->left) st.push(t->left);
            cout << t->getValue() <<" "<<t->getWeight()<<endl;
        }
    }
};

class customer{
public:
    string name;
    vector<pair<char, int>> Cname;
    AVLHuffTree Tname;
    string binaryResult;
    int result;
    int nthCustomer;
public:
    customer(string name) : name{name} {
        buildAVLHuffTree();
        updateHAND();
        nthCustomer = CUSTOMERCOUNT;
        ++CUSTOMERCOUNT;
    }
    ~customer(){delete Tname.root;}
    void buildAVLHuffTree(){
        unordered_map<char, int> freq, Cfreq;
        for(const auto& ch : name) freq[ch]++;
        vector<pair<char, int>> v;
        for(auto& p : freq) v.push_back(make_pair(ceasarEncode(p.first, p.second), p.second));
        for(const auto& a : v) Cfreq[a.first] += a.second;
        for(const auto& a: Cfreq) Cname.push_back(make_pair(a.first, a.second));
        sort(Cname.begin(), Cname.end(), comp2);
        priority_queue<AVLHuffTree, vector<AVLHuffTree>,  function<bool(AVLHuffTree, AVLHuffTree)>> pq(comp1);
        int s = (int)Cname.size(), nth=0;
        for(nth;nth<s;++nth){
            HuffNode* t = new LeafNode(Cname[nth]);
            pq.push(AVLHuffTree(t, nth));
        }
        while(pq.size()>1){
            AVLHuffTree t1 = pq.top();
            pq.pop();
            AVLHuffTree t2 = pq.top();
            pq.pop();
            pq.push(AVLHuffTree(t1, t2, nth));
            ++nth;
        }
        Tname = pq.top();
        unordered_map<char, string> mp;
        string binary, binName;
        if(!Tname.root->isLeaf()) createDictionary(mp, binary, Tname.root);
        else mp[Tname.root->getValue()] = "0";
        for(const auto& p: Cname){
            for(int j=0;j<p.second;++j){
                binName += mp[p.first];
            }
        }
        int t = (int)binName.size();
        for(int i=0;i<10&&i<t;++i){
            this->binaryResult+= binName[t-1-i];
        }
        result = binaryToInt(binaryResult);
    }
    void createDictionary(unordered_map<char, string>& mp, string& binary, HuffNode* t){
        if(t->isLeaf()){
            mp[t->getValue()] = binary;
            return;
        }
        if(t->left){
            binary +='0';
            createDictionary(mp, binary, t->left);
            binary.pop_back();
        }
        if(t->right){
            binary += '1';
            createDictionary(mp, binary, t->right);
            binary.pop_back();
        }
    }
    void printCustomer(){
        cout << "name: " << this->name <<endl;
        Tname.printTree();
        cout << "nth: " <<nthCustomer <<endl;
        cout << "char frequency: ";
        for(const auto& a : Cname){
            cout << a.first <<": "<<a.second <<" -> ";
        }
        cout << endl;
        cout << "binary result: "<< this->binaryResult <<endl;
        cout << "result: " << this->result <<endl;
    }
    void updateHAND(){
        Hand.clear();
        recursiveUpdateHand(Hand, Tname.root);
    }
    void recursiveUpdateHand(string& target, HuffNode* t){
        if(!t) return;
        recursiveUpdateHand(target, t->left);
        if(t->isLeaf()){
            target += t->getValue();
        }
        else target += to_string(t->getWeight());
        target += "\n";
        recursiveUpdateHand(target, t->right);
    }
};

char ceasarEncode(char ch, int t){
    if(isupper(ch)){
        ch = (ch-'A'+t)%26 + 'A';
    }
    else{
        ch = (ch-'a'+t)%26 + 'a';
    }
    return ch;
}
bool comp1(AVLHuffTree a, AVLHuffTree b){
    if(a.weight()== b.weight()) return a.nthTree > b.nthTree;
    return a.weight() > b.weight();
}
bool comp2(pair<char, int> a, pair<char, int> b){
    if(a.second != b.second) return a.second < b.second;
    if(islower(a.first)&&isupper(b.first)) return true;
    if(isupper(a.first)&&islower(b.first)) return false;
    return a.first < b.first;
}
int binaryToInt(string& binary){
    int s = (int)binary.size(), result = 0;
    for(int i=0;i<s;++i){
        result *= 2;
        result += binary[i] - '0';
    }
    return result;
}

