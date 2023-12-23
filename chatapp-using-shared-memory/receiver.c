#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msg {
    long msg_type;
    char message[100];
};

int main() {
    key_t key;
    int msgid;
    struct msg message;

    // Generate a key
    key = ftok("keyfile", 65);
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    // Create a message queue (Note: Use the same key as the sender)
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    // Receive the message
    if (msgrcv(msgid, &message, sizeof(message.message), 1, 0) == -1) {
        perror("msgrcv");
        return 1;
    }

    // Display the received message
    printf("Message received: %s\n", message.message);

    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        return 1;
    }

    return 0;
}
