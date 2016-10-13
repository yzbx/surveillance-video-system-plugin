#include "DataDriveFunctions.h"
#include "DataDriveMain.h"
#include "yzbx_config.h"
using namespace DataDrive ;

 class DllTest:public Base{
public:
    GET_CLASS_NAME
    DllTest(std::shared_ptr<DataDriveMain> data):Base(data){}
    bool run();
};

bool DllTest::run(){
  data->frameNum++;
  return true;
};

extern "C" Base * create(std::shared_ptr<DataDriveMain> data){
  return new DllTest(data);
};

extern "C" void destroy(Base * p){
  delete p;
};

