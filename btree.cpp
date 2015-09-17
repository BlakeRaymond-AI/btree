#include "btree.h"
#include <vector>
#include <iostream>
#include <cassert>
#include <iterator>

struct S
{
    explicit S(int s)
        : _s(s)
    {}

    int value() const { return _s; }

    int _s;
};

int main(int argc, char** argv)
{
    std::cout << "Hello, world!" << std::endl;

    block_vector<int> bv;
    bv.push_back(1);
    bv.push_back(2);
    bv.push_back(3);
    bv.push_back(4);
    bv.push_back(5);
    bv.push_back(6);

    std::copy(bv.begin(), bv.end(), std::ostream_iterator<int>(std::cout, ", "));

    block_vector<S> bvs;
    bvs.emplace_back(1);
    bvs.emplace_back(2);
    bvs.emplace_back(3);
    bvs.emplace_back(4);
    bvs.emplace_back(5);
    bvs.emplace_back(6);
    bvs.emplace_back(7);
    auto iter = bvs.begin();

    std::cout << std::endl;

    while (iter != bvs.end())
    {
        std::cout << iter->value() << ", ";
        ++iter;
    }

    return 0;
}

