#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <cassert>

using namespace std;

// number of states
int n;

// max number of actions
int K;

//Assumes k is even, r is odd.
vector<string> getPolicies(unsigned int n, unsigned int r, vector<int> &k_list ){

  vector<string> pol;
  pol.resize(0);
  int k=k_list[n-1];
  assert(r<k && "r >= k");
  if(n == 1){
    for(unsigned int i = 0; i <= k - 2; i += 2){
      pol.push_back("("+to_string(i)+")");
    }
    pol.push_back("("+to_string(r)+")");
  }
  else{

    for(int row = 0; row <= k - 2; row += 2){
      vector<string> smallerPol = getPolicies(n - 1, row + 1, k_list);
      int l = smallerPol.size();
      for(int col = 1; col <= l; col++){
		stringstream ss;
		ss << "("+to_string(row)+")";
		ss << smallerPol[col - 1];
		pol.push_back(ss.str());
      }
      stringstream ss;
      ss << "(" + to_string(((row != (k - 2))? (row + 2) : r)) + ")";
      ss << smallerPol[l - 1];
      pol.push_back(ss.str());
    }
    
    stringstream ss;
    ss <<"(" + to_string(r) + ")";
    for(int i = 0;i < n - 1; i++){
      ss << "(0)";
    }
    pol.push_back(ss.str());
  }
  
  return pol;
}

double R(unsigned int state, unsigned int action){
  
  double rew = 0;
  if(action % 2 == 0){
    rew = action * pow(K, state);
  }
  else{
    rew = (action - 1) * pow(K, state + 1) + (K - 1) * pow(K, state);
  }
  
  return rew;
}

int T(unsigned int state, unsigned int action){
  
  int nextState = -1;
  if(action % 2 == 0){
    nextState = state + 1;
  }
  else{
    nextState = state + 2;
  }
  if(nextState >= n){
    nextState = -1;
  }
  
  return nextState;
}

vector<int> parsePolicy(string policy){

	vector<int> action_values;
	size_t pos=0;
	string s=policy;
	string a;

	for(int i=0;i<n;i++) {
		pos = s.find(")");
        a = s.substr(1, pos);
        action_values.push_back(stoi(a));
        s.erase(0, pos + 1);
	}
  
	return action_values; 
}


vector<double> valueFunction(string policy){
  
  vector<double> v;
  v.resize(n, 0);
  
  vector<int> action_values =parsePolicy(policy);
  
  for(int s = n - 1; s >= 0; s--){
    unsigned int a = action_values[n - 1 - s] ;
    
    double val = R(s, a);
    int ns = T(s, a);
    if(ns != -1){
      val += v[ns];
    }

    v[s] = val;
  }
  
  return v;
}

void printVector(vector<double> vec){
  for(int i = 0; i < n; i++){
    cout << vec[i] << "\t";
  }
  cout << "\n";
}

int main(){
	
  n=3;
	
   // for uniform actions
  vector<int> k_list(n,12);
  // for non uniform actions: should be non increasing 
  // k_list[0]=16;
  // k_list[1]=14;
  K=k_list[0];

  unsigned int r = 1;

  vector<string> policies = getPolicies(n, r, k_list);
  
  cout<<"number of policies = "<<policies.size()<<endl;

  vector<double> val, valNext;
  val = valueFunction(policies[0]);
  cout << policies[0] << "\n";
  printVector(val);

  for(unsigned int i = 1; i < policies.size(); i++){
    val = valueFunction(policies[i - 1]);
    valNext = valueFunction(policies[i]);
    bool allge = true;
    bool oneg = false;
    for(int j = 0; j < n; j++){
      if(valNext[j] < val[j]){
	      allge = false;
      }
      if(valNext[j] > val[j]){
	      oneg = true;
      }
    }

    cout << policies[i] << "\n";
    printVector(valNext);

    if(allge && oneg){
    	
      cout << "Increasing.\n";
      
    }
    else{
      cout << "Decreasing!\n";
      assert(false && "Decreasing");
      break;
    }

  }
  
  return 0;
}
