#pragma once
namespace splay{
    template<typename T>
    class node{
        public:
            int size;
            int count;
            T val;
            node<T>* father;
            node<T>** son;
            node();
            ~node();
            node<T>* get_kth_node(int k);
    };
    template<typename T>
    class tree{
        public:
            node<T>* root;
            tree();
            ~tree();
            void connect(node<T>* x,node<T>* f,int k);
            int ident_right_son(node<T>* x);
            void push_up(node<T>* x);
            void rotate(node<T>* x);
            void splaying(node<T>* x,node<T>* y);
            void splaying(node<T>* x);
            void insert(node<T>*& now,T val,node<T>* father);
            void insert(T val);
            void del(node<T>* now,T val);
            void del(T val);
            node<T>* try_get_less_node(T val);
            node<T>* try_get_more_node(T val);
            node<T>* split(T l,T r);
            int get_value_rank(T val);
            T get_pre(T val);
            T get_nxt(T val);
    };
}