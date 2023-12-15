#include "_Gojo.cpp"

void simulate(string filename){
    int line = 1;
    Gojo *G;
    Sukuna *S;
    ifstream ss(filename);
    string str, maxsize, name, num; 
    while(ss >> str){
        // cout << line <<" ";
        if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize); 
            // cout << str <<" "<<maxsize<<endl;
            G = new Gojo();
            S = new Sukuna();
    	}
        else if(str == "LAPSE") 
        {
            ss >> name;
            // cout << str <<" "<<name <<endl;
            unordered_set<char> s;
            for(const auto& ch : name) s.insert(ch);
            if(s.size() < 3) continue; 
            customer* cus = new customer(name);
            if(cus->result%2==1) G->LAPSE(cus);
            else S->LAPSE(cus);
    	}
    	else if(str == "KOKUSEN") 
    	{
            // cout << str <<endl;
            G->KOKUSEN();
		}
    	else if(str == "LIMITLESS") 
    	{
            ss >> num;
            // cout << str <<" "<<num <<endl;
    		G->LIMITLESS(stoi(num));
		}
    	else if(str == "KEITEIKEN") // UNLIMITED_VOID
     	{   	
			ss >> num;
            // cout << str <<" "<<num <<endl;
            S->KEITEIKEN(stoi(num));
    	}
    	else if(str == "CLEAVE") // DOMAIN_EXPANSION
    	{
			ss >> num;
            // cout << str <<" "<<num <<endl;
            S->CLEAVE(stoi(num));
    	}
    	else if(str == "HAND")
    	{
            // cout << str << endl;
            cout << Hand;
    	}
        else throw "error" ;
		++line;
    }
    delete G;
    delete S;
}

int main(){
    time_t start, end;
    time(&start);
    ios_base::sync_with_stdio(false); 
    string fileName = "input.txt";
    simulate(fileName);
    time(&end);
    // cout << "time taken by program is: " << (double)(end - start) << " sec" << endl;
    return 0;
}
