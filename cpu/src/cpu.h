
/*
 * cpu.h
 *
 *  Created on: 02/05/2014
 *      Author: lucas
 */

#ifndef CPU_H_
#define CPU_H_

#define configuracion "/home/lucas/Escritorio/configuracion/CPU/conexion.cfg"
#define	cant_cpu 10
#define malloc_size 256
	char* s_ip_kernel;
	char* s_ip_umv;
	int i_puerto_kernel,i_puerto_umv;
	int	  cpus=0;
typedef struct{
	char ip_conectar[malloc_size];
	int	 puerto_conectar;
}t_socket;
pthread_mutex_t mutex;
void pasarachar(char* nombre, char regresa[]);
void *proceso_cpu();
//t_log* loggercpu;

#endif /* CPU_H_ */
