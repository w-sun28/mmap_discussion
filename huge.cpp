#include <chrono>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/mman.h>


int main(int argc, char** argv){
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    srand(439); 

    // No file for this one, our Linux kernel doesn't have support for file
    // backed huge pages. This will be like our naive_mmap(nullptr, length)
    // What is the flag called on Linux?
    int size = 1024 * 1024 * 2;
    void* addr;

    // easy / hacky way to pass cmdline args
    if (argc == 1){
        // use 4k page size if no additional args
        std::cout << "Page size: 4k\n";
        addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0); // TODO
    }
    else{
        // use 2MB page size if additional args
        // Note reserving huge pages requires some setup before you actually
        // run the program. You need special permissions which the CS
        // department was nice enough to give us for the day :D
        //
        // The superpages are already reserved, so you should be able to mmap
        // them as long as one is available (you might have to wait a few
        // seconds if other students are using all of the available superpages).
        //
        // If you want to learn more about this special setup, check out 
        // https://docs.kernel.org/admin-guide/mm/hugetlbpage.html
        std::cout << "Page size: 2MB\n";
        addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, 0, 0); // TODO 
    }

    // Always check your return values!
    if(addr == MAP_FAILED){
        std::cout << "Oh dear, something went wrong with mmap()! " << strerror(errno) << "\n";    
    }

    // TODO: run benchmark here
    auto file = (char*) addr;
    for(int i = 0; i<size; i+=4096){
        file[i] = 'a';
    }

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;


    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s\n";
    munmap(addr, 1);
}

