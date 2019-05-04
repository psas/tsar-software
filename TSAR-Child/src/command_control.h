#ifndef __CM__
#define __CM__

class CommandControl {
    public:
        CommandControl();
        ~CommandControl();
        int start_system();

    private:
        int interface();

        SystemStatus status;
};

#endif 
