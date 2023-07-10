// I won't use ros in the client
// #include <ros/ros.h>
// for using serial communication
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <arpa/inet.h>

#include <iostream>
#include <cstring>
#include <cstdio>


// setup the initial name
// using namespace ros;
using namespace std;

#define DF_UDP_BUFFER_SIZE  256
#define DF_UDP_PORTNUM      16100

#define DF_UDP_SERVER_ADDR  "192.168.0.109"
#define DF_UDP_CLIENT_ADDR  "192.168.0.2"	//"127.0.0.1" -> local 


struct timeval tv_timeo = { 0, 500000 };   // 0.5 seconds

struct struct_t_UDP
{
    int    Socket;
    struct sockaddr_in ServerAddr;
    double   RXBuffer[DF_UDP_BUFFER_SIZE];
};
#pragma pack(push,1)
struct RX_message_data
{
    uint16_t usrg_password;
    double tmp_num_double;
};
#pragma pack(1)

struct Pointcloud_data
{

    uint16_t header_1;
	double name;

    uint16_t usrg_password;
    char we_know;
 /*   uint8_t veh_mode;
    uint8_t brake_cmd;
    int8_t  steer_cmd;
    uint8_t powertrain_mode;
    uint8_t speed_cmd;
    uint8_t chk_sum_1;
    uint8_t chk_sum_2;*/
};

#pragma pack()
#pragma pack(pop)


class CLIENT_TO_RECEIVE
{
    public:
        CLIENT_TO_RECEIVE();        
        ~CLIENT_TO_RECEIVE();

    // void VehCommandCallback(const ackermann_msgs::AckermannDriveStamped& msg);
 
};

CLIENT_TO_RECEIVE::CLIENT_TO_RECEIVE() 
{
    // Constructor
    std::cout<<"Running Constructor"<<std::endl;
};

CLIENT_TO_RECEIVE::~CLIENT_TO_RECEIVE() 
{    
    std::cout<<"Running deconstructor"<<std::endl;
}


// node main loop, for ROS
int main(int argc, char** argv)
{    
    // node name initialization
    // init(argc, argv, "UnmannedSol_UDP_RX");
    
    printf("Initiate: Server_RX\n");

    int    Socket;

    struct struct_t_UDP           StrUDP;
    struct sockaddr_in            MyAddr;
    struct Pointcloud_data        RX_buff;


    // Socket Creation
    StrUDP.Socket = socket(PF_INET, SOCK_DGRAM, 0);
    if(StrUDP.Socket == -1)
    {
        printf("[ERROR] 'socket()'\n");
        return -1;
    }
    else
    {
        printf("[DONE] UDP socket is created\n");
    }

    // UDP-IP Setting_Client
    memset(&StrUDP.ServerAddr, 0, sizeof(StrUDP.ServerAddr)); // Clear to 0
    StrUDP.ServerAddr.sin_family      = PF_INET;
    StrUDP.ServerAddr.sin_port        = htons(DF_UDP_PORTNUM); // PORT#
    StrUDP.ServerAddr.sin_addr.s_addr = inet_addr(DF_UDP_SERVER_ADDR); // IP for Server (Normally PC IP)

    memset(&MyAddr, 0, sizeof(MyAddr));
    MyAddr.sin_family = PF_INET;
    MyAddr.sin_port = htons(DF_UDP_PORTNUM);
    //MyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    MyAddr.sin_addr.s_addr = inet_addr(DF_UDP_CLIENT_ADDR);

    if(bind(StrUDP.Socket,(struct sockaddr *)&MyAddr, sizeof(MyAddr))!=0)
    {
        printf("bind1() error!\n");
        return -1;
    }

    int size_addr = sizeof(StrUDP.ServerAddr);

    if(setsockopt(StrUDP.Socket,SOL_SOCKET, SO_RCVTIMEO, &tv_timeo, sizeof(tv_timeo)) == -1)
    {
        printf("setsockopt error!\n");
        return -1;
    }
    
    CLIENT_TO_RECEIVE _client_to_receive;

    int temp_data;
    int index = 0;
    std::cout<<"start main loop"<<std::endl;
    while(index<10000)
    {
        memset(StrUDP.RXBuffer, 0, sizeof(StrUDP.RXBuffer));
        temp_data = recvfrom(StrUDP.Socket, StrUDP.RXBuffer, sizeof(StrUDP.RXBuffer), 0, (struct sockaddr *)(&StrUDP.ServerAddr), (socklen_t *)&size_addr);

        memcpy(&RX_buff, (Pointcloud_data*)StrUDP.RXBuffer, sizeof(Pointcloud_data));


        uint16_t password = RX_buff.usrg_password;
        double received_name = RX_buff.name;
        // double tmp_num_buf = RX_buff.tmp_num_double;
        // double out = _client_to_receive.out_pass;
        std::cout<<"passward : "<<received_name<<std::endl;
        
    }

    // for debugging
    std::cout<<"Terminate: Client\n"<<std::endl;
    close(StrUDP.Socket);

    return 0;
}
