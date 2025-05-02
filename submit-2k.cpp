#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

string getInitPolicy(const unsigned int &k){
  
  string pi= "("+to_string(k - 1) +")(0)";
  
  return pi;
}

vector<int> parsePolicy(string policy){

	vector<int> action_values;
	size_t pos=0;
	string s=policy;
	string a;
	
	for(int i=0;i<2;i++) {
		pos = s.find(")");
        a = s.substr(1, pos);
        action_values.push_back(stoi(a));
        s.erase(0, pos + 1);
	}
	
	return action_values; 
}

string getNextPolicy(const string &pi){
	
  vector<int> action_values =parsePolicy(pi);
  
  int i =action_values[0];
  int j = action_values[1];

  int i1 = -1;
  int j1 = -1;

  if(i > j){
    i1 = i;
    j1 = j + 1;
  }
  else if(i == j){
    i1 = 0;
    j1 = j;
  }
  else if(i < j - 1){
    i1 = i + 1;
    j1 = j;
  }
  else{//i == j - 1
    i1 = i;
    j1 = 0;
  }

  string pi1= "("+ to_string(i1) +")(" + to_string(j1) + ")";
  
  return pi1;
}

void printVector(vector<double> vec, const unsigned int &n){
  for(int i = 0; i < n; i++){
    cout << vec[i] << "\t";
  }
  cout << "\n";
}

void setParameters(const unsigned int &k, vector<double> &lambda1, vector<double> &lambda2, vector<double> &mu1, vector<double> &mu2, const double &gamma){

  lambda1.resize(k, 0);
  lambda2.resize(k, 0);
  for(int i = 0; i < k; i++){
    lambda1[i] = (i + 1.0) / (k + 1.0);
    lambda2[i] = lambda1[i];
  }

  mu1.resize(k, 0);
  mu2.resize(k, 0);
  mu1[0] = 0;
  mu2[0] = 0;

  for(int a = 0; a < k - 1; a++){

    double t1 = 0;
    double t2 = 0;
    double t3 = 0;

    //Set mu2[a + 1]
    t1 = mu2[0] * (1.0 + gamma * (1.0 - lambda1[0] - lambda2[a + 1]));
    t2 = gamma * (lambda2[a + 1] - lambda2[0]) * mu1[a];
    t3 = 1.0 + gamma * (1 - lambda1[0] - lambda2[0]);
    mu2[a + 1] = (t1 + t2) / t3;

    for(int i = 0; i <= a - 1; i++){
      t1 = mu1[i + 1] - mu1[i];
      t2 = 1.0 + gamma * (1.0 - lambda1[i] - lambda1[a + 1]);
      t3 = gamma * (lambda1[i + 1] - lambda1[i]);
      double temp = mu1[i] + (t1 * t2) / t3;
      if(temp < mu2[a + 1]){
	        mu2[a + 1] = temp;
      }
    }
    mu2[a + 1] -= 1.0;

    //Set mu1[a + 1]
    t1 = mu1[0] * (1.0 + gamma * (1.0 - lambda1[a + 1] - lambda2[a + 1]));
    t2 = gamma * (lambda1[a + 1] - lambda1[0]) * mu2[a + 1];
    t3 = 1.0 + gamma * (1 - lambda1[0] - lambda2[a + 1]);
    mu1[a + 1] = (t1 + t2) / t3;

    for(int j = 0; j <= a; j++){
      t1 = mu2[j + 1] - mu2[j];
      t2 = 1.0 + gamma * (1.0 - lambda1[a + 1] - lambda2[j]);
      t3 = gamma * (lambda2[j + 1] - lambda2[j]);
      double temp = mu2[j] + (t1 * t2) / t3;
      if(temp < mu1[a + 1]){
	mu1[a + 1] = temp;
      }
    }
    mu1[a + 1] -= 1.0;
  }

  cout << "gamma: " << gamma << "\n";
  cout << "lambda1: "; printVector(lambda1, k);
  cout << "lambda2: "; printVector(lambda2, k);
  cout << "mu1: "; printVector(mu1, k);
  cout << "mu2: "; printVector(mu2, k);
}

vector<double> valueFunction(string policy, const vector<double> &lambda1, const vector<double> &mu1, const vector<double> &lambda2, const vector<double> &mu2, const double &gamma){

  vector<double> v;
  v.resize(2, 0);
  
  vector<int> action_values =parsePolicy(policy);

  unsigned int i =action_values[0];
  unsigned int j =action_values[1];

  double nr1 = 0;
  double nr2 = 0;
  double dr;
  
  nr1 = mu1[i] * (1.0 - gamma * lambda2[j]) + mu2[j] * gamma * (1.0 - lambda1[i]);
  nr2 = mu2[j] * (1.0 - gamma * lambda1[i]) + mu1[i] * gamma * (1.0 - lambda2[j]);
  dr = (1.0 - gamma) * (1.0 + gamma * (1 - lambda1[i] - lambda2[j]));
    
  v[0] = nr1 / dr;
  v[1] = nr2 / dr;
  
  return v;
}

int main(){

  int  k = 5;
  vector<double> lambda1;
  vector<double> lambda2;
  vector<double> mu1;
  vector<double> mu2;

  double gamma = 0.9;
  setParameters(k, lambda1, lambda2, mu1, mu2, gamma);

  vector<double> val;
  
  
  string pi = getInitPolicy(k);
  val = valueFunction(pi, lambda1, mu1, lambda2, mu2, gamma);
  cout << pi << "\t";
  printVector(val, 2);
  
  for(int i = 1; i < k * k; i++){
    pi = getNextPolicy(pi);
    val = valueFunction(pi, lambda1, mu1, lambda2, mu2, gamma);
    cout << pi << "\t";
    printVector(val, 2);
  }
  
  return 0;
}
