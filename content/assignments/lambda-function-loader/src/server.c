// SPDX-License-Identifier: BSD-3-Clause

#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#include "ipc.h"


#include "ipc.h"
#include "server.h"

#ifndef OUTPUT_TEMPLATE
#define OUTPUT_TEMPLATE "../checker/output/out-XXXXXX"
#endif

static int lib_prehooks(struct lib *lib)
{
       // pre exectution here crd
        /* TODO: Implement lib_prehooks(). */
    return 0;
}

static int lib_load(struct lib *lib)
 {
	//char* mere="/home/student/hackkk/operating-systems/content/assignments/lambda-function-loader/tests";
    //printf("++ %s ++",lib->filename);
	lib->handle = dlopen(lib->filename,RTLD_NOW);
    if (!lib->handle) {
        fprintf(stderr, "Error loading library: %s\n", dlerror());
        return -1;
    }
    return 0;
 	
 }


static int lib_execute(struct lib *lib)
{
	/* TODO: Implement lib_execute(). */
    // Call the function
	typedef void (*Functia)();
	Functia yourFunction = (Functia)dlsym(lib->handle, lib->funcname);
    // Add any other operations you need to perform
	yourFunction();
	//fclose(lib->handle);
	return 0;
}

static int lib_close(struct lib *lib)
{
	/* TODO: Implement lib_close(). */
	fclose(lib->handle);
	return 0;
}

static int lib_posthooks(struct lib *lib)
{
	/* TODO: Implement lib_posthooks(). */
	return 0;
}

static int lib_run(struct lib *lib)
{
	int err;

	err = lib_prehooks(lib);
	if (err)
		return err;

	err = lib_load(lib);
	if (err)
		return err;

	err = lib_execute(lib);
	if (err)
		return err;

	err = lib_close(lib);
	if (err)
		return err;

	return lib_posthooks(lib);	
}

static int parse_command(const char *buf, char *name, char *func, char *params)
{
	int ret;

	ret = sscanf(buf, "%s %s %s", name, func, params);
	if (ret < 0)
		return -1;

	return ret;
}

int main(void)
{
	/* TODO: Implement server connection. */
	int ret;	
	struct lib lib;

	
	lib.filename=malloc(sizeof(char)*100);
	lib.funcname=malloc(sizeof(char)*100);
	lib.outputfile=malloc(sizeof(char)*100);
	lib.libname=malloc(sizeof(char)*100);
	
	//printf("%s",lib.outputfile);
	int server_socket;
    int client_socket;
	// fac  accept dupa iau cu recv meajul
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;

    int result;

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_NAME);

    int slen = sizeof(server_addr);

    bind(server_socket, (struct sockaddr *) &server_addr, slen);

    listen(server_socket, 10);

	while (1) {
		//printf("aaa");
		/* TODO - get message from client */
		/* TODO - parse message with parse_command and populate lib */
		/* TODO - handle request from client */
		parse_command(buffer)
		char *buffer=malloc(1024);
		//trb sa pun in structura datele si sa d au malloc al structura
        int clen = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
        recv(client_socket, buffer, 1024,0);
		printf("%s",buffer);
        //printf("\nServer: I recieved %c from client!\n", ch);
        //ret = lib_run(&lib);
		close(client_socket);
	}	

	return 0;
}
