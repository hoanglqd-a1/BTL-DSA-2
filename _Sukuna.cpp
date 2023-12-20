#include "_AVLHuffTree.cpp"

class heapNode;

bool comp3(heapNode* a, heapNode* b);

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

bool comp3(heapNode* a, heapNode* b){
    if(a->size == b->size) return a->lastChange < b->lastChange;
    return a->size < b->size;
}
