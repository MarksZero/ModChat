#include <stdio.h>
#include "serial.h"
#include "slip.h"
#include "string.h"

#define LEN 15
#define BYTE unsigned char
#define HELP "-h"
// #define VIRTUAL_PORT "../tmp/p1"
struct frame_ipv4
{
    BYTE flag_fragmento=0; //4bits  0 no fragmentado, 1 fragmentado, 2 ultimo fragmento
    int offset_fragmento=0; //12bits   si esta fragmentado
    BYTE len_datos[2]={0}; //16bits 2bytes largo total del paquete
    BYTE TTL=0; //8bits 1byte
    BYTE identificacion=0; //8bits 1byte    si esta fragmentado
    BYTE s_verificacion[2]={0}; //16bits 2bytes
    BYTE ip_origen[4]={0}; //32bits 4bytes
    BYTE ip_destino[4]={0}; //32 bits 4bytes
    BYTE DATA[LEN]={0}; //  memcpy(void*(org),void*(dst),len)
};


int main(int nargs, char* arg_arr[]){
    if(nargs == 4){ //./nodo "nombreIp" Emisor Receptor
        char msg[500];
        char msg_rx[1000];
        int len = 0;
        //Obtiene nombre IP
        char* nombreIP = arg_arr[1];
        //Obtiene puerto virtual tx
        char* virtual_port_tx = arg_arr[2];
        //Obtiene puerto virtual rx
        char* virtual_port_rx = arg_arr[3];

        FILE* vport_escribe = fopen(virtual_port_tx, "w");
        FILE* vport_lee = fopen(virtual_port_rx, "r");
        //Inicia la capa 1 y 2
        //Obtiene descriptores
        int stdin_desc = fileno(stdin);
        //Obtiene descriptores
  
        //Indica inicio del chat
        printf("chat\n");
        printf("Ya puede escribir sus mensajes !\n");
        printf("MENU\n");
        printf("Mi IP: %s\n", nombreIP);
        while(true){
            //Lee mensajes del puerto virtual y los muestra
            len = readSlip((BYTE*)msg_rx, 1000,vport_lee);
            msg_rx[len-1] = '\0';
            if(len>0){ //y si es 255 entonces broadcast
                printf("El otro usuario dice: %s", msg_rx);
            }// si len mayor a 0 y ip destino igual a nombreIP entonces unicast
            //sino omite
            /*
            
            */
            // len = readSlip((BYTE*)msg_rx,1000);
            // msg_rx[len] = '\0';
            // if(len>0){
            //     printf("El otro usuario dice: %s", msg_rx);
            // }
            //Lee consola y envía el mensaje por el puerto virtual
            len = readPort(stdin_desc, (BYTE*)msg, 500, 100);
            msg[len] = '\0';
            if(len>0){
                
                writeSlip((BYTE*)msg, len,vport_escribe);
            }
        }
    }else if(nargs==2 && strcmp(arg_arr[1],HELP)==0){
        printf("MANUAL DE USUARIO:\n");
		printf("-------------------\n");
		printf("\t Modo de Uso:\n");
		printf("\t\t1-.\t./nodo nombreIP emisor receptor\n");
		printf("\t Ejemplo:\n");
		printf("\t\t1-.\t./nodo 192.168.130.1 ../tmp/p1 ../tmp/p10\n");
    }else{
        //Se requiere un y solo un argumento; el puerto virtual
        printf("Debe indicar el puerto virtual y/o IP!\n");
        printf("Para mas ayuda escriba ./nodo -h\n");
    }    
    return 0;
}
