
#include <ros/ros.h>
// for using serial communication
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <arpa/inet.h>

// setup the initial name
using namespace ros;
using namespace std;

#define DF_UDP_PORTNUM      16100 
#define DF_UDP_SERVER_ADDR  "192.168.0.109"
#define DF_UDP_CLIENT_ADDR  "192.168.0.2"
#define DF_UDP_BUFFER_SIZE  256

class UDP_ROS_Server
{
public:
    UDP_ROS_Server(ros::NodeHandle& n);        
    ~UDP_ROS_Server();
    ros::NodeHandle nh;
};

UDP_ROS_Server::UDP_ROS_Server(ros::NodeHandle& n)// : steering_angle(0.0), speed(0.0)
{

    

    ROS_DEBUG("SERVER_TO_SEND is created");
};

UDP_ROS_Server::~UDP_ROS_Server() 
{    
    ROS_INFO("SERVER_TO_SEND destructor.");
}

#pragma pack(1)

struct Pointcloud_data
{

    uint16_t header_1;
	double name;

    uint16_t usrg_password;
    char we_know;

};

#pragma pack()


// node main loop, for ROS
int main(int argc, char** argv)
{   
    // ROS Configuration
    // node name initialization
    init(argc, argv, "UDP_ROS_Server");

    ros::NodeHandle nh_;
    UDP_ROS_Server server_node(nh_);

    // for debugging
    printf("Initiate: Server_TX\n");
    ros::Rate loop_rate(25);

    int    Socket;
    struct sockaddr_in ServerAddr;
    //struct struct_t_UDP           StrUDP;
    //struct sockaddr_in            MyAddr;
    struct Pointcloud_data        TX_buff;

    // UDP Configuration
    // Socket Creation
    Socket = socket(PF_INET, SOCK_DGRAM, 0);
    int enable = 1;
    setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    if(Socket == -1){
        printf("[ERROR] 'socket()'\n");
        return -1;
    }
    else{
        printf("[DONE] UDP socket is created\n");
    }

    // UDP-IP Setting
    memset(&ServerAddr, 0, sizeof(ServerAddr)); // Clear to 0
    ServerAddr.sin_family      = PF_INET;
    ServerAddr.sin_port        = htons(DF_UDP_PORTNUM); // PORT#
    ServerAddr.sin_addr.s_addr = inet_addr(DF_UDP_CLIENT_ADDR); // IP for Server (Normally PC IP)
    

    

    while(ros::ok()){

        // int8_t steerToSend = steering_angle * STEERING_RESOLUTION_GAIN 

        TX_buff.header_1 = 80;
        TX_buff.name=1234 ;
        TX_buff.usrg_password = 1;


        sendto(Socket, (char*)&TX_buff, sizeof(TX_buff), 0, (struct sockaddr *)(&ServerAddr), sizeof(ServerAddr));
        ROS_INFO("Send message!");
        // std::cout << "--------------------------------------------------------" << std::endl;
        // ROS_INFO("Steering cmd:  %f, Speed cmd : %f)", _server_to_send.steering_angle, _server_to_send.speed);
        // ROS_INFO("TX data to Unmanned: STEER: %d, SPEED : %d)", TX_buff.steer_cmd, TX_buff.speed_cmd);

        loop_rate.sleep();
        spinOnce();
    }

    // for debugging
    printf("Terminate\n");
    close(Socket);

    return 0;
}
