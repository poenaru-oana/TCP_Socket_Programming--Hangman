# TCP_Socket_Programming--Hangman
 
How it works:

Server reads a word from keyboard, and then tells the client the number of letters within it. The latter has to guess the word with less than 8 wrong letter guesses.

After each letter guess, server gives the client feedback:

- If client guessed the letter correctly, the server will return the word with the letter filled in the blanks where it belongs, accompanied by a corresponding message.
- If client did not guess the letter correctly, the server will return the word with as it was, alongside the number of tries left, accompanied by a corresponding message.
- If client correctly guessed the word, the server will send back a win message and close the connection.
- If client used up all of their remaining tries, by guessing 8 wrong letters in total, the server wil lsend back a lose message, and close the connection.

The program validates the sent and received data, has a time_out function, and, when necessary, provides error messages and exits with error code 1.

	The server side of the connection was written in C, on a VM, in an Ubuntu server, using vim.
	The client side of the connection was written in Python, on host machine, in Windows, using PyCharm.
	The protocol used for the connection is TCP.

This project was realised in preparation for a laboratory test taken for the Computer Networks course in Semester 3.