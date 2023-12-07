#include "_Gojo.cpp"

void simulate(string filename){
    int line = 1;
    Gojo *G;
    Sukuna *S;
    ifstream ss(filename);
    string str, maxsize, name, num;
    while(ss >> str){
        cout << line <<" ";
        if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize); 
            cout << str <<" "<<maxsize<<endl;
            G = new Gojo();
            S = new Sukuna();
    	}
        else if(str == "LAPSE") 
        {
            ss >> name;
            set<char> n;
            for(const auto& ch : name) n.insert(ch);
            if(n.size() < 3) continue;
            customer* cus = new customer(name);
            if(cus->result%2==0) G->LAPSE(cus);
            else S->LAPSE(cus);
            cout << str <<" "<<name<<endl;
    	}
    	else if(str == "KOKUSEN") 
    	{
            G->KOKUSEN();
            cout << str <<endl;
		}
    	else if(str == "LIMITLESS") 
    	{
            ss >> num;
    		G->LIMITLESS(stoi(num));
            cout << str <<" "<<num <<endl;
		}
    	else if(str == "KEITEIKEN") // UNLIMITED_VOID
     	{   	
			ss >> num;
            S->KEITEIKEN(stoi(num));
            cout << str <<" "<<num <<endl;
    	}
    	else if(str == "CLEAVE") // DOMAIN_EXPANSION
    	{
			ss >> num;
            S->CLEAVE(stoi(num));
            cout << str <<" "<<num <<endl;
    	}
    	else if(str == "HAND")
    	{
            cout << str << endl;
            cout << Hand;
    	}
        else throw "error" ;
		++line;
    }
    delete G;
    delete S;
}

int main(){
    string fileName = "input.txt";
    cout << "method balance incompleted" <<endl;
    simulate(fileName);
    return 0;
}
