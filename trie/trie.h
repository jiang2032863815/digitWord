#pragma once
#include<string>
#include "../splay/splay.hpp"
#include<map>
#include<vector>
namespace trie{
    char edge_to_char(int idx);
    int char_to_edge(char c);
    struct result_pair{
        std::string english;
        std::string chinese;
    };
    class node{
        public:
            struct word_len_splay_data{
                node* trie_node;
                int word_len;
                bool operator < (const word_len_splay_data& other)const{
                    return word_len<other.word_len;
                }
                bool operator == (const word_len_splay_data& other)const{
                    return false;
                }
                bool operator > (const word_len_splay_data& other)const{
                    return word_len>other.word_len;
                }
            };
            node* father;
            node** son;
            int father_edge;
            bool is_word_end;
            int word_len;
            int word_count;
            splay::tree<word_len_splay_data>* word_len_splay;
            node();
            ~node();
            node* find(std::string prefix);
            std::string jump_to_string();
            std::vector<node*> random_choose(int l,int r,int cnt);
    };
    class tree{
        public:
            node* root;
            node* insert(node* now,std::string& english,int idx);
            std::map<node*,std::string>* nodes_to_chinese;
            void insert(std::string english,std::string chinese);
            std::vector<result_pair> random_choose(std::string prefix,int l,int r,int cnt);
            std::vector<result_pair> random_choose(int l,int r,int cnt);
            int word_count(std::string prefix,int l,int r);
            int word_count(int l,int r);
            tree();
            ~tree();
    };
}