#pragma once
#include "trie.h"
#include<ctime>
#include<iostream>
char trie::edge_to_char(int idx){
    if(idx<=25){
        return 'A'+idx;
    }else{
        return 'a'+idx-26;
    }
}
int trie::char_to_edge(char c){
    if(c>='A'&&c<='Z'){
        return c-'A';
    }else{
        return c-'a'+26;
    }
}
trie::node::node(){
    this->father=0;
    this->son=new node*[52];
    for(int i=0;i<52;i++){
        this->son[i]=0;
    }
    this->father_edge=-1;
    this->is_word_end=false;
    this->word_len=0;
    this->word_count=0;
    this->word_len_splay=new splay::tree<word_len_splay_data>();
}
trie::node::~node(){
    delete this->father;
    for(int i=0;i<52;i++){
        if(this->son[i]!=0){
            delete this->son[i];
        }
    }
    delete[] this->son;
    delete this->word_len_splay;
}
trie::node* trie::node::find(std::string prefix){
    node* now=this;
    int sz=prefix.size();
    for(int i=0;i<sz;i++){
        int edge=char_to_edge(prefix[i]);
        if(now->son[edge]==0){
            return 0;
        }
        now=now->son[edge];
    }
    return now;
}
std::string trie::node::jump_to_string(){
    std::string ret;
    node* now=this;
    while(now->father!=0){
        ret.push_back(edge_to_char(now->father_edge));
        now=now->father;
    }
    int l=0,r=ret.size()-1;
    while(l<r){
        char t=ret[l];
        ret[l]=ret[r];
        ret[r]=t;
        l++,r--;
    }
    return ret;
}
std::vector<trie::node*> trie::node::random_choose(int l,int r,int cnt){
    srand(time(0));
    std::vector<node*> ret;
    if(l>r){    
        return ret;
    }
    word_len_splay_data lft_val;
    lft_val.word_len=l;
    word_len_splay_data rgt_val;
    rgt_val.word_len=r;
    splay::node<word_len_splay_data>* nd=this->word_len_splay->split(lft_val,rgt_val);
    if(nd==0){
        return ret;
    }
    if(nd->size<=cnt){
        for(int i=1;i<=nd->size;i++){
            ret.push_back(nd->get_kth_node(i)->val.trie_node);
        }
    }else{
        int las=0;
        for(int i=1;i<=cnt;i++){
            int mx_rgt=nd->size-(cnt-i+1)+1;
            int now=las+rand()%(mx_rgt-las)+1;
            ret.push_back(nd->get_kth_node(now)->val.trie_node);
            las=now;
        }
    }
    return ret;
}
trie::tree::tree(){
    this->root=new node();
    this->nodes_to_chinese=new std::map<node*,std::string>();
}
trie::tree::~tree(){
    delete this->root;
    delete this->nodes_to_chinese;
}
trie::node* trie::tree::insert(node* now,std::string& english,int idx){
    if(now==0){
        return 0;
    }
    if(idx>=english.size()){
        if(now->is_word_end){
            return 0;
        }
        now->is_word_end=true;
        now->word_len=english.size();
        node::word_len_splay_data val_data;
        val_data.trie_node=now;
        val_data.word_len=now->word_len;
        now->word_len_splay->insert(val_data);
        now->word_count++;
        return now;
    }
    int edge=char_to_edge(english[idx]);
    if(now->son[edge]==0){
        now->son[edge]=new node();
        now->son[edge]->father=now;
        now->son[edge]->father_edge=edge;
    }
    node* t=this->insert(now->son[edge],english,idx+1);
    if(t!=0){
        now->word_count++;
        node::word_len_splay_data val_data;
        val_data.trie_node=t;
        val_data.word_len=t->word_len;
        now->word_len_splay->insert(val_data);
    }
    return t;
}
void trie::tree::insert(std::string english,std::string chinese){
    for(int i=0,sz=english.size();i<sz;i++){
        if(!(english[i]>='a'&&english[i]<='z'||english[i]>='A'&&english[i]<='Z')){
            return;
        }
    }
    node* t=this->insert(this->root,english,0);
    if(t!=0){
        (*this->nodes_to_chinese)[t]=chinese;
    }
}
std::vector<trie::result_pair> trie::tree::random_choose(std::string prefix,int l,int r,int cnt){
    node* nd=this->root->find(prefix);
    std::vector<trie::result_pair> ret;
    if(nd==0){
        return ret;
    }
    std::vector<node*> choose_nd=nd->random_choose(l,r,cnt);
    for(int i=0,sz=choose_nd.size();i<sz;i++){
        result_pair x={choose_nd[i]->jump_to_string(),(*this->nodes_to_chinese)[choose_nd[i]]};
        ret.push_back(x);
    }
    return ret;
}
std::vector<trie::result_pair> trie::tree::random_choose(int l,int r,int cnt){
    node* nd=this->root;
    std::vector<trie::result_pair> ret;
    std::vector<node*> choose_nd=nd->random_choose(l,r,cnt);
    for(int i=0,sz=choose_nd.size();i<sz;i++){
        trie::result_pair x={choose_nd[i]->jump_to_string(),(*this->nodes_to_chinese)[choose_nd[i]]};
        ret.push_back(x);
    }
    return ret;
}
int trie::tree::word_count(std::string prefix,int l,int r){
    node* nd=this->root->find(prefix);
    if(nd==0){
        return 0;
    }
    node::word_len_splay_data lft_val;
    lft_val.word_len=l;
    node::word_len_splay_data rgt_val;
    rgt_val.word_len=r;
    splay::node<node::word_len_splay_data>* x=nd->word_len_splay->split(lft_val,rgt_val);
    if(x==0){
        return 0;
    }
    return x->size;
}
int trie::tree::word_count(int l,int r){
    node* nd=this->root;
    node::word_len_splay_data lft_val;
    lft_val.word_len=l;
    node::word_len_splay_data rgt_val;
    rgt_val.word_len=r;
    splay::node<node::word_len_splay_data>* x=nd->word_len_splay->split(lft_val,rgt_val);
    if(x==0){
        return 0;
    }
    return x->size;
}