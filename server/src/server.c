#include "server.h"

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			// opcion 1: puts. Funca pero no escribe en el log.
			//list_iterate(lista, puts);
			// opcion 2: iterator. No funca porque closure espera un argumento y logger tiene dos?
			//list_iterate(lista, (void*) iterator);
			// opcion 3: copypasteo la implementacion de list_iterate y reemplazo closure por el iterator o el log_info, es lo mismo
			t_link_element *element = lista->head;
				t_link_element *aux = NULL;
				while (element != NULL) {
					aux = element->next;
					log_info(logger, "%s\n", element->data);
					element = aux;
				}
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,
					"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;
}

void iterator(t_log* logger, char* value) {
	log_info(logger,"%s\n", value);
}
