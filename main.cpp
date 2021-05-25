#include <iostream>
#include <memory>

struct Object {
    int x, y, z;
};

struct AllocMetrics
{
    uint32_t TotalAllocated = 0;
    uint32_t TotalFreed = 0;

    [[nodiscard]] uint32_t CurrentUsage() const { return TotalAllocated - TotalFreed; }
};

static AllocMetrics s_AllocMetrics;

void* operator new (size_t size)
{
//    std::cout << "Allocating " << size << " bytes\n";
    s_AllocMetrics.TotalAllocated += size;

    return malloc(size);
}

void operator delete (void* memory, size_t size)
{
//    std::cout << "Freeing " << size << " bytes\n";
    s_AllocMetrics.TotalFreed += size;
    free(memory);
}

static void printMemoryUsage()
{
    std::cout << "Memory Usage: " << s_AllocMetrics.CurrentUsage() << " bytes\n";
}

int main() {
    // before allocating a string
    {
        printMemoryUsage();
        std::string str = "1234567890123456";
        std::cout << str << "\n";
        printMemoryUsage();
        {
            std::unique_ptr<Object> obj = std::make_unique<Object>();
            printMemoryUsage();
        }
        // after freeing unique
        printMemoryUsage();
    }
    printMemoryUsage();
    return 0;
}
