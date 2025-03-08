#include <array>
#include <cassert>
#include <string>
#include <iostream>

#include <fcntl.h>
#include <nfsc/libnfs.h>

#define check(x) if (!(x)) { std::cerr << "check failed: " #x "\n"; }

#define PPRINT(x) std::cout << #x "=" << (x) << "\n"

struct nfs_context *m_pNfsContext;
struct nfsfh *m_pFileHandle;
auto g_timeout = -1;  // from m_nfsTimeout

void setOptions(struct nfs_context* context) {
    nfs_set_timeout(context, g_timeout);

    auto retries = 0; // from m_nfsRetries;
    nfs_set_autoreconnect(context, retries);

    auto nfsVersion = 3;  // from GetSettings()
    int ret = nfs_set_version(context, nfsVersion);
    check(ret == 0 && "failed to set version");
}

void getContextForExport(std::string exportname)
{
    if (!m_pNfsContext)
    {
        m_pNfsContext = nfs_init_context();
        check(m_pNfsContext && "nfs_init_context failed");

        setOptions(m_pNfsContext);
    }
}

void Connect(std::string server, std::string path)
{
    getContextForExport(server);

    auto nfsRet = nfs_mount(m_pNfsContext, server.c_str(), path.c_str());
    check(nfsRet == 0 && "failed to mount");

    auto m_readChunkSize = nfs_get_readmax(m_pNfsContext);
    auto m_writeChunkSize = nfs_get_writemax(m_pNfsContext);

    PPRINT(m_readChunkSize);
    PPRINT(m_writeChunkSize);
}

void Open(std::string server, std::string path, std::string filename)
{
    Connect(server, path);

    int ret = nfs_open(m_pNfsContext, filename.c_str(), O_RDONLY, &m_pFileHandle);
    check(ret == 0 && "unable to open");
}

ssize_t Read(void *lpBuf, size_t uiBufSize)
{
    auto numberOfBytesRead = nfs_read(m_pNfsContext, m_pFileHandle, lpBuf, uiBufSize);
    check(numberOfBytesRead > 0 && "unable to read");
    std::string str(reinterpret_cast<char*>(lpBuf), numberOfBytesRead);
    PPRINT(str);
    return numberOfBytesRead;
}
int main(int argc, char* argv[]) {
    std::string server = argv[1];
    std::string path = argv[2];
    std::string filename = argv[3];
    g_timeout = atoi(argv[4]);

    Open(server, path, filename);
    std::array<char, 256> buf;
    Read(buf.data(), buf.size());
    check(m_pNfsContext && "no context");
}
