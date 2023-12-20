#include "_Gojo.cpp"

void simulate(string filename){
    Gojo *G;
    Sukuna *S;
    ifstream ss(filename);
    string str, maxsize, name, num;
    while(ss >> str){
        if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize); 
            G = new Gojo();
            S = new Sukuna();
    	}
        else if(str == "LAPSE") 
        {
            ss >> name;
            unordered_set<char> s;
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

int main(){
    ios_base::sync_with_stdio(false); 
    string fileName = "input.txt";
    simulate(fileName);
    return 0;
}
