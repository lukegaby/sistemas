/** client.c -- Ejemplo de cliente de sockets de flujo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORT 5005 // puerto al que vamos a conectar
char buffer[512];

void enviar_msj(int socketServidor ,char* cadenaBuff);

int main(){
int sockfd;
char* mensajeAenviar;
mensajeAenviar=(char*)malloc(sizeof(char)*128); // se asume un mensaje de 64 bytes maximo


struct sockaddr_in their_addr; // información de la dirección de destino

if ((sockfd =socket(AF_INET, SOCK_STREAM, 0)) == -1) {
perror("socket");
exit(1);
}
their_addr.sin_family = AF_INET; // Ordenación de bytes de la máquina
their_addr.sin_port = htons(PORT); // short, Ordenación de bytes de la red
their_addr.sin_addr.s_addr =INADDR_ANY;
memset(&(their_addr.sin_zero),'\0', 8); // poner a cero el resto de la estructura
if (connect(sockfd, (struct sockaddr *)&their_addr,
sizeof(struct sockaddr)) == -1) {
printf("no me puedo conectar");
perror("connect");
exit(1);}
strcpy(mensajeAenviar,"pepe,1245,mefuialcine,estoesunaprueba");
enviar_msj(sockfd, mensajeAenviar);
memset(mensajeAenviar, '\0', 128);
strcpy(mensajeAenviar, "undiacomohoy,desaprovamosso,porqueelnegroesputo,25veces");
enviar_msj(sockfd, mensajeAenviar);
free(mensajeAenviar);
close(sockfd);
return (0);
}

void enviar_msj(int socketServidor, char* cadenaBuff){
		int tamanio;
		tamanio=strlen(cadenaBuff);
		memset(buffer, '\0', 512);
		memcpy(&buffer[0], &tamanio ,sizeof(int));
		memcpy(&buffer[4], cadenaBuff ,sizeof(char)*tamanio);
		send(socketServidor, buffer,  sizeof(int)+sizeof(char)*tamanio ,0);
}
