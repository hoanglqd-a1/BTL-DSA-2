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
            cout << root->cus->name <<endl;
            return;
        }
        Node**p = &root;
        while(*p!=nullptr){
            if((*p)->cus->result > cus->result) {p = &(*p)->left; cout << "left" <<endl;}
            else {cout << "right" <<endl; p = &(*p)->right;}
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
    long long combinationCountHelper(vector<vector<int>>& COMBINATION, Node* p, int& size){
        if(!p){
            size = 0;
            return 1ll;
        }
        int lsize, rsize;
        long long countLeft = combinationCountHelper(COMBINATION, p->left, lsize), countRight = combinationCountHelper(COMBINATION, p->right, rsize);
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
        int ID = cus->result % MAXSIZE + 1;
        // cout << "ID: " << ID <<endl;
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


// int main(){
//     customer *c6 = new customer("ItadoriYuji");
//     customer *c2 = new customer("MegumiFushiguro");
//     customer *c3 = new customer("KugisakiNobara");
//     customer *c4 = new customer("NanamiKento");
//     customer *c1 = new customer("ZenninMaki");
//     customer *c5 = new customer("YutaOkkotsu");
//     customer *c7 = new customer("GetoSuguru");
//     customer *c8 = new customer("HakariKinji");
//     customer *c9 = new customer("YukiTsukumo");
//     Gojo G;
//     c1->printCustomer();
//     G.LAPSE(c1);
//     c2->printCustomer();
//     G.LAPSE(c2);
//     c3->printCustomer();
//     G.LAPSE(c3);
//     c4->printCustomer();
//     G.LAPSE(c4);
//     c5->printCustomer();
//     G.LAPSE(c5);
//     c6->printCustomer();
//     G.LAPSE(c6);
//     c7->printCustomer();
//     G.LAPSE(c7);
//     c8->printCustomer();
//     G.LAPSE(c8);
//     c9->printCustomer();
//     G.LAPSE(c9);
//     G.LIMITLESS(0);
//     G.KOKUSEN();
//     G.LIMITLESS(0);
// }
