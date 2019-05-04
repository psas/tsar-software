#ifndef __CM__
#define __CM__

class CentralManager {
    public:
        CentralManager();
        ~CentralManager();
        void CM_loop();

    private:
        int read_hardware();
        int update();
        int state_machine();
        int control();
}

#endif
