#include<iostream>
#include<set>
#include<string>

using namespace std;

struct comp{
  bool operator() (const string &a, const string &b) const{
    if(a.length() == b.length()){
      return a < b;
    }

    return a.length() < b.length();
  }
};

void aux(char m[20][20], string s, set<string,comp> &out, short x, short y, short n){
  if(x>=n || y>=n || y<0 || x<0) return;
  if(s.length()>0 && m[x][y] <= s.back()) return;

  s.push_back(m[x][y]);

  if(s.length()>2) out.insert(s);

  aux(m,s,out,x+1,y,n);
  aux(m,s,out,x-1,y,n);
  aux(m,s,out,x,y+1,n);
  aux(m,s,out,x,y-1,n);
  aux(m,s,out,x+1,y+1,n);
  aux(m,s,out,x-1,y-1,n);
  aux(m,s,out,x+1,y-1,n);
  aux(m,s,out,x-1,y+1,n);
}

void search(char matrix[20][20], short n){
  set<string,comp> out;
  string s ("");
  for(short i=0; i<n; i++){
    for(short j=0; j<n; j++){
      aux(matrix, s, out, i, j, n);
    }
  }

  for(auto i=out.begin(); i!=out.end(); i++){
    cout<<*i<<endl;
  }
  return;
}

void print(char matrix[20][20], short n){
  for(short i=0; i<n; i++){
    for(short j=0; j<n; j++){
      cout<<matrix[i][j];
    }
  }
  return;
}

int main(void){
  int cases;
  short n;
  char matrix[20][20];
  cin>>cases;
  while(cases--){
    cin>>n;
    for(short i=0; i<n; i++){
      for(short j=0; j<n; j++){
        cin>>matrix[i][j];
      }
    }
    search(matrix, n);

    if(cases>0) cout<<endl;
  }
  return 0;
}
