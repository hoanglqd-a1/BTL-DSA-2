#include "main.h"

class customer;
class AVLHuffTree;
class heapNode;

int MAXSIZE = 1;
string Hand;
int CUSTOMERCOUNT = 0;

char ceasarEncode(char ch, int t);
bool comp1(AVLHuffTree a, AVLHuffTree b);
bool comp2(pair<char, int> a, pair<char, int> b);
int binaryToInt(string& binary);
bool comp3(heapNode* a, heapNode* b);
class HuffNode{
public:
    HuffNode *left=nullptr, *right=nullptr;
public:
    virtual ~HuffNode() {}
    virtual bool isLeaf() = 0;
    virtual int getWeight() = 0;
    virtual int getHeight() = 0;
    virtual int balanceValue() = 0;
    virtual char getValue() = 0;
    virtual void updateNode() = 0;
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
    int getHeight() {return 0;}
    void updateNode() {height = 0;}
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
    void updateNode(){
        int l = left->getHeight();
        int r = right->getHeight();
        height = max(l, r) + 1;
    }
    int balanceValue() {
        if(!isLeaf()) return left->getHeight() - right->getHeight();
        return 0;
    }
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
        int rotate = 3;
        root = balance(root, rotate);
    }
    int weight() {return root->getWeight();}
    HuffNode* rotateLeft(HuffNode* p){
        if(!p->right) return p;
        if(root==p) root = p->right;
        HuffNode* tmp = p->right;
        p->right = tmp->left;
        tmp->left = p;
        p->updateNode();
        tmp->updateNode();
        return tmp;
    }
    HuffNode* rotateRight(HuffNode* p) {
        if(!root->left) return p;
        if(root==p) root = p->left;
        HuffNode* tmp = p->left;
        p->left = tmp->right;
        tmp->right = p;
        p->updateNode();
        tmp->updateNode();
        return tmp;
    }
    HuffNode* balance(HuffNode* p, int& rotate){
        if(p->isLeaf()) return p;
        if(rotate==0) return p;
        while(rotate){
            if(p->balanceValue() < -1){
                if(p->right->balanceValue() > 0){
                    p->right = rotateRight(p->right);
                    p->updateNode();
                }
                p = rotateLeft(p);
                --rotate;
            }
            else if(p->balanceValue() > 1){
                if(p->left->balanceValue() < 0){
                    p->left = rotateLeft(p->left);
                }
                p = rotateRight(p);
                --rotate;
            }
            else break;
        }
        p->left = balance(p->left, rotate);
        p->right = balance(p->right, rotate);
        p->updateNode();
        if(p->balanceValue()<-1 || p->balanceValue() > 1) p = balance(p, rotate); // ????
        return p;
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
    ~customer(){
        delete Tname.root;
    }
    void buildAVLHuffTree(){
        map<char, int> freq, Cfreq;
        for(const auto& ch : name) freq[ch]++;
        vector<pair<char, int>> v;
        for(auto& p : freq) v.push_back(make_pair(ceasarEncode(p.first, p.second), p.second));
        for(const auto& a : v) Cfreq[a.first] += a.second;
        for(const auto& a: Cfreq) Cname.push_back(make_pair(a.first, a.second));
        sort(Cname.begin(), Cname.end(), comp2);
        priority_queue<AVLHuffTree, vector<AVLHuffTree>,  function<bool(AVLHuffTree, AVLHuffTree)>> pq(comp1);
        int s = (int)Cname.size(), nth=0;
        for(;nth<s;++nth){
            HuffNode* t = new LeafNode(Cname[nth]);
            pq.push(AVLHuffTree(t, nth));
        }
        while(pq.size()>1){
            AVLHuffTree t1 = pq.top();
            pq.pop();
            AVLHuffTree t2 = pq.top();
            pq.pop();
            AVLHuffTree tmp(t1, t2, nth);
            pq.push(tmp);
            ++nth;
        }
        Tname = pq.top();
        map<char, string> mp;
        string binary{}, binName, cname;
        if(!Tname.root->isLeaf()) createDictionary(mp, binary, Tname.root);
        else{
            mp[Tname.root->getValue()] = "0";
        }
        for(const auto& ch : name){
            cname += ceasarEncode(ch, freq[ch]);
        }
        for(const auto& ch : cname){
            binName += mp[ch];
        }
        for(int i=0;i<10&&i<(int)binName.size();++i){
            binaryResult += binName[(int)binName.size()-1-i];
        }
        result = binaryToInt(binaryResult);
    }
    void createDictionary(map<char, string>& mp, string& binary, HuffNode* t){
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

class heapNode{
public:
    deque<customer*> LIST;
    int size=0, lastChange, ID;
public:
    heapNode(int ID) : ID{ID} {}
    ~heapNode(){
        while(size){
            customer* cus = this->popFront();
            delete cus;
        }
    }
    bool add(customer* cus){
        if(cus==nullptr) throw "pass nullptr";
        LIST.push_back(cus);
        ++size;
        if(size==1) {
            return true;
        }
        return false;
    }
    customer* popFront(){
        if(size==0) throw "empty queue";
        customer* t = LIST.front();
        LIST.pop_front();
        --size;
        return t;
    }
    customer* popBack(){
        if(size==0) throw "empty queue";
        customer* t = LIST.back();
        LIST.pop_back();
        --size;
        return t;
    }
    void print(int num){
        stack<customer*> st;
        while(num--&&size){
            st.push(this->popBack());
            cout << ID <<'-'<<st.top()->result<<"\n";
        }
        while(!st.empty()){
            customer *tmp = st.top();
            st.pop();
            LIST.push_back(tmp);
            ++size;
        }
    }
    bool operator > (const heapNode& X){
        if(this->size==X.size) return this->lastChange > X.lastChange;
        return this->size > X.size;
    }
};

class minHeap{
public:
    vector<heapNode*> AREA;
    int size=0, changeCnt = 0;
    map<int, int> mp;
public:
    minHeap(){
        AREA = vector<heapNode*>(MAXSIZE);
        for(int position=0;position<MAXSIZE;++position) {
            int ID = position+1;
            AREA[position] = new heapNode(ID);
            mp[ID] = position;
        }
    }
    ~minHeap() {
        for(int i=0;i<MAXSIZE;++i){
            delete AREA[i];
        }
    }
    void heapUp(int pos){
        if(pos < 0||pos>=size) throw "out of range";
        while(pos > 0){
            int parent = (pos-1)/2;
            if(*AREA[parent] > *AREA[pos]){
                swapNode(parent, pos);
                pos = parent;
            }
            else break;
        }
    }
    void heapDown(int pos){
        if(pos>=size) throw "out of range";
        while(pos*2+1 < size){
            int left = 2*pos+1, right = 2*pos+2;
            if(*AREA[left] > *AREA[pos] && (right>=size||*AREA[right] > *AREA[pos])) break;
            if(right >= size || *AREA[right] > *AREA[left]){
                swapNode(left, pos);
                pos = left;
            }
            else{
                swapNode(right, pos);
                pos = right;
            }
        }
    }
    void swapNode(int posA, int posB){
        swap(mp[AREA[posA]->ID], mp[AREA[posB]->ID]);
        swap(AREA[posA], AREA[posB]);
    }
    void add(customer* cus){
        if(!cus) throw "pass nullptr";
        int ID = cus->result % MAXSIZE + 1;
        int pos = mp[ID];
        AREA[pos]->lastChange = changeCnt;
        ++changeCnt;
        if(AREA[pos]->add(cus)){
            swapNode(pos, size);
            pos = size;
            ++size;
            heapUp(pos);
        }
        heapDown(pos);
    }
    void remove(int num){
        int s = min(num, size);
        vector<heapNode*> v;
        for(int i=0;i<s;++i){
            v.push_back(AREA[i]);
        }
        string removedCustomer{};
        sort(v.begin(), v.end(), comp3);
        for(int i=0;i<s;++i){
            if(size==0) break;
            for(int j=0;j<num;++j){
                if(v[i]->size==0) break;
                customer* tmp = v[i]->popFront();
                removedCustomer += (to_string(tmp->result) + '-' + to_string(v[i]->ID) + "\n");
                delete tmp;
            }
            int pos = mp[v[i]->ID];
            AREA[pos]->lastChange = changeCnt;
            ++changeCnt;
            if(v[i]->size==0){
                --size;
                if(pos >= size) continue;
                swapNode(pos, size);
                if(pos > 0 && *AREA[(pos-1)/2] > *AREA[pos]) heapUp(pos);
                else heapDown(pos);
            }
            else{
                heapUp(pos);
            }
        }
        cout << removedCustomer;
    }
    void traversePreorder(int num, int i){
        if(i<0) throw "error";
        if(i>=size) return;
        AREA[i]->print(num);
        traversePreorder(num, 2*i+1);
        traversePreorder(num, 2*i+2);
    }
};

class Sukuna{
public:
    minHeap restaurant;
public:
    void LAPSE(customer* cus){
        restaurant.add(cus);
    }
    void KEITEIKEN(int num){
        restaurant.remove(num);
    }
    void CLEAVE(int num){
        restaurant.traversePreorder(num, 0);
    }
};

class Node{
public:
    customer* cus;
    Node *left=nullptr, *right=nullptr;
public:
    Node() {}
    Node(customer* cus) : cus(cus), left{nullptr}, right{nullptr} {}
    ~Node() {delete cus;}
};

class BST{
public:
    Node *root = nullptr;
    int nodeCount=0;
    queue<int> q;
public:
    ~BST(){
        // cout <<"delete BST"<<endl;
        int all = nodeCount;
        removeYNode(root, all);
    }
    void add(customer* cus){
        q.push(cus->result);
        ++nodeCount;
        if(nodeCount != (int)q.size()) throw "error";
        if(!root){
            root = new Node(cus);
            return;
        }
        Node**p = &root;
        while(*p!=nullptr){
            if((*p)->cus->result > cus->result) {p = &(*p)->left;}
            else {p = &(*p)->right;}
        }
        *p = new Node(cus);
    }
    Node* remove(Node* p, const int& num){
        if(!p){ throw " error";}
        if(p->cus->result < num){
            p->right = remove(p->right, num);
            return p;
        }
        if(p->cus->result > num){
            p->left = remove(p->left, num);
            return p;
        }
        --nodeCount;
        if(!p->left){
            Node *tmp = p;
            if(root==p) root = p->right;
            p = p->right;
            delete tmp;
            return p;
        }
        if(!p->right){
            Node *tmp = p;
            if(root==p) root = p->left;
            p = p->left;
            delete tmp;
            return p;
        }
        Node *mlr = p->right;
        if(!mlr->left){
            mlr->left = p->left;
            if(root==p) root = mlr;
            delete p;
            return mlr;
        }
        while(mlr->left->left) mlr = mlr->left;
        Node *tmp = mlr->left;
        swap(p->cus, tmp->cus);
        mlr->left = tmp->right;
        delete tmp;
        return p;
    }
    Node* removeYNode(Node* p, int Y){
        while(Y--&&!q.empty()){
            int p = q.front();
            q.pop();
            root = remove(root, p);
            if((int)q.size() != nodeCount) throw "error";
        }
        if(nodeCount==0) root = nullptr;
        return root;
    }
    int combinationCount(){
        vector<vector<int>>COMBINATION(nodeCount+1, vector<int>(nodeCount+1, 1));
        for(int i=1;i<=nodeCount;++i){
            COMBINATION[i][0]  = 1;
            COMBINATION[i][1]  = i%MAXSIZE;
            COMBINATION[i][i-1]= i%MAXSIZE;
            COMBINATION[i][i]  = 1;
        }
        for(int i=2;i<=nodeCount;++i){
            for(int j=2;j<=nodeCount;++j){
                if(i>j+1) {
                    COMBINATION[i][j] = (COMBINATION[i-1][j-1] + COMBINATION[i-1][j]) % MAXSIZE;
                }
                else break;
            }
        }
        int size;
        return combinationCountHelper(COMBINATION, root, size);
    }
    int combinationCountHelper(vector<vector<int>>& COMBINATION, Node* p, int& size){
        if(!p){
            size = 0;
            return 1;
        }
        int lsize, rsize;
        int countLeft = combinationCountHelper(COMBINATION, p->left, lsize), countRight = combinationCountHelper(COMBINATION, p->right, rsize);
        size = lsize + rsize + 1;
        int ans = (countLeft * countRight) % MAXSIZE * COMBINATION[rsize + lsize][lsize] % MAXSIZE;
        return ans;
    }
    void KOKUSEN(){
        if(nodeCount!=(int)q.size()) throw "error";
        if(nodeCount==0) return;
        int cnt = combinationCount();
        root = removeYNode(root, cnt);
    }
    void printInorder(Node *t){
        if(!t) return;
        printInorder(t->left);
        cout << t->cus->result << "\n";
        printInorder(t->right);
    }
};

class Gojo{
public:
    vector<BST> table;
public:
    Gojo() {table = vector<BST>(MAXSIZE);}
    void LAPSE(customer* cus){
        if(!cus) throw "pass nullptr" ;
        int ID = cus->result % MAXSIZE + 1;
        table[ID-1].add(cus);
    }
    void KOKUSEN(){
        for(int i=0;i<MAXSIZE;++i){
            table[i].KOKUSEN();
        }
    }
    void LIMITLESS(int ID){
        ID = (ID-1) % MAXSIZE + 1;
        table[ID-1].printInorder(table[ID-1].root);
    }
};

void simulate(string filename){
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);
    cout.tie(NULL);
    ifstream ss(filename);
    string str, maxsize, name, num;
    string M;
    ss >> M;
    ss >> maxsize;
    MAXSIZE = stoi(maxsize);
    Gojo* G = new Gojo();
    Sukuna* S = new Sukuna();
    while(ss >> str){
        if(str == "LAPSE") 
        {
            ss >> name;
            set<char> s;
            for(const auto& ch : name) s.insert(ch);
            if(s.size() < 3) continue; 
            customer* cus = new customer(name);
            if(cus->result%2==1) G->LAPSE(cus);
            else S->LAPSE(cus);
    	}
    	else if(str == "KOKUSEN") 
    	{
            G->KOKUSEN();
		}
    	else if(str == "LIMITLESS") 
    	{
            ss >> num;
    		G->LIMITLESS(stoi(num));
		}
    	else if(str == "KEITEIKEN")
     	{   	
			ss >> num;
            S->KEITEIKEN(stoi(num));
    	}
    	else if(str == "CLEAVE") // DOMAIN_EXPANSION
    	{
			ss >> num;
            S->CLEAVE(stoi(num));
    	}
    	else if(str == "HAND")
    	{
            cout << Hand;
    	}
        else throw "error" ;
    }
    delete G;
    delete S;
}

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
bool comp3(heapNode* a, heapNode* b){
    if(a->size == b->size) return a->lastChange < b->lastChange;
    return a->size < b->size;
}
int binaryToInt(string& binary){
    if(binary.size()==0) return 0;
    int s = (int)binary.size(), result = 0;
    for(int i=0;i<s;++i){
        result *= 2;
        result += binary[i] - '0';
    }
    return result;
}