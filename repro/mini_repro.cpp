#include <cassert>
#include <string>
#include <iostream>

#include <nfsc/libnfs.h>

#define PPRINT(x) std::cout << #x "=" << (x) << "\n"

void connect(std::string server, std::string path, int timeout)
{
    struct nfs_context *context = nfs_init_context();
    assert(context && "nfs_init_context failed");

    nfs_set_timeout(context, timeout);

    auto retries = 3; // from m_nfsRetries;
    nfs_set_autoreconnect(context, retries);

    auto nfsVersion = 3;  // from GetSettings()
    int ret = nfs_set_version(context, nfsVersion);
    assert(ret == 0 && "failed to set version");

    auto nfsRet = nfs_mount(context, server.c_str(), path.c_str());
    assert(nfsRet == 0 && "failed to mount");

    auto m_readChunkSize = nfs_get_readmax(context);
    auto m_writeChunkSize = nfs_get_writemax(context);

    PPRINT(m_readChunkSize);
    PPRINT(m_writeChunkSize);
}

int main(int argc, char* argv[]) {
    std::string server = argv[1];
    std::string path = argv[2];
    int timeout = atoi(argv[3]);
    connect(server, path, timeout);
}
