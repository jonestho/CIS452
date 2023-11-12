#include "Server.hpp"



int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    std::vector<std::vector<int>> numbers;

    int rank, size;
    MPI_Info info;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the current process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the total number of processes

    MPI_Info_create(&info); // Create an info object

    MPI_Comm server_comm, client_comm;
    MPI_Comm_create(MPI_COMM_WORLD, MPI_GROUP_EMPTY, &server_comm); // Create a new communicator for the server
    MPI_Comm_create(MPI_COMM_WORLD, MPI_GROUP_EMPTY, &client_comm); // Create a new communicator for the client

    MPI_Group server_group, client_group;
    MPI_Comm_group(MPI_COMM_WORLD, &server_group); // Get the group of the parent communicator
    MPI_Group_incl(server_group, 1, &rank, &client_group); // Create a new group from the parent group
    

    if (rank == 0) {
        std::cout << "Server is running..." << std::endl;
        std::cout << "Server rank: " << rank << std::endl;
        std::cout << "Server size: " << size << std::endl;
    }

    // Listen for a client
    int client_sockfd = listenForClient();

    if (rank == 0) {// master thread
        std::cout << "Client connected" << std::endl;
        std::vector<int> buffer(2);
        read(client_sockfd, buffer.data(), sizeof(int) * 2);
        numbers.push_back(buffer);

        //send the jobs out
        MPI_Send(buffer.data(), 2, MPI_INT, 1, 0, server_comm);
        if(buffer[1] < 0) {
            break;
        }

    }
    else { //TODO figure out how to make each job take 1
        std::vector<int> buffer(2);
        MPI_Recv(buffer.data(), 2, MPI_INT, 0, 0, server_comm, MPI_STATUS_IGNORE);

        std::vector<int> factors(5);
        memcpy(factors.data(), getPrimeFactors(buffer[0], buffer[1]), sizeof(int) * 5);

        MPI_Send(factors.data(), 5, MPI_INT, 0, 0, server_comm);
    }


    MPI_Finalize();

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

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        perror("ERROR on binding");
    
    listen(sockfd, MAX_CLIENTS);
    clilen = sizeof(client_addr);

    if ((newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &clilen) < 0))
        perror("ERROR on accept");

    return newsockfd;
}

int* getPrimeFactors(int ID, int number) {

    int* factors = (int*)malloc(sizeof(int) * 5);
    factors[0] = ID;




    return factors;
}