#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int c;

//timeout mechanism
void time_out(int signal) {

	int32_t result = -1;
	result = htonl(result);

	printf("TIME OUT.\n");

	send(c, &result, sizeof(int32_t), 0);

	close(c);
	exit(1);
}

void handle_client(char word[]) {
	int wrong = 0, rem = 8;
	char client_word[20];
	char msg[50];
	char ch;
	int code;
	bool correct;
	int filled = 0;
	int32_t len = 0;

	if (c < 0) {
		fprintf(stderr, "ERROR ON CONNECTING TO THE CLIENT!\n");
		exit(1);
	}

	signal(SIGALRM, time_out);
	alarm(5);

	for (int i = 0; i < strlen(word) - 1; i++)
		client_word[i] = '_';

	strcpy(msg, "Guess the word. You have 8 tries remaining.\n");

	len = strlen(msg) + 1;
	code = send(c, &len, sizeof(int32_t), 0);
	code = send(c, &msg, len, 0);

	len = strlen(client_word) - 1;
	code = send(c, &len, sizeof(int32_t), 0);
	code = send(c, &client_word, len, 0);

	while (wrong < 8 && filled < strlen(word) - 1) {

		code = recv(c, &ch, 1, 0);
		if (code < 0 || code != 1) {
			strcpy(msg, "ERROR ON RECEIVING LETTER FROM CLIENT!\n");
			break;
		}

		alarm(5);
		correct = 0;
		for (int i = 0; i < strlen(word) - 1; i++) {
			if (word[i] == ch) {
				client_word[i] = ch;
				correct = 1;
				filled++;

				if (filled >= strlen(word) - 1)
					break;
				continue;
			}

			if (!(client_word[i] >= 'a' && client_word[i] <= 'z')) {
				client_word[i] = '_';
			}
		}
		
		if (correct == 0) {
			wrong++;
			rem--;
			snprintf(msg, sizeof(msg), "Letter not in word. %d tries remaining.\n", rem);
		}
		else if (correct) {
			snprintf(msg, sizeof(msg), "Discovered new letter!\n");
		}

		len = strlen(msg) + 1;
		code = send(c, &len, sizeof(int32_t), 0);
		code = send(c, &msg, len, 0);

		len = strlen(client_word) - 1;
		code = send(c, &len, sizeof(int32_t), 0);
		code = send(c, &client_word, len, 0);
	}

	if (filled == strlen(word) - 1) {
		alarm(0);
		strcpy(msg, "CONGRATULATIONS! You have guessed the word!\n");
		printf("Client won.\n");

		len = strlen(msg) + 1;
		code = send(c, &len, sizeof(int32_t), 0);
		code = send(c, &msg, len, 0);

		len = strlen(client_word) - 1;
		code = send(c, &len, sizeof(int32_t), 0);
		code = send(c, &client_word, len, 0);

		close(c);
		exit(0);
	}

	if (wrong == 8) {
		alarm(0);
		strcpy(msg, "YOU LOST!\nWord was: ");
		printf("Client lost.\n");

		len = strlen(msg) + 1;
		code = send(c, &len, sizeof(int32_t), 0);
		code = send(c, &msg, len, 0);

		len = strlen(client_word) - 1;
		code = send(c, &len, sizeof(int32_t), 0);
		code = send(c, &client_word, len, 0);

		close(c);
		exit(0);
	}

	len = strlen(msg) + 1;
	code = send(c, &len, sizeof(int32_t), 0);
	code = send(c, &msg, len, 0);
	close(c);
	exit(1);
}

int main() {
	int s;
	int code;
	char hangman[9][6];
	char word[20];
	int32_t wLen;

	struct sockaddr_in server, client;

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		fprintf(stderr, "ERROR ON CREATE SERVER SOCKET!\n");
		return 1;
	}

	memset(&server, 0, sizeof(server));
	server.sin_port = htons(1500);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;

	code = bind(s, (struct sockaddr*)&server, sizeof(server));
	if (code < 0) {
		fprintf(stderr, "ERROR ON BIND SERVER SOCKET!\n");
		return 1;
	}

	printf("Word for the client to guess: ");
	fgets(word, sizeof(word), stdin);

	listen(s, 4);

	while (1) {

		memset((struct sockaddr*)&client, 0, sizeof(client));

		printf("Waiting for connection...");

		int len = sizeof(client);
		c = accept(s, (struct sockaddr*)&client, (socklen_t*)&len);

		printf("Incoming from %s:%d\n", inet_ntoa(client.sin_addr), nthos(client.sin_port));

		if (fork() == 0) {
			handle_client(word);
		}
	}
}
