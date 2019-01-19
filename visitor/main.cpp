#include <iostream>

using namespace std;

template<typename T>
struct visitor
{
    virtual void visit(T*) = 0;
};

struct visitor_token
{
    virtual ~visitor_token() = default;
};

struct animal
{
    virtual int move() = 0;
    virtual void accept(visitor_token*) = 0;
    virtual ~animal() = default;
};

// crtp
template<typename T>
struct visitable : public animal
 {
     void accept(visitor_token * v) override
     {
         dynamic_cast<visitor<T>*>(v)->visit(static_cast<T*>(this));
     }
 };

struct dog : public visitable<dog>{
    int move() override {
        return 4;
    }

    void swim(){
        std::cout<<"swim"<<std::endl;
    }
};

struct bird : public visitable<bird>{
    int move() override {
        return 2;
    }

    void fly(){
        std::cout<<"fly"<<std::endl;
    }
};

struct fish : public visitable<fish>{
    int move() override {
        return 1;
    }

    void dive(){
        std::cout<<"dive"<<std::endl;
    }
};

template<class... T>
struct MultipleVisitor : public visitor_token, public visitor<T>...
{
    //using visitor<T>::visit...;
};

using MyVisitor = MultipleVisitor<dog,bird>;
using MyVisitor1 = MultipleVisitor<fish>;

struct visitor_impl : public MyVisitor{
    void visit(dog* d) override{
        d->swim();
    }

    void visit(bird* b) override{
        b->fly();
    }
};

struct visitor_impl1 : public MyVisitor1{
    void visit(fish* f) override{
        f->dive();
    }
};

int main()
{
    animal* a = new dog;
    visitor_token* v = new visitor_impl;

    a->accept(v);

    animal* b = new bird;
    b->accept(v);

    visitor_token* v1 = new visitor_impl1;
    animal* c = new fish;
    c->accept(v1);
}
