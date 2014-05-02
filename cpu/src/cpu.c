/*
 * cpu.c
 *
 *  Created on: 01/05/2014
 *      Author: lucas
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/string.h>
#include <parser/parser.h>
#include "cpu.h"
int main(int argc, char* argv[]) {
	char* directorio;
	int cpu_conectada=0;
	t_config* archConfig;
	pthread_t hilo_cpu[cant_cpu];
	s_ip_kernel=calloc(malloc_size,sizeof(char));
	s_ip_umv=calloc(malloc_size,sizeof(char));
	directorio=calloc(malloc_size,sizeof(char));
	//directorio = argv[1];
	directorio = configuracion;
	//loggercpu=log_create("CPU","CPU",true,LOG_LEVEL_INFO);
	archConfig = config_create(directorio);
	s_ip_kernel = config_get_string_value(archConfig, "IP_KERNEL");
	s_ip_umv = config_get_string_value(archConfig, "IP_UMV");
	i_puerto_kernel=config_get_long_value(archConfig,"PUERTO_KERNEL");
	i_puerto_umv=config_get_long_value(archConfig,"PUERTO_UMV");
	pthread_mutex_init(&mutex, NULL);
	for(cpu_conectada=0;(cpu_conectada<=cant_cpu);cpu_conectada++)
	{
		if (pthread_create(&hilo_cpu[cpu_conectada], NULL, &proceso_cpu,NULL) != 0) {
			printf("Uh-oh! Falla al crear la Cpu%d\n",cpu_conectada);
			/*return -1;*/}
	}
	if	(cpus==cant_cpu){
		free(archConfig);
		free(s_ip_umv);
		free(s_ip_kernel);
	}
	for(cpu_conectada=0;(cpu_conectada<=cant_cpu);cpu_conectada++)
		pthread_join(hilo_cpu[cpu_conectada], NULL );
	pthread_mutex_destroy(&mutex);
	puts("terminaron los hilos");
	return 1;
}
void pasarachar(char* nombre, char regresa[]){
	int tamanio,i;
	tamanio=strlen(nombre)-1;
	for (i=0;(i<=tamanio);i++){
		regresa[i]=*(nombre + i);
	};
}
void *proceso_cpu(){
	t_socket  kernel;
	t_socket  umv;
	memset(kernel.ip_conectar,'\0',malloc_size);
	memset(umv.ip_conectar,'\0',malloc_size);
	pthread_mutex_lock(&mutex);
	int nro_cpu=cpus;
	cpus++;
	pthread_mutex_unlock(&mutex);
	pasarachar(s_ip_kernel,&kernel.ip_conectar[0]);
	pasarachar(s_ip_umv,&umv.ip_conectar[0]);
	kernel.puerto_conectar = i_puerto_kernel;
	umv.puerto_conectar = i_puerto_umv;
	printf("KERNEL: %s:%d \nUMV:%s:%d\nEl numero de cpu es : %d \n",kernel.ip_conectar,kernel.puerto_conectar,umv.ip_conectar,umv.puerto_conectar,nro_cpu);
	//puts(kernel.ip_conectar);
	//puts(umv.ip_conectar);
	return NULL;
}
