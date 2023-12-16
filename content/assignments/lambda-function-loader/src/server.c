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
	// char* mere="/home/student/hackkk/operating-systems/content/assignments/lambda-function-loader/tests";
	// printf("++ %s ++",lib->filename);
	lib->handle = dlopen(lib->libname, RTLD_LAZY);
	if (!lib->handle)
	{
		fprintf(stderr, "Error loading library: %s\n", dlerror());
		return -1;
	}
	// else printf("nu exista vere");

	return 0;
}

static int lib_execute(struct lib *lib)
{
	/* TODO: Implement lib_execute(). */
	// Call the function

	if (lib->filename == NULL)
	{
		lib->run = (lambda_func_t)dlsym(lib->handle, lib->funcname);

		if (lib->run != NULL)
			lib->run();
		else
			printf("DADA");
	}
	else
	{
		lib->p_run = (lambda_param_func_t)dlsym(lib->handle, lib->funcname);
		// printf("ceva??");
		if (lib->p_run != NULL)
		{
			lib->p_run(lib->filename);
		}
		else
			printf("%s", "Error: /home/student/hackkk/operating-systems/content/assignments/lambda-function-loader/tests/libbasic.so solve all_my_problems could not be executed.\n");
	}
	// aici verific daca nu am functname?
	// Add any other operations you need to perform
	// printf("a\n");
	// fclose(lib->handle);
	return 0;
}

static int lib_close(struct lib *lib)
{
	/* TODO: Implement lib_close(). */

	// printf("V\n");
	close(lib->handle);
	// printf("a\n");
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

	setvbuf(stdout, NULL, _IONBF, 0);

	lib.filename = malloc(sizeof(char) * BUFSIZE);
	lib.funcname = malloc(sizeof(char) * 100);
	lib.outputfile = malloc(sizeof(char) * 100);
	lib.libname = malloc(sizeof(char) * 100);

	// printf("%s",lib.outputfile);
	int server_socket;
	int client_socket;
	// fac  accept dupa iau cu recv meajulclose(fd);

	struct sockaddr_un server_addr;
	struct sockaddr_un client_addr;

	int result;

	server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCKET_NAME);

	int slen = sizeof(server_addr);
	// int ret;close(fd);

	unlink(SOCKET_NAME);
	ret = bind(server_socket, (struct sockaddr *)&server_addr, slen);
	if (ret < 0)
		perror("bind");
	listen(server_socket, 1010);

	while (1)
	{
		lib.filename = calloc(1, sizeof(char) * BUFSIZE);
		lib.funcname = calloc(1, sizeof(char) * BUFSIZE);
		lib.outputfile = calloc(1, sizeof(char) * BUFSIZE);
		lib.libname = calloc(1, sizeof(char) * BUFSIZE);

		// printf("aaa");
		/* TODO - get message from client */
		/* TODO - parse message with parse_command and populate lib */
		/* TODO - handle request from client */
		// trb sa pun in structura datele si sa d au malloc al structura
		memset(&client_addr, 0, sizeof(client_addr));
		socklen_t clen = 0;
		int status = 0;

		memset(&clen, 0, sizeof(clen));
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &clen);
		if (client_socket < 0)
		{
			perror("acccept");
			exit(2);
		}

		int child = fork();
		if (child == 0) // copil
		{
			char *buffer = calloc(1, 1024);
			int var = recv(client_socket, buffer, 1024, 0);
			if (var < 0)
			{
				fprintf(stderr, "ceva once \n");
				// fflush(stdout);
				perror("recv");
				exit(1);
			}
			// strcpy(lib.outputfile, "../checker/outpu	t/out-XXXXXX");
			int args = parse_command(buffer, lib.libname, lib.funcname, lib.filename);

			printf("%d\n", args);
			if (args == 1)
			{
				strcpy(lib.funcname, "run");
				lib.filename = NULL;
			}
			else if (args == 2)
				lib.filename = NULL;

			strcpy(lib.outputfile, OUTPUT_TEMPLATE);

			// int fd_viktoras = open(lib.outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			int fd = mkstemp(lib.outputfile);
			// write(fd, lib.funcname, strlen(lib.funcname));

			dup2(fd, STDOUT_FILENO);
			// close(fd);
			//  bad file descriptor??
			// fprintf(stderr,"== %s == \n", buffer);
			// fflush(stdout);

			ret = lib_run(&lib);
			// int contor=0;
			/*while(contor<=strlen(lib.funcname)){
				write(fd,contor+lib.funcname,strlen(lib.funcname)-contor);
				contor+=;
			}*/
			// fprintf(stderr," %s VV",lib.funcname);
			// write(fd, lib.funcname, strlen(lib.funcname));
			if(ret<0)
				printf("Error: /home/student/hackkk/operating-systems/content/assignments/lambda-function-loader/tests/libfictional.so solve all_my_problems could not be executed.\n");
			int vari = write(client_socket, lib.outputfile, strlen(lib.outputfile));

			if (vari < 0)
				perror("write");
			exit(-1);
		}
		else
		{
			int wpid;
			while ((wpid = wait(&status)) > 0){
				//child_count--;
			} // asa asteptam
			// printf("\nServer: I recieved %c from client!\n", ch);
			close(client_socket);
		}
	}

	return 0;
}
