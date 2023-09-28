


int main(int argc, char** argv) {
    int ID = atoi(argv[1]);

    //make a while loop to read using ftok()
    /*
                key_t ftok(const char *pathname, int proj_id);
                https://man7.org/linux/man-pages/man3/ftok.3.html
    
        the read portion for each reader should be pointing to
        shared_mem_start + 1 + ID

        readFlag = (uint8_t)shmID + 1 + (uint8_t)ID;
    */


    char* keyPath = ".key"
    key_t key = ftok(keyPath, 7);



    return 0;
}