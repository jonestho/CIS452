#include "Server.hpp"

#define PORT "5000"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Info info;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the current process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the total number of processes

    MPI_Info_create(&info); // Create an info object


    if (rank == 0) {
        std::cout << "Server is running..." << std::endl;
        std::cout << "Server rank: " << rank << std::endl;
        std::cout << "Server size: " << size << std::endl;
        runServer(listenForClient(), size);
    }
    else {
        while (true) {

            uint64_t* buffer = (uint64_t*)calloc(sizeof(uint64_t), 2);
            int* factors;

            MPI_Recv(buffer, 2, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // if payload is -1, then we are done
            if (buffer[0] == 0) {
                return 0;
            }

            // get the prime factors
            factors = getPrimeFactors(buffer[0], buffer[1]);

            // Send the prime factors back to the main thread
            MPI_Send(factors, 5, MPI_INT, 0, 0, MPI_COMM_WORLD);
            free(buffer);
            free(factors);
        }
    }

    return 0;
}


int listenForClient() {
    int sockfd, newsockfd;
    uint clilen;
    struct sockaddr_in server_addr, client_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Failed to create socket" << std::endl;
        exit(1);
    }
    bzero((char*)&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(PORT));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    listen(sockfd, MAX_CLIENTS);
    clilen = sizeof(client_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }
    std::cout << "Client connected" << std::endl;
    return newsockfd;
}


void runServer(int client_sockfd, int ranks) {
    pthread_t thread;
    char* buffer = (char*)calloc(sizeof(char), 100);
    int size, dest = 0;

    if ((pthread_create(&thread, NULL, sendToClient, (void*)&client_sockfd)) < 0) {
        perror("ERROR on thread creation");
        exit(1);
    }


    while (1) {
        bzero(buffer, sizeof(char) * 100);
        if ((read(client_sockfd, buffer, sizeof(char) * 99)) < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
        uint64_t* nums = parseMessage(buffer);
        std::cout << "Received: " << nums[0] << " " << nums[1] << std::endl;

        // exit condition
        if (nums[0] == 0) [[unlikely]] {
            std::cout << "Exiting..." << std::endl;

            // send exit signal to all processes
            for (int i = 1; i < ranks; i++) {
                MPI_Send(nums, 2, MPI_UINT64_T, i, 0, MPI_COMM_WORLD);
            }

            // kill the thread
            if ((pthread_cancel(thread)) < 0) {
                perror("ERROR on thread kill");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }

            close(client_sockfd);
            free(buffer);
            free(nums);
            return;
        }

            //send the jobs out
        dest = (dest++ == ranks - 1) ? 2 : dest;
        std::cout << "Sending to rank: " << dest << std::endl;
        MPI_Send(nums, 2, MPI_UINT64_T, dest, 0, MPI_COMM_WORLD);


        free(nums);
    }
}

int* getPrimeFactors(uint64_t ID, uint64_t number) {
    int* factors = (int*)calloc(sizeof(int), 5);
    factors[0] = ID;

    while (number % 2 == 0) {
        factors[1]++;
        number /= 2;
    }
    while (number % 3 == 0) {
        factors[2]++;
        number /= 3;
    }
    while (number % 5 == 0) {
        factors[3]++;
        number /= 5;
    }
    while (number % 7 == 0) {
        factors[4]++;
        number /= 7;
    }

    return factors;
}

uint64_t* parseMessage(char* buffer) {
    uint64_t* numbers = (uint64_t*)calloc(sizeof(uint64_t), 2);
    char* token = strtok(buffer, " ");
    numbers[0] = atoi(token);
    token = strtok(NULL, " ");
    numbers[1] = atoi(token);
    return numbers;
}


void* sendToClient(void* client_sockfd) {
    int sockfd = *(int*)client_sockfd;

    while (true) {
        int* buffer = (int*)calloc(sizeof(int), 5);
        std::string output;

        MPI_Recv(buffer, 5, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Create string to send
        for (int i = 0; i < 4; i++) {
            output += std::to_string(buffer[i]);
            output += ",";
        }
        output += std::to_string(buffer[4]);

        std::cout << "Sending to client: " << output << std::endl;

        write(sockfd, output.c_str(), strlen(output.c_str()));
    }
    return NULL;
}