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

    // Create a message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    // Get input from the user
    printf("Enter message: ");
    fgets(message.message, sizeof(message.message), stdin);

    // Set the message type
    message.msg_type = 1;

    // Send the message
    if (msgsnd(msgid, &message, sizeof(message.message), 0) == -1) {
        perror("msgsnd");
        return 1;
    }

    printf("Message sent.\n");

    return 0;
}
