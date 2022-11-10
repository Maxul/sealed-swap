#include "memory.hpp"
#include <vector>

namespace vram {
    namespace memory {
        std::vector<void *> pool;
        int total_blocks = 0;

        bool is_available() {
            // always return true since we use DRAM directly
            return true;
        }

        void set_device(size_t device) {
            return;
        }

        /*
        std::vector<std::string> list_devices() {
            std::vector<std::string> device_names;
            return device_names;
        }
        */

        int pool_size() {
            return total_blocks;
        }

        int pool_available() {
            return pool.size();
        }

        size_t increase_pool(size_t size) {
            int block_count = 1 + (size - 1) / block::size;
            printf("%s %d, size: %ld, block::size %ld, block_count %d\n",
                __func__, __LINE__, size, block::size, block_count);

            for (int i = 0; i < block_count; i++) {
                // use DRAM buffer directly
                void * buf = calloc(1, block::size);
                memset(buf, 0, block::size); // make sure no page faults

                if (nullptr != buf) {
                    pool.push_back(buf);
                    total_blocks++;
                } else {
                    return i * block::size;
                }
            }

            return block_count * block::size;
        }

        block_ref allocate() {
            if (pool.size() != 0) {
                return block_ref(new block());
            } else {
                return nullptr;
            }
        }

        block::block() {
            buffer = pool.back();
            pool.pop_back();
        }

        block::~block() {
            pool.push_back(buffer);
        }

        void block::read(off_t offset, size_t size, void* data) const {
            // printf("%s %d, size: %ld\n", __func__, __LINE__, size);
            memcpy(data, (char *)buffer + offset, size);
        }

        void block::write(off_t offset, size_t size, const void* data, bool async) {
            // printf("%s %d, size: %ld, async %d\n", __func__, __LINE__, size, async);
            memcpy((char *)buffer + offset, data, size);
        }

        void block::sync() {
        }
    }
}
