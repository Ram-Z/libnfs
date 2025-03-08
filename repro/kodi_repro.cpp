#include <cassert>
#include <string>
#include <iostream>

#include <nfsc/libnfs.h>

#define PPRINT(x) std::cout << #x "=" << (x) << "\n"

struct nfs_context *m_pNfsContext;

void setOptions(struct nfs_context* context) {
    auto timeout = -1;  // from m_nfsTimeout
    nfs_set_timeout(context, timeout);

    auto retries = 3; // from m_nfsRetries;
    nfs_set_autoreconnect(context, retries);

    auto nfsVersion = 3;  // from GetSettings()
    int ret = nfs_set_version(context, nfsVersion);
    assert(ret == 0 && "failed to set version");
}

void getContextForExport(std::string exportname)
{
    if (!m_pNfsContext)
    {
        m_pNfsContext = nfs_init_context();
        assert(m_pNfsContext && "nfs_init_context failed");

        setOptions(m_pNfsContext);
    }
}

void Connect(std::string server, std::string path)
{
    getContextForExport(server);

    auto nfsRet = nfs_mount(m_pNfsContext, server.c_str(), path.c_str());
    assert(nfsRet == 0 && "failed to mount");

    auto m_readChunkSize = nfs_get_readmax(m_pNfsContext);
    auto m_writeChunkSize = nfs_get_writemax(m_pNfsContext);

    PPRINT(m_readChunkSize);
    PPRINT(m_writeChunkSize);
}

int main(int argc, char* argv[]) {
    std::string server = argv[1];
    std::string path = argv[2];
    Connect(server, path);
    assert(m_pNfsContext && "no context");
}
