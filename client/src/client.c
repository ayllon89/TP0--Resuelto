#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */
	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger,"Hola, soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor=config_get_string_value(config,"CLAVE");
	ip=config_get_string_value(config,"IP");
	puerto=config_get_string_value(config,"PUERTO"); 

	// Loggeamos el valor de config

	log_info(logger,"lei la clave:%s de la ip:%s en el puerto:%s\n",valor,ip,puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip,puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	//creo un archivo tp0.log donde se setean los logs
	if ((nuevo_logger=log_create("cfg/tp0.log","tp0",1,LOG_LEVEL_INFO))==NULL)
	{
		printf("no se pudo iniciar el logger");
		exit(1);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	if((nuevo_config=config_create("cfg/cliente.config"))==NULL) //previamente hay que crear el archivo tp02.config
	{
		printf("no se puede crear el config");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline(">ingrese la linea a loguear:");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// ¡No te olvides de liberar las lineas antes de regresar!

	while(strcmp(leido,"") != 0)  //me fijo que lo que ingreso el usuario no sea vacio, si es vacio salgo del while
	{
		log_info(logger,leido);
		free(leido);
		leido = readline(">ingrese la linea a loguear:");
	}

	/*while(leido[0]!='\0')  //otra forma de hacerlo
	{
		log_info(logger,leido);
		free(leido);
		leido = readline(">ingresar linea:");
	}*/ 

	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;
	paquete=crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete, aca hago lo mismo que en la etapa 2 de leer en consola
	leido = readline(">ingresar linea a enviar al servidor:");
	while(strcmp(leido,"") != 0)  //me fijo que lo que ingreso el usuario no sea vacio, si es vacio salgo del while
	{
		agregar_a_paquete(paquete,leido, strlen(leido)+1);
		free(leido);
		leido = readline(">ingresar linea a enviar al servidor:");
	}

	enviar_paquete(paquete,conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if(logger!=NULL)
	{
		log_destroy(logger);
	}
	if(config!=NULL)
	{
		config_destroy(config);
	}
}
