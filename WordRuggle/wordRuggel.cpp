#include <iostream>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define pii pair<int,int>
#define mss map<string,string> 
#define pss pair<string,string> 

using namespace std;

int trie[3000000][26][2];
int next=2;
vector<int> neigh[16];
int visited[16];
string board;
vector<pii> pres;
mss words;

bool cmp(pss p1,pss p2){
	return p1.first.length()>p2.first.length();
}
int chr2num(char c){
	int x = c-'a';
	if(x<0) return c-'A';
	return x;
}
void addWord(string str){
	int pres=1,prev=-1;
	for(int i=0;i<str.length();i++){
		char c = str[i],nc = chr2num(c);
		if(!(nc>=0 && nc<26)) continue;
		if(trie[pres][nc][1]==0){
			trie[pres][nc][1] = next;
			next+=1;
		}
		prev = pres;
		pres = trie[pres][chr2num(c)][1];
	}
	char c= str[str.length()-1];
	trie[prev][chr2num(c)][0] = 1;
}

bool ok(int i,int j){
	return 0<=i && 0<=j && i<4 && j<4;
}
int num(int i,int j){
	return i*4+j;
}
string num2string(int n){
	string ret="0";
	ret[0]+=n;
	return ret;
	
}
void clean(){
	string path="",str = "";
	for(int i=0;i<pres.size();i++){
		str+=board[pres[i].second];
		path= path+ ("("+(num2string(pres[i].second/4))+","+(num2string(pres[i].second %4))+") ");
	}
	if(words.find(str)==words.end()){
		words[str] = path;
	}
}

void recurse(int start){
	int trieNode = pres[pres.size()-1].first;
	int char1 = chr2num(board[start]);
	if(visited[start]) return;
	if(trie[trieNode][char1][0]) clean();
	if(trie[trieNode][char1][1]==0) return;
	
	visited[start]=1;
	
	
	for(int i=0;i<neigh[start].size();i++){
		int n = neigh[start][i];
		pres.push_back(pii(trie[trieNode][char1][1],n));
		recurse(n);
		pres.pop_back();
	}	
	visited[start]= 0;
}
int main(){
	cout<<sizeof(trie)<<"\n";
	fill(trie[0][0],trie[0][0]+sizeof(trie)/4,0);	
	fill(visited,visited+16,0);
	
	cout<< "start\n";
	
	ifstream inp("all");
	while(!inp.eof()){
		string word;inp>>word;
		if(word.length()<3) continue;
		addWord(word);
		assert(next<3000000);
	}
	
	cout<< "trie built\n";
	string tmp;  board="";
	cin>>tmp; board+=tmp;
	cin>>tmp; board+=tmp;
	cin>>tmp; board+=tmp;
	cin>>tmp; board+=tmp;
	
	for(int i=0;i<16;i++){
		int x = i/4,y=i%4;
		for (int xd =-1;xd<=1;xd++){
			for (int yd =-1;yd<=1;yd++){
				if(xd==0 and yd==0) continue;
				if(ok(x+xd,y+yd)) neigh[i].push_back(num(x+xd,y+yd));
			}
		}
	}
	
	for(int i=0;i<16;i++){
		pres.push_back(pii(1,i));
		recurse(i);
		pres.pop_back();
	}
	pair<string,string> array[words.size()];
	int p=0;
	for(std::map<string,string>::iterator it=words.begin();it!=words.end();it++){
		array[p++]= make_pair(it->first,it->second);
	}
	sort(array,array+p,cmp);
	for (int i = 0; i < p; i += 1){
		cout<<array[i].first<<" : "<<array[i].second<<"\n";
	}
	
}
