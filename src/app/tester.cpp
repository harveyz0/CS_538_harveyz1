#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>
#include <typeinfo>
#include "doctest/doctest.h"
#include "vec.hpp"
using namespace std;


int main(){
//TEST_CASE("ONE"){
    Vec3<int>* vi = new Vec3<int>(1,2,3);
    cout << "vi " << vi->x << " " << vi->y << " " << vi->z << endl;
    Vec3<float> vf(1.5,2.5, 3.5);
    Vec3<double> vd(1.5,2.5, 3.5);
    auto va = vf + vd;

    cout << "va " << va.x << " " << va.y << " " << va.z << " " << typeid(va).name() << endl;
    // The previous line prints the following
    // va 3 5 7 4Vec3IdE
    // I have no idea what 4Vec3IdE means.
    //return 0;
}

