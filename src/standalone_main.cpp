#include "../xislLib/include/Acq.h"
#include "iostream"
int main(){
    int numSensors = 0;
    std::cout << "calling library function..." << std::endl;
    auto ret = Acquisition_EnumSensors(reinterpret_cast<UINT *>(&numSensors), false, false);
    std::cout << "got result " << ret << std::endl;
    return ret;
}