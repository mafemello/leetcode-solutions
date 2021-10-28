#include<iostream>
#include<stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

bool funct(double md, int n, vector<int> &v){
  int count = 0;
  double last=-1e6;
  for(auto i=v.begin(); i!=v.end(); i++){
    if((*i-last)>md){
      count++;
      last=*i+md;
    }
  }
  //cout<<count<<"@"<<endl;
  return (count<=n);
}

double search(vector<int> &v, int n){
  double hi =1e6, lo = 0;
  double md;
  while(hi-lo>1e-6){
    md =lo +(hi-lo)/2.0;
    //cout<<lo<<"|"<<md<<"|"<<hi<<endl;
    if(funct(md, n, v)) hi = md;
    else lo = md;
  }

  return hi;
}

int main(void){
  int t, n, m, h;
  vector<int> houses;

  cin>>t;
  while(t--){
    cin>>n>>m;
    for(int i=0; i<m; i++){
      cin>>h;
      houses.push_back(h);
    }
    sort(houses.begin(),houses.end());
    printf("%.1lf\n", search(houses,n));
    houses.clear();
  }

  return 0;
}
