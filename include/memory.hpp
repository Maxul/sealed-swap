#ifndef VRAM_MEMORY_HPP
#define VRAM_MEMORY_HPP

/*
 * ERAM block allocation
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <memory>
// #include <vector>

using namespace std;

namespace vram {
    namespace memory {
        class block;
        typedef std::shared_ptr<block> block_ref;

        // Check if current machine supports VRAM allocation
        bool is_available();

        // Set the device to use
        void set_device(size_t num);

        // Returns a list of device names
        // std::vector<std::string> list_devices();

        // Total blocks and blocks currently free
        int pool_size();
        int pool_available();

        // Allocate pool of memory blocks, returns actual amount allocated (in bytes)
        size_t increase_pool(size_t size);

        // Get a new block of memory from the pool, returns nullptr if pool is empty
        block_ref allocate();

        /*
         * Block of allocated VRAM
         */

        class block : public std::enable_shared_from_this<block> {
            friend block_ref allocate();

        public:
            // Best performance/size balance
            static const size_t size = 128 * 1024;

            block(const block& other) = delete;

            ~block();

            void read(off_t offset, size_t size, void* data) const;

            // Data may be freed afterwards, even if called with async = true
            void write(off_t offset, size_t size, const void* data, bool async = false);

            // Wait for all writes to this block to complete
            void sync();

        private:
            void * buffer;
            // True until first write (until then it contains leftover data from last use)
            bool dirty = true;

            block();
        };
    }
}

#endif
