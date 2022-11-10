#include "memory.hpp"
#include <vector>

extern int backing_file;

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

#define FS_BLKSIZE (4096)

        void block::read(off_t offset, size_t size, void* data) const {
            // printf("%s %d, size: %ld\n", __func__, __LINE__, size);
            memcpy(data, (char *)buffer + offset, size);

            #if 0
            void *buf;
            int ret;
            ret = posix_memalign((void **)&buf, FS_BLKSIZE, size);
            if (ret) {
                perror("posix_memalign failed");
                abort();
            }
            ret = pread(backing_file, buf, size, offset);
            // printf("pread ret %d size 0x%x offset 0x%x\n", ret, size, offset);
            memcpy(data, buf, size);
            free(buf);
            #endif

            #if 0
            if (dirty) {
                memset(data, 0, size);
            } else {
                // Queue is configured for in-order execution, so writes before this
                // are guaranteed to be completed first
            }
            #endif
        }

        void block::write(off_t offset, size_t size, const void* data, bool async) {
            // printf("%s %d, size: %ld, async %d\n", __func__, __LINE__, size, async);
            memcpy((char *)buffer + offset, data, size);

            #if 0
            void *buf;
            int ret;
            ret = posix_memalign((void **)&buf, FS_BLKSIZE, size);
            if (ret) {
                perror("posix_memalign failed");
                abort();
            }
            memcpy(buf, data, size);
            ret = pwrite(backing_file, buf, size, offset);
            // printf("pwrite ret %d size 0x%x offset 0x%x\n", ret, size, offset);
            free(buf);
            #endif

            #if 0
            // If this block has not been written to yet, and this call doesn't
            // overwrite the entire block, clear with zeros first
            if (dirty && size != block::size) {
            }

            if (async) {
                char* data_copy = new char[size];
                memcpy(data_copy, data, size);
                data = data_copy;
            }

            dirty = false;
            #endif
        }

        void block::sync() {
        }
    }
}
