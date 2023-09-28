


int main(int argc, char** argv) {
    int ID = atoi(argv[1]);

    //make a while loop to read using ftok()
    /*
                key_t ftok(const char *pathname, int proj_id);
                https://man7.org/linux/man-pages/man3/ftok.3.html
    

    */


   

    char* keyPath = ".key"
    key_t key = ftok(keyPath, 7);



    return 0;
}