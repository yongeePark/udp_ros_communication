
#include <ros/ros.h>
// for using serial communication
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <arpa/inet.h>

// setup the initial name
using namespace ros;
using namespace std;

#define DF_UDP_PORTNUM      1234 
#define DF_UDP_SERVER_ADDR  "192.168.0.92"
#define DF_UDP_CLIENT_ADDR  "192.168.0.2"
#define DF_UDP_BUFFER_SIZE  128

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

// void UDP_ROS_Server::floatCallback(const std_msgs::UInt16& msg)
// {
//     floatMsg = msg;
// }



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
    struct Pointcloud_data        pointcloud_data;

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

        pointcloud_data.header_1 = 80;
        pointcloud_data.name=1234 ;


        // pointcloud_data.usrg_password = _server_to_send.floatMsg.data;
        // pointcloud_data.we_know = 'f';
	    // ROS_INFO("%d",TX_buff.usrg_password);


        /*

        if(!_server_to_send.joyMsg.buttons.empty())
        {
            if(_server_to_send.joyMsg.buttons.size() < 8)
                return 0;
            if(_server_to_send.joyMsg.buttons.at(7) == 1 )
			{}
                //TX_buff.powertrain_mode = 2; // 0 : neutral, 1: forward, 2: backward
            // else if(_server_to_send.joyMsg.buttons.at(6) == 1 || (_server_to_send.turnMsg.data == true && _server_to_send.joyMsg.buttons.at(5) != 1)) //
            // {
            //     TX_buff.powertrain_mode = 0; // 0 : neutral, 1: forward, 2: backward
            //     TX_buff.speed_cmd = _server_to_send.speed * SPEED_RESOLUTION_GAIN;
            //     if(TX_buff.speed_cmd > 20)
            //         TX_buff.speed_cmd = 20;
            // }
                
            else
            {
                //TX_buff.powertrain_mode = 1; // 0 : neutral, 1: forward, 2: backward
            }
        }

        //TX_buff.chk_sum_1 = 0;
        //TX_buff.chk_sum_2 = 0;
        

        */

        sendto(Socket, (char*)&pointcloud_data, sizeof(pointcloud_data), 0, (struct sockaddr *)(&ServerAddr), sizeof(ServerAddr));
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
