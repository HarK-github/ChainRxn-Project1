#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// TODO: Implement a simple hash function or use a real one
void simple_hash(const char* input, char* output) {
    unsigned int hash = 0xabcdef01;

    for (int i = 0; input[i] != '\0'; ++i) {
        hash ^= (unsigned char)(input[i]);
        hash = (hash << 5) | (hash >> (32 - 5)); // Rotate left 5 bits
        hash += 0x9e3779b9; // Magic constant
    }

    const char* hexDigits = "0123456789abcdef";
    for (int i = 0; i < 8; ++i) {
        output[i] = hexDigits[(hash >> (28 - i * 4)) & 0xF];
    }
    output[8] = '\0';
}

// Block structure
typedef struct Block {
    int index;
    char timestamp[100];
    char data[256];
    char previousHash[256];
    char hash[256];
    struct Block* next;
} Block;

// TODO: Fill this to get current time in string format
void get_timestamp(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", tm_info);
}

// TODO: Implement logic to create a new block
Block* create_block(int index, char *data, char* previousHash) {
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->index = index;
    get_timestamp(newBlock->timestamp);
    strcpy(newBlock->data, data);
    strcpy(newBlock->previousHash, previousHash);
    newBlock->next = NULL;

    // Create a string from block data to hash
    char toHash[1024];
    snprintf(toHash, sizeof(toHash), "%d%s%s%s", index, newBlock->timestamp, data, previousHash);

    // Call simple_hash on the string and store in block->hash
    simple_hash(toHash, newBlock->hash);

    return newBlock;
}

// TODO: Implement logic to add a block to the chain
void add_block(Block **head, char *data) {
    if (!(*head)) {
        *head = create_block(0, data, "0");
        return;
    }

    Block* current = *head;
    int index = 0;
    while (current->next != NULL) {
        current = current->next;
        index++;
    }

    Block* newBlock = create_block(index + 1, data, current->hash);
    current->next = newBlock;
}

// TODO: Print all blocks in the blockchain
void print_chain(Block *head) {
    Block* current = head;
    while (current != NULL) {
        printf("Block #%d\n", current->index);
        printf("Timestamp: %s\n", current->timestamp);
        printf("Data: %s\n", current->data);
        printf("Previous Hash: %s\n", current->previousHash);
        printf("Hash: %s\n", current->hash);
        printf("---------------------------\n");
        current = current->next;
    }
}

// Free all blocks in the chain
void free_chain(Block* head) {
    while (head != NULL) {
        Block* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Block* blockchain = NULL;

    // TODO: Add a few blocks with sample data
    add_block(&blockchain, "Genesis Block");
    add_block(&blockchain, "Transaction A -> B: 5 BTC");
    add_block(&blockchain, "Transaction B -> C: 2 BTC");

    // TODO: Print the chain
    print_chain(blockchain);

    // TODO: Free the chain
    free_chain(blockchain);

    return 0;
}
