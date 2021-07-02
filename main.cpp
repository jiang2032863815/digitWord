#include<bits/stdc++.h>
#include "trie/trie.hpp"
using namespace std;
void print_help(){
    cout<<"find-prefix prefix l r max_count; 指定前缀，长度范围随机返回单词"<<endl;
    cout<<"find l r max_count; 指定长度范围随机返回单词"<<endl;
    cout<<"prefix-count prefix l r; 指定前缀，长度范围返回单词数量"<<endl;
    cout<<"count l r; 指定长度范围返回单词数量"<<endl;
    cout<<"help; 帮助"<<endl;
    cout<<"exit; 退出"<<endl;
}
int main(){
    ifstream fin("words-en-zh.txt");
    string english,chinese;
    trie::tree* tr=new trie::tree();
    int cnt=0;
    cout<<"正在读入数据并构建单词系统..."<<endl;
    while(fin>>english>>chinese){
        tr->insert(english,chinese);
    }
    fin.close();
    cout<<"单词系统共计"<<tr->root->word_count<<"个单词"<<endl;
    print_help();
    while(true){
        cout<<"wordSystem>";
        string opt;
        cin>>opt;
        if(opt=="find-prefix"){
            string prefix;
            int l,r,cnt;
            cin>>prefix>>l>>r>>cnt;
            vector<trie::result_pair> v=tr->random_choose(prefix,l,r,cnt);
            for(int i=0;i<v.size();i++){
                cout<<v[i].chinese<<" "<<v[i].english<<endl;
            }
        }else if(opt=="find"){
            int l,r,cnt;
            cin>>l>>r>>cnt;
            vector<trie::result_pair> v=tr->random_choose(l,r,cnt);
            for(int i=0;i<v.size();i++){
                cout<<v[i].chinese<<" "<<v[i].english<<endl;
            }
        }else if(opt=="prefix-count"){
            string prefix;
            int l,r;
            cin>>prefix>>l>>r;
            cout<<tr->word_count(prefix,l,r)<<endl;
        }else if(opt=="count"){
            int l,r;
            cin>>l>>r;
            cout<<tr->word_count(l,r)<<endl;
        }else if(opt=="exit"){
            cout<<"bye"<<endl;
            break;
        }else if(opt=="help"){
            print_help();
        }else{
            cout<<"未知命令"<<endl;
        }
    }
    return 0;
}