/*
 * server.c
 *
 *  Created on: Oct 9, 2012
 *      Author: hmng
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "jsonrpc-c.h"

#include <inttypes.h>

#define PORT 1234  // the port users will be connecting to

struct jrpc_server my_server;

cJSON * say_hello(jrpc_context * ctx, cJSON * params, cJSON *id) {
	cJSON *result;
	if (params) {
		cJSON *json = cJSON_GetArrayItem(params, 0);
		if (json && (json->type == cJSON_Number)) {
			if (cJSON_Maybe64(json)) {
				printf("uint64_t=%" PRIu64 "\n", json->valueu64);
				result = cJSON_CreateNumber64(json->valueu64);
			} else {
				printf("double=%lf\n", json->valuedouble);
				result = cJSON_CreateNumber(json->valuedouble);
			}
		}
        json = cJSON_GetArrayItem(params, 1);
		if (json && (json->type == cJSON_Number)) {
			if (cJSON_Maybe64(json)) {
				printf("uint64_t=%" PRIu64 "\n", json->valueu64);
				result = cJSON_CreateNumber64(json->valueu64);
			} else {
				printf("double=%lf\n", json->valuedouble);
				result = cJSON_CreateNumber(json->valuedouble);
			}
		}
	} else {
		result = cJSON_CreateString("Hello!");
	}

	return result;
}

cJSON * exit_server(jrpc_context * ctx, cJSON * params, cJSON *id) {
	jrpc_server_stop(&my_server);
	return cJSON_CreateString("Bye!");
}

int main(void) {
	jrpc_server_init(&my_server, PORT);
	jrpc_register_procedure(&my_server, say_hello, "sayHello", NULL );
	jrpc_register_procedure(&my_server, exit_server, "exit", NULL );
	jrpc_server_run(&my_server);
	jrpc_server_destroy(&my_server);
	return 0;
}
