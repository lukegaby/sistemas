#include "ServOriginal.h"
#define MAX_CLIENTES 5
char buffer[512];

int main() {
	int socketServidor; /* Descriptor del socket servidor */
	t_cliente socketCliente[MAX_CLIENTES];/* Descriptores de sockets con clientes */
	int numeroClientes = 0; /* Número clientes conectados */
	fd_set descriptoresLectura; /* Descriptores de interes para select() */
	int maximo; /* Número de descriptor más grande */
	int i=0; /* Para bubles */
	int bytecount;
	int salida = 1;
	int fuera = 0;
	int bytesArecibir;
	char** mensajeRecibido;
	//struct timeval tv;
	// Se abre el socket servidor, verificando error y configurando parametros necesarios
	socketServidor = Abre_Socket_Inet();
	if (socketServidor == -1) {
		perror("Error al abrir servidor");
		exit(-1);
	}
	//inicializa el vector de socketCliente en 0
	while(MAX_CLIENTES>=i){
		socketCliente[i].socket = 0;
		i++;}

	// Bucle, atiende clientes nuevos y clientes ya conectados ,
	//sale cuando no hay mas clientes
	while (salida) {
		//tv.tv_sec = 0;
		//tv.tv_usec = 5000;
		compactaClaves(socketCliente, &numeroClientes);
		// Se eliminan todos los clientes que hayan cerrado la conexión
		FD_ZERO(&descriptoresLectura);// se inicializa el descriptor de lectura
		FD_SET(socketServidor, &descriptoresLectura);
		// Se añade para select() el socket servidor
		// Se añaden para select() los sockets con los clientes ya conectados
		for (i = 0; i < numeroClientes; i++)
			FD_SET(socketCliente[i].socket, &descriptoresLectura);
		// devuelve el valor del descriptor más grande. Si no hay ningún cliente,
		//devolverá 0
		maximo = dameMaximo(socketCliente, numeroClientes);
		if (maximo < socketServidor)
			maximo = socketServidor;
		// Espera indefinida hasta que alguno de los descriptores tenga algo
		// que decir: un nuevo cliente o un cliente ya conectado que envía un
		// mensaje
		select(maximo + 1, &descriptoresLectura, NULL, NULL, /*&tv*/NULL);
		memset(buffer, 0, 512);
		// Se comprueba si algún cliente ya conectado ha enviado algo
		for (i = 0; i < numeroClientes; i++) {
			if (FD_ISSET (socketCliente[i].socket, &descriptoresLectura)) {
				//recibimos SOLO el int que representa la cantidad de bytes del mensaje ej:21
				bytecount = recv(socketCliente[i].socket, buffer,sizeof(int) , 0);
				memcpy(&bytesArecibir, &buffer[0], sizeof(int));
				if (strncmp(buffer, "EXIT", 4) == 0)
					bytecount = 0;
				switch (bytecount) {
				case -1:
					perror("No puedo recibir informacion");
					break;
				case 0:
					printf("Cliente %d ha cerrado la conexión\n", i + 1);
					close(socketCliente[i].socket);
					socketCliente[i].socket = -1;
					break;
				default:
					memset(buffer, '\0', 512);
					mensajeRecibido=recibir_msj(socketCliente[i].socket, bytesArecibir);
					printf("Cliente %d envía  %s %s %s %s\n", i + 1,  mensajeRecibido[0],	mensajeRecibido[1],mensajeRecibido[2], mensajeRecibido[3]);
					break;
				}
			}
		}
		// Se comprueba si algún cliente nuevo desea conectarse y se le
		 if (FD_ISSET (socketServidor, &descriptoresLectura))
			nuevoCliente(socketServidor, socketCliente, &numeroClientes);
		 //condicion de salida
		i=0;
		//compactaClaves(socketCliente, &numeroClientes);
		while(MAX_CLIENTES>i){
			if ((socketCliente[i].socket == 0) || (socketCliente[i].socket == -1))
				fuera++;
			i++;}
		if (fuera == MAX_CLIENTES)
			salida = 0;else fuera=0;
	}//fin while
	printf("Socket Servidor Cerrado\n");
	close(socketServidor);
	return (0);
}
//--> fin de main --<

//Crea un nuevo socket cliente.

void nuevoCliente(int servidor, t_cliente *clientes, int *nClientes) {
	clientes[*nClientes] = Acepta_Conexion_Cliente(servidor);
	(*nClientes)++;
//Si se ha superado el maximo de clientes, se cierra la conexión,
//se deja todo como estaba y se vuelve.
	if ((*nClientes) >= MAX_CLIENTES) {
		close(clientes[(*nClientes) - 1].socket);
		(*nClientes)--;
		return;
	}
// Escribe en pantalla que ha aceptado al cliente y vuelve */
	printf("Aceptado cliente %d\n", *nClientes);
	return;
}

// Función que devuelve el valor máximo en la tabla.
int dameMaximo(t_cliente *tabla, int n) {
	int i;
	int max;
	if ((tabla == NULL )|| (n<1))return 0;
	max = tabla[0].socket;
	for (i = 0; i < n; i++)
		if (tabla[i].socket > max)
			max = tabla[i].socket;
	return max;
}
// Busca en array todas las posiciones con -1 y las elimina,
//copiando encima las posiciones siguientes.
void compactaClaves(t_cliente *tabla, int *n) {
	int i, j;
	if ((tabla == NULL )|| ((*n) == 0))return;
	j = 0;
	for (i = 0; i < (MAX_CLIENTES); i++) {
		if ((tabla[i].socket != -1) && (tabla[i].socket != 0))  {
			tabla[j].socket = tabla[i].socket;
			j++;
		}else
			tabla[j].socket=0;
	}
	*n = j;
}
//Abre un socket servidor de tipo AF_INET
int Abre_Socket_Inet() {
	struct sockaddr_in Direccion;
	int Puerto=5005;
	int Descriptor;
	Descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (Descriptor == -1)
		return -1;
// Se rellenan los campos de la estructura Direccion, necesaria
	Direccion.sin_family = AF_INET;
	Direccion.sin_port = htons(Puerto);
	Direccion.sin_addr.s_addr = INADDR_ANY;
	if (bind(Descriptor, (struct sockaddr *) &Direccion, sizeof(Direccion))	== -1) {
		close(Descriptor);
		return -1;
	}
//Ponerse a la escucha si da error cerrar el socket
	if (listen(Descriptor, 1) == -1) {
		close(Descriptor);
		return -1;
	}
	return Descriptor;
}
//Se le pasa un socket de servidor y acepta en el una conexion de cliente
//devuelve el descriptor del socket del cliente o -1 si hay problemas.
t_cliente Acepta_Conexion_Cliente(int Descriptor) {
	socklen_t Longitud_Cliente;
	struct sockaddr_in Cliente;
	t_cliente Hijo;
	Longitud_Cliente = sizeof(Cliente);
	Hijo.socket = accept(Descriptor, (struct sockaddr*) &Cliente,&Longitud_Cliente);
	Hijo.socketInfo = Cliente;
	return Hijo;
}

char** recibir_msj(int socketCliente, int bytesArecibir){
char** substr;
char *separador = ",";
recv(socketCliente, buffer, bytesArecibir , 0); //recivimos el mensaje concreto ej "auto,avion,yate"
substr = string_split(buffer, separador);
return(substr);
}
