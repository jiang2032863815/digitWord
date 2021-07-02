#pragma once
#include "splay.h"
template<typename T>
splay::node<T>::node(){
    this->size=1;
    this->count=1;
    this->father=0;
    this->son=new node<T>*[2];
    this->son[0]=0;
    this->son[1]=0;
}
template<typename T>
splay::node<T>::~node(){
    delete this->father;
    if(this->son[0]!=0){
        delete this->son[0];
    }
    if(this->son[1]!=0){
        delete this->son[1];
    }
    delete[] this->son;
}
template<typename T>
splay::node<T>* splay::node<T>::get_kth_node(int k){
    if(k<=0||k>this->size){
        return 0;
    }
    node<T>* now=this;
    while(now!=0){
        if(now->son[0]==0){
            if(k<=now->count){
                return now;
            }
            k-=now->count;
            now=now->son[1];
        }else{
            if(k>=now->son[0]->size+1&&k<=now->son[0]->size+now->count){
                return now;
            }
            if(k<=now->son[0]->size){
                now=now->son[0];
            }else{
                k-=now->son[0]->size+now->count;
                now=now->son[1];
            }
        }
    }
    return 0;
}
template<typename T>
splay::tree<T>::tree(){
    this->root=0;
}
template<typename T>
splay::tree<T>::~tree(){
    delete this->root;
}
template<typename T>
void splay::tree<T>::connect(node<T>* x,node<T>* f,int k){
    if(x!=0){
        x->father=f;
    }
    if(f!=0){
        f->son[k]=x;
    }
}
template<typename T>
int splay::tree<T>::ident_right_son(node<T>* x){
    if(x==0){
        return 0;
    }
    if(x->father==0){
        return 0;
    }
    return x->father->son[1]==x;
}
template<typename T>
void splay::tree<T>::push_up(node<T>* x){
    if(x==0){
        return;
    }
    x->size=x->count;
    if(x->son[0]!=0){
        x->size+=x->son[0]->size;
    }
    if(x->son[1]!=0){
        x->size+=x->son[1]->size;
    }
}
template<typename T>
void splay::tree<T>::rotate(node<T>* x){
    if(x==0){
        return;
    }
    if(x->father==0){
        return;
    }
    node<T>* f=x->father;
    node<T>* ff=f->father;
    int xk=this->ident_right_son(x);
    int fk=this->ident_right_son(f);
    this->connect(x->son[xk^1],f,xk);
    this->connect(f,x,xk^1);
    this->connect(x,ff,fk);
    this->push_up(f);
    this->push_up(x);
}
template<typename T>
void splay::tree<T>::splaying(node<T>* x,node<T>* y){
    if(x==0||y==0){
        return;
    }
    while(x->father!=y){
        node<T>* f=x->father,*ff=f->father;
        if(ff!=y){
            if(this->ident_right_son(x)==this->ident_right_son(f)){
                this->rotate(f);
            }else{
                this->rotate(x);
            }
        }
        this->rotate(x);
    }
}
template<typename T>
void splay::tree<T>::splaying(node<T>* x){
    if(x==0){
        return;
    }
    this->root=x;
    while(x->father!=0){
        node<T>* f=x->father,*ff=f->father;
        if(ff!=0){
            if(this->ident_right_son(x)==this->ident_right_son(f)){
                this->rotate(f);
            }else{
                this->rotate(x);
            }
        }
        this->rotate(x);
    }
}
template<typename T>
void splay::tree<T>::insert(node<T>*& now,T val,node<T>* father){
    if(now==0){
        now=new node<T>();
        now->val=val;
        now->father=father;
        this->splaying(now);
        return;
    }
    if(now->val==val){
        now->count++;
        now->size++;
        this->splaying(now);
        return;
    }
    if(val<now->val){
        this->insert(now->son[0],val,now);
    }else{
        this->insert(now->son[1],val,now);
    }
}
template<typename T>
void splay::tree<T>::insert(T val){
    this->insert(this->root,val,0);
}
template<typename T>
void splay::tree<T>::del(node<T>* now,T val){
    if(now==0){
        return;
    }
    if(now->val==val){
        if(now->count>1){
            now->count--;
            now->size--;
            this->splaying(now);
        }else{
            this->splaying(now);
            if(now->son[1]==0){
                this->root=now->son[0];
                if(this->root!=0){
                    this->root->father=0;
                }
                now->father=0;
                now->son[0]=0;
                now->son[1]=0;
                delete now;
            }else{
                node<T>* t=now->son[1];
                while(t->son[0]!=0){
                    t=t->son[0];
                }
                this->splaying(t,now);
                this->connect(now->son[0],t,0);
                this->root=t;
                this->root->father=0;
                this->push_up(this->root);
                now->father=0;
                now->son[0]=0;
                now->son[1]=0;
                delete now;
            }
        }
        return;
    }
    if(val<now->val){
        this->del(now->son[0],val);
    }else{
        this->del(now->son[1],val);
    }
}
template<typename T>
splay::node<T>* splay::tree<T>::try_get_less_node(T val){
    node<T>* now=this->root;
    node<T>* ret=0;
    while(now!=0){
        if(!(now->val<val)&&!(now->val>val)){
            if(ret==0){
                ret=now;
            }
            now=now->son[0];
        }else if(now->val<val){
            ret=now;
            now=now->son[1];
        }else{
            now=now->son[0];
        }
    }
    return ret;
}
template<typename T>
splay::node<T>* splay::tree<T>::try_get_more_node(T val){
    node<T>* now=this->root;
    node<T>* ret=0;
    while(now!=0){
        if(!(now->val<val)&&!(now->val>val)){
            if(ret==0){
                ret=now;
            }
            now=now->son[1];
        }else if(now->val>val){
            ret=now;
            now=now->son[0];
        }else{
            now=now->son[1];
        }
    }
    return ret;
}
template<typename T>
void splay::tree<T>::del(T val){
    this->del(this->root,val);
}
template<typename T>
splay::node<T>* splay::tree<T>::split(T l,T r){
    if(this->root==0||l>r){
        return 0;
    }
    node<T>* lft=this->try_get_less_node(l);
    node<T>* rgt=this->try_get_more_node(r);
    if(lft!=0){
        if(rgt!=0){
            if(lft->val<l){
                if(rgt->val>r){
                    this->splaying(lft);
                    this->splaying(rgt,this->root);
                    return rgt->son[0];
                }else{
                    this->splaying(lft);
                    return this->root->son[1];
                }
            }else{
                if(rgt->val>r){
                    this->splaying(rgt);
                    return this->root->son[0];
                }else{
                    return this->root;
                }
            }
        }else{
            if(lft->val<l){
                this->splaying(lft);
                return this->root->son[1];
            }else{
                return this->root;
            }
        }
    }else if(rgt!=0){
        if(rgt->val>r){
            this->splaying(rgt);
            return this->root->son[0];
        }else{
            return this->root;
        }
    }
    return this->root;
}
template<typename T>
int splay::tree<T>::get_value_rank(T val){
    int res=0;
    node<T>* now=this->root;
    while(now!=0){
        if(now->val<val){
            res+=now->count;
            if(now->son[0]!=0){
                res+=now->son[0]->size;
            }
            now=now->son[1];
        }else{
            now=now->son[0];
        }
    }
    return res+1;
}
template<typename T>
T splay::tree<T>::get_pre(T val){
    T ret;
    node<T>* now=this->root;
    while(now!=0){
        if(now->val<val){
            ret=now->val;
            now=now->son[1];
        }else{
            now=now->son[0];
        }
    }
    return ret;
}
template<typename T>
T splay::tree<T>::get_nxt(T val){
    T ret;
    node<T>* now=this->root;
    while(now!=0){
        if(now->val>val){
            ret=now->val;
            now=now->son[0];
        }else{
            now=now->son[1];
        }
    }
    return ret;
}