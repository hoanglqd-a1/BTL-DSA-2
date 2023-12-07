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
            cout << ID <<'-'<<st.top()->result<<'-'<<st.top()->name<<"\n";
        }
        while(!st.empty()){
            customer *tmp = st.top();
            st.pop();
            LIST.push_back(tmp);
            ++size;
        }
    }
    bool operator > (const heapNode& X){
        if(this->size==X.size) return this->lastChange < X.lastChange;
        return this->size > X.size;
    }
};

class minHeap{
public:
    vector<heapNode*> AREA;
    int size=0, changeCnt = 0;
    unordered_map<int, int> mp;
public:
    void printHeap(){
        cout << "printHeap: "<<endl;
        cout << "heap size: " <<size <<endl;
        for(int i=0;i<MAXSIZE;++i){
            cout << "ID: "<<AREA[i]->ID<<" position: "<<mp[AREA[i]->ID]<<endl;
            cout << "size: "<< AREA[i]->size <<endl;
        }
    }
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
        heapNode* tmp = std::move(AREA[pos]);
        while(pos){
            int parent = (pos-1)/2;
            if(*AREA[parent] > *tmp){
                mp[AREA[parent]->ID] = pos;
                AREA[pos] = std::move(AREA[parent]);
                pos = parent;
            }
            else break;
        }
        mp[tmp->ID] = pos;
        AREA[pos] = std::move(tmp);
    }
    void heapDown(int pos){
        if(pos>=size) throw "out of range";
        heapNode* tmp = std::move(AREA[pos]);
        while(pos*2+1<size){
            int left = pos*2+1, right = left+1;
            if(*AREA[left] > *tmp && (right==size||*AREA[right] > *tmp)) break;
            if(right == size || AREA[left] > AREA[right]){
                mp[AREA[left]->ID] = pos;
                AREA[pos] = std::move(AREA[left]);
                pos = left;
            }
            else{
                mp[AREA[right]->ID] = pos;
                AREA[pos] = std::move(AREA[right]);
                pos = right;
            }
        }
        mp[tmp->ID] = pos;
        AREA[pos] = std::move(tmp);
    }
    void moveUp(int pos){
        int parent = (pos-1)/2;
        swap(mp[AREA[parent]->ID], mp[AREA[pos]->ID]);
        swap(AREA[parent], AREA[pos]);
    }
    void add(customer* cus){
        if(!cus) throw "pass nullptr";
        int ID = cus->result % MAXSIZE + 1;
        int pos = mp[ID];
        AREA[mp[ID]]->lastChange = changeCnt;
        if(AREA[mp[ID]]->add(cus)){
            swap(mp[ID], mp[AREA[size]->ID]);
            swap(AREA[size], AREA[pos]);
            ++size;
            heapUp(mp[ID]);
        }
        heapDown(mp[ID]);
        ++changeCnt;
    }
    void remove(int num){
        num = min(num, this->size);
        vector<heapNode*> v;
        for(int i=0;i<size;++i){
            v.push_back(AREA[i]);
        }
        string removedCustomer{};
        sort(v.begin(), v.end(), comp3);
        for(int i=0;i<num&&0<size;++i){
            for(int j=0;j<num;++j){
                if(v[i]->size==0) break;
                customer* tmp = v[i]->popFront();
                removedCustomer += (to_string(tmp->result) + '-' + to_string(v[i]->ID) + ' '+ tmp->name + "\n");
                delete tmp;
            }
            AREA[mp[v[i]->ID]]->lastChange = changeCnt;
            ++changeCnt;
            if(v[i]->size==0){
                --size;
                int pos = mp[v[i]->ID];
                swap(mp[AREA[pos]->ID], mp[AREA[size]->ID]);
                swap(AREA[pos], AREA[size]);
                heapDown(mp[pos]);
                heapUp(mp[pos]);
                // cout << AREA[size]->ID <<" "<<mp[AREA[size]->ID] <<endl;
                // cout << AREA[pos]->ID<<" "<<mp[AREA[pos]->ID] <<endl;
            }
            else{
                int j=i;
                while(j > 0 && AREA[j]->size >= AREA[(j-1)/2]->size){
                    moveUp(j);
                    j = (j-1)/2;
                }
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

// int main(){
//     Sukuna* S = new Sukuna();
//     vector<customer*> v;
//     v.push_back(new customer("Kenjaku"));
//     v.push_back(new customer("Jogo"));
//     v.push_back(new customer("Hanami"));
//     v.push_back(new customer("Mahoraga"));
//     v.push_back(new customer("ZenninNaoya"));
//     v.push_back(new customer("Mahito"));
//     v.push_back(new customer("Uraume"));
//     v.push_back(new customer("OgiZennin"));
//     v.push_back(new customer("RyomenSukuna"));
//     for(int i=0;i<v.size();++i){
//         S->LAPSE(v[i]); 
//     }
//     S->restaurant.printHeap();
//     S->KEITEIKEN(11); 
//     S->CLEAVE(7);
//     delete S;
// }