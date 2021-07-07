
#include "Click_C5GNBIoT_types.h"
#include "Click_C5GNBIoT_config.h"

//Information
char AT[ 5 ]            = "AT";
char ATI[ 5 ]           = "ATI";
char AT_CIMI[ 10 ]      = "AT+CIMI";
char AT_CGMR[ 10 ]      = "AT+CGMR";
char AT_CGSN[ 10 ]      = "AT+CGSN";
char AT_CCID[ 10 ]      = "AT+CCID";
char AT_CGMI[ 10 ]      = "AT+CGMI";
char AT_CGMM[ 10 ]      = "AT+CGMM";

//thiet lap ket noi mang
char AT_CFUN[ 15 ]      = "AT+CFUN=0";
char AT_CEREG2[ 15 ]    = "AT+CEREG=5";
char AT_CGDCONT1[ 60 ]  = "AT+CGDCONT=1,\"IPV4V6\",\"E-connect\"";
char AT_CGDCONT[ 15 ]   = "AT+CGDCONT?";
char AT_CGATT[ 15 ]     = "AT+CGATT?";
char AT_CEREG[ 15 ]     = "AT+CEREG?";
char AT_CESQ[ 15 ]     = "AT+CESQ?";
char AT_COPS[ 15 ]     = "AT+COPS?";
char AT_NRB[ 15 ]     = "AT+NRB";
char AT_CGPADDR[ 15 ]     = "AT+CGPADDR";
char AT_NCONFIG[ 15 ]     = "AT+NCONFIG";

//thiet lap giao thuc ung dung
char AT_SICS[ 50 ]      = "AT^SICS";
char AT_SISS[ 100 ]      = "AT^SISS=0,\"address\",\"sockudp://192.168.1.31:910\"";
char AT_SISI[ 50 ]      = "AT^SISI=\"Ping\",0,192.168.1.1,4,2000";
char AT_SISX[ 100 ]      = "AT^SISX";      //Ping
char AT_SIPS[ 50 ]      = "AT^SIPS";      //save, load
char AT_SISO[ 50 ]      = "AT^SISO=0";      //Open UDP
char AT_SISC[ 50 ]      = "AT^SISC=0";      //Close UDP
char AT_SISW[ 50 ]      = "AT^SISW";      //Write Data
char AT_SISR[ 50 ]      = "AT^SISR";      //Receive Data





void interrupt_init (  )
{
    RXNEIE_USART1_CR1_bit = 1;
    NVIC_IntEnable( IVT_INT_USART1 );
    EnableInterrupts();
}

void system_init ( )
{
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_INT_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );

    mikrobus_uartInit( _MIKROBUS1, &C5GNBIOT_UART_CFG[0] );

    mikrobus_logInit( _LOG_USBUART_A, 115200 );

    Delay_ms( 100 );
    mikrobus_logWrite( " ***** SYS INIT ***** ", _LOG_LINE );
}

void application_init ( )
{
    c5gnbiot_uart_driver_init( (c5gnbiot_obj_t)&_MIKROBUS1_GPIO,
                                (c5gnbiot_obj_t)&_MIKROBUS1_UART );

    interrupt_init(  );

    c5gnbiot_set_on( 1 );
    Delay_ms( 2000 );

    c5gnbiot_send_cmd( &AT[0] );
    Delay_ms( 2000 );
    c5gnbiot_send_cmd( &ATI[0] );
    Delay_ms( 2000 );
    c5gnbiot_send_cmd( &AT_CFUN[0] );
    Delay_ms( 2000 );
    c5gnbiot_send_cmd( &AT_CEREG2[0] );
    Delay_ms( 2000 );
    c5gnbiot_send_cmd( &AT_CGDCONT1[0] );
    Delay_ms( 2000 );
    c5gnbiot_send_cmd( &AT_CIMI[0] );
    Delay_ms( 2000 );
}

void application_task ( )
{
    c5gnbiot_send_cmd( &AT_CGATT[0] );
    Delay_ms( 5000 );
    c5gnbiot_send_cmd( &AT_CEREG[0] );
    Delay_ms( 5000 );
}

char numberstr;
char portmaxstr;
char *iprv[80];
int i;

void ping5G(char *ip[50],int number,int portmax)
{
     UART2_Init(115200);
    // SISX - Ping
    IntToStr(number,numberstr);
    IntToStr(number,portmaxstr);
    //AT_SISI[ 50 ]      = "AT^SISI=\"Ping\",0,192.168.1.1,4,2000";
    AT_SISI[ 50 ]      = "AT^SISI=\"Ping\",0,";
    strcat( AT_SISI,ip);
    strcat (AT_SISI,",");
    strcat( AT_SISI,numberstr);
    strcat (AT_SISI,",");
    strcat( AT_SISI,portmaxstr);

    c5gnbiot_send_cmd( &AT_SISI[0] );

    for(i=0;i<(number+1);i++)
    {
        UART1_Read_Text(iprv,"\r\n",50);
        UART2_Write_Text(iprv);
    }
}

void reset5G(void)
{
    // AT+NRB - Reset board
    c5gnbiot_send_cmd( &AT_NRB[0] );
    Delay_ms( 2000 );
}

