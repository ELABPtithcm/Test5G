#include "Click_C5GNBIoT_types.h"
void interrupt_init (  );
void application_init ( );
void system_init ( );
void application_task ( );
void ping5G(char *ip[50],int number,int portmax);  //Example: ping5G("192.168.1.1",4,2000);
void reset5G(void);
void configUDP(char *ipudp[50],int port);    //Example: configUDP("192.168.1.1",1234);
void configTCP(char *iptcp[50],int port);
void configMQTT(void);
void openport(void);
void closeport(void);
void send_data(char *datasend[50]);