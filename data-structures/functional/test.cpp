#include "list.h"
#include <iostream>

using namespace std;

int main()
{
    List<int> l;
    for(int i=0;i<20;++i)
    {
        l = l.push_front(i);
    }
    l = filter([](int i){ return i%4==0; }, fmap<int>([](int i){ return i*2;}, l));
    forEach(l, [](int v)
    {
        cout << v << ", ";
    });
    cout << endl;
}
