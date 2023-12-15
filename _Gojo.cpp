#include "_Sukuna.cpp"

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
public:
    ~BST(){
        int all = nodeCount;
        removeYNode(root, all);
    }
    void add(customer* cus){
        ++nodeCount;
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
    Node* remove(Node* p, customer* cus){
        if(!p) return nullptr;
        --nodeCount;
        if(p->cus->result < cus->result){
            p->right = remove(p->right, cus);
            return p;
        }
        if(p->cus->result > cus->result){
            p->left = remove(p->left, cus);
            return p;
        }
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
        p->cus = std::move(tmp->cus);
        mlr->left = tmp->right;
        delete tmp;
        return p;
    }
    Node* removeYNode(Node* p, int& Y){
        if(!p) return nullptr;
        p->left = removeYNode(p->left, Y);
        p->right = removeYNode(p->right, Y);
        if(Y==0) return p;
        delete p;
        --Y;
        --nodeCount;
        return nullptr;
    }
    int combinationCount(){
        vector<vector<int>>COMBINATION(nodeCount+1, vector<int>(nodeCount+1, 1));
        for(int i=1;i<=nodeCount;++i){
            COMBINATION[i][1] = i%MAXSIZE;
            COMBINATION[i][i-1]=i%MAXSIZE;
        }
        for(int i=2;i<=nodeCount;++i){
            for(int j=2;j<=nodeCount;++j){
                if(i>j-1) COMBINATION[i][j] = (COMBINATION[i-1][j-1] + COMBINATION[i-1][j]) % MAXSIZE;
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
        return (countLeft * countRight) % MAXSIZE * COMBINATION[rsize + lsize][lsize] % MAXSIZE;
    }
    void KOKUSEN(){
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
        // cout << "Gojo " << ID <<" "<<cus->result <<" "<<table[ID-1].nodeCount<<endl;
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

