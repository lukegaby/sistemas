/*
 * ServOriginal.h
 *
 *  Created on: 02/04/2014
 *      Author: utnso
 */

#ifndef SERVORIGINAL_H_
#define SERVORIGINAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <commons/string.c>
typedef struct{
	int socket;
	struct sockaddr_in socketInfo;
}t_cliente;


void nuevoCliente(int servidor, t_cliente *clientes, int *nClientes);
int dameMaximo(t_cliente *tabla, int n);
void compactaClaves(t_cliente *tabla, int *n);
int Abre_Socket_Inet();
t_cliente Acepta_Conexion_Cliente(int Descriptor);
void enviar_msj(char* cadena);
char** recibir_msj(int socketCliente, int bytesArecibir);

#endif /* SERVORIGINAL_H_ */
