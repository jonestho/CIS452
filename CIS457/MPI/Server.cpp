#include "Server.hpp"

#define PORT "5000"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    std::vector<std::vector<int>> numbers;
    int dest = 0;
    char* port = (char*)calloc(sizeof(char), 5);
    strcpy(port, PORT);
    int client_sockfd;
    int rank, size;
    MPI_Info info;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the current process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the total number of processes

    MPI_Info_create(&info); // Create an info object

    // MPI_Comm server_comm, client_comm;
    // MPI_Comm_create(MPI_COMM_WORLD, MPI_GROUP_EMPTY, &server_comm); // Create a new communicator for the server
    // MPI_Comm_create(MPI_COMM_WORLD, MPI_GROUP_EMPTY, &client_comm); // Create a new communicator for the client

    // MPI_Group server_group, client_group;
    // MPI_Comm_group(MPI_COMM_WORLD, &server_group); // Get the group of the parent communicator
    // MPI_Group_incl(server_group, 1, &rank, &client_group); // Create a new group from the parent group

    // MPI_Comm_group(MPI_COMM_WORLD, &client_group); // Get the group of the parent communicator
    // MPI_Group_incl(client_group, 1, &rank, &server_group); // Create a new group from the parent group

    if (rank == 0) {
        // MPI_Open_port(info, port); // Open a port for the server to listen on
        std::cout << "Server is running..." << std::endl;
        std::cout << "Server rank: " << rank << std::endl;
        std::cout << "Server size: " << size << std::endl;
        listenForClient(size);
        exit(0);
    }
    if (rank == 1) {
        MPI_Recv(&client_sockfd, 5, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // Listen for a client
    while (true) {
        if (rank == 1) { // thread to send to client
            std::vector<int> buffer(5);
            MPI_Recv(buffer.data(), 5, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (buffer[1] < 0) {
                break;
            }
            std::cout << "Sending to client: " << buffer[0] << " " << buffer[1] << " " << buffer[2] << " " << buffer[3] << " " << buffer[4] << " " << buffer[5] << std::endl;
            write(client_sockfd, buffer.data(), sizeof(int) * 5);
        }
        else { // thread to receive jobs
            std::vector<int> buffer(2);
            MPI_Recv(buffer.data(), 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // if payload is -1, then we are done
            if (buffer[1] < 0) {
                break;
            }

            std::vector<int> factors(5);
            memcpy(factors.data(), getPrimeFactors(buffer[0], buffer[1]), sizeof(int) * 5);
            printf("Rank: %d, Number: %d, Factors: %d %d %d %d %d\n", rank, buffer[1], factors[1], factors[2], factors[3], factors[4], factors[5]);
            MPI_Send(factors.data(), 5, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
    }

    return 0;
}


void listenForClient(int ranks) {
    int sockfd, newsockfd, size, dest;
    uint clilen;
    struct sockaddr_in server_addr, client_addr;
    char* buffer = (char*)calloc(sizeof(char), 100);

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
    // MPI_Send(&newsockfd, 5, MPI_INT, 1, 0, MPI_COMM_WORLD);
    std::cout << "Client connected" << std::endl;

    while (1) {
        bzero(buffer, sizeof(char) * 100);
        // if((read(newsockfd, buffer, sizeof(char) * 9)) < 0) {
            // perror("ERROR reading from socket");
            // exit(1);
        // }
        read(newsockfd, buffer, sizeof(char) * 99);
        buffer[9] = '\0';
        printf("Here is the message: %s\n", buffer);
        std::vector<int> nums = parseMessage(buffer);
        std::cout << "Received: " << nums[0] << " " << nums[1] << std::endl;
        //send the jobs out
        dest = (dest++ == ranks-1) ? 2 : dest;
        std::cout << "Sending to rank: " << dest << std::endl;
        MPI_Send(nums.data(), 2, MPI_INT, dest, 0, MPI_COMM_WORLD);
        if (nums[1] < 0) {
            free(buffer);
            break;
        }
    }


}

int* getPrimeFactors(int ID, int number) {
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

std::vector<int> parseMessage(char* buffer) {
    std::vector<int> numbers;
    char* token = strtok(buffer, " ");
    numbers.push_back(atoi(token));
    token = strtok(NULL, " ");
    numbers.push_back(atoi(token));
    return numbers;

}