char *tempportudp[20];
char end_cmd[ 3 ] = "\"";
char tempcmd[3] = ":";
char etx [20]=";etx=26;timer=200";
void configUDP(char *ipudp[50],int port)
{

    //GPRS
    AT_SICS[ 50 ]  = "AT^SICS=0,\"conType\",\"GPRS0\"";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );

    //APN viettel e-connect
    AT_SICS[ 50 ]  = "AT^SICS=0,\"apn\",\"e-connect\"";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );

    //service socket
    AT_SISS[ 100 ]  = "AT^SISS=0,\"srvType\",\"Socket\"";
    c5gnbiot_send_cmd( &AT_SISS[0]);
    Delay_ms(500);
    
    //
    AT_SISS[ 100 ]  = "AT^SISS=0,\"conId\",\"0\"";
    c5gnbiot_send_cmd( &AT_SISS[0]);
    Delay_ms(500);
    
    //
    IntToStr(port, tempportudp);
    AT_SISS[ 100 ]  = "AT^SISS=0,\"address\",\"sockudp://";
    strcat(AT_SISS,ipudp);
    strcat(AT_SISS,tempcmd);
    strcat(AT_SISS,tempportudp);
    strcat(AT_SISS,end_cmd);
     
    c5gnbiot_send_cmd( &AT_SISS[0]);
    Delay_ms(500);
}

void configTCP(char *iptcp[50],int port)
{
    AT_SISS[ 100 ]  = "AT^SISS=1,\"srvType\",\"Socket\"";
    c5gnbiot_send_cmd( &AT_SISS[0]);
    Delay_ms( 500 );

    //APN viettel e-connect
    AT_SISS[ 100 ]  = "AT^SICS=1,\"conId\",1";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );

    //service socket
      IntToStr(port, tempportudp);
    AT_SISS[ 100 ]  = "AT^SISS=1,\"address\",\"socktcp://";
        strcat(AT_SISS,iptcp);
    strcat(AT_SISS,tempcmd);
    strcat(AT_SISS,tempportudp);
    strcat(AT_SISS, etx);
    strcat(AT_SISS,end_cmd);
    c5gnbiot_send_cmd( &AT_SISS[0]);
    Delay_ms( 500 );
}

void configMQTT(void)
{
/*
AT^SISS=0,"srvType","Mqtt" Select service type Mqtt.
OK
AT^SISS=0,"alphabet","1" The character set of string parameters(ASCII).
OK
AT^SISS=0,"conId",1 Select connection profile 1.
OK
AT^SISS=0,"address","mqtt://
user:pass@mqtt.gemalto.com"
Specify the address of MQTT server.
OK
AT^SISS=0,cmd,"publish" Select PUBLISH request to be send by MQTT client
to server.
OK
AT^SISS=0,"hcContentLen",10 Specify PUBLISH request payload will have 10 bytes
and will be defined by SISW command.
OK
AT^SISS=0,topic,"alarm/code" Select topic of PUBLISH request.
OK
AT^SISS=0,qos,"1"
*/
    AT_SISS[ 50 ]  = "AT^SICS=0,\"srvType\",\"Mqtt\"";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );
    
    AT_SISS[ 50 ]  = "AT^SISS=0,\"alphabet\",\"1\"";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );
    
    AT_SISS[ 50 ]  = "AT^SISS=0,\"conId\",1";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );
    
    AT_SISS[ 50 ]  = "AT^SISS=0,\"address\",\"mqtt://user:pass@mqtt.gemalto.com\"";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );
    
    AT_SISS[ 50 ]  = "AT^SISS=0,cmd,\"publish\"";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );
    
    AT_SISS[ 50 ]  = "AT^SISS=0,\"hcContentLen\",10";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );
    
    AT_SISS[ 50 ]  = "AT^SISS=0,topic,\"alarm/code\"";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );
    
    AT_SISS[ 50 ]  = "AT^SISS=0,qos,\"1\"";
    c5gnbiot_send_cmd( &AT_SICS[0]);
    Delay_ms( 500 );


}

void openport(void)
{
    c5gnbiot_send_cmd( &AT_SISO[0] );
    Delay_ms( 500 );
}

void closeport(void)
{
    c5gnbiot_send_cmd( &AT_SISC[0] );
    Delay_ms( 500 );


    strcat(AT_SISS,end_cmd);
    c5gnbiot_send_cmd( &AT_SISS[0]);
    Delay_ms( 500 );

    //save config
    AT_SIPS[ 30 ]  = "AT^SIPS=all,save,0";
    c5gnbiot_send_cmd( &AT_SIPS[0]);
    Delay_ms( 500 );

    //load config
    AT_SIPS[ 30 ]  = "AT^SIPS=all,load,0";
    c5gnbiot_send_cmd( &AT_SIPS[0]);
    Delay_ms( 500 );
}


    
char *templen;

void send_data(char *datasend[50])
{
    //configUDP();
    //openport();

    WordToStr(strlen(datasend),templen);
    AT_SISW[ 30 ]  = "AT^SISW=0,";
    strcat( AT_SISW, templen );

    c5gnbiot_send_cmd( &AT_SISW[0] );
    Delay_ms( 500 );

    c5gnbiot_send_cmd( datasend[0]);
}



void UART_RX_ISR() iv IVT_INT_USART1 ics ICS_AUTO
{
    if ( RXNE_USART1_SR_bit )
    {
        char tmp = USART1_DR;

        mikrobus_logWrite( &tmp, _LOG_BYTE );
    }
}