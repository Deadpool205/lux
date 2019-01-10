#ifndef LUX_STORAGECONTROLLER_H
#define LUX_STORAGECONTROLLER_H

#include "uint256.h"
#include "net.h"
#include "protocol.h"
#include "storageheap.h"
#include "storageorder.h"
#include "proposalsagent.h"

class StorageController;

extern StorageController storageController;

static const size_t STORAGE_MIN_RATE = 1;

static const unsigned short DEFAULT_DFS_PORT = 1507;

class StorageController
{
private:
    StorageHeap storageHeap;
    StorageHeap tempStorageHeap;
    ProposalsAgent proposalsAgent;
public:
    size_t rate;
    CService address;
    std::map<uint256, boost::filesystem::path> mapLocalFiles;
    std::map<uint256, StorageOrder> mapAnnouncements;
    std::map<uint256, StorageHandshake> mapReceivedHandshakes;

    StorageController() : rate(STORAGE_MIN_RATE), address(CService("127.0.0.1")) {} // TODO: Change to global-wide address (SS)

    void AnnounceOrder(const StorageOrder &order);
    void AnnounceOrder(const StorageOrder &order, const boost::filesystem::path &path);
    std::vector<StorageProposal> SortProposals(const StorageOrder &order);
    void StartHandshake(const StorageProposal &proposal);
    void ClearOldAnnouncments(std::time_t timestamp);
    void ProcessStorageMessage(CNode* pfrom, const std::string& strCommand, CDataStream& vRecv, bool& isStorageCommand);
    // Proposals Agent
    void ListenProposal(const uint256 &orderHash);
    void StopListenProposal(const uint256 &orderHash);
    bool isListen(const uint256 &proposalHash);
    std::vector<StorageProposal> GetProposals(const uint256 &orderHash);
    // Temp
    void FindReplicaKeepers(const StorageOrder &order, const int countReplica);
    void CreateReplica(const boost::filesystem::path &filename, const StorageOrder &order, const StorageProposal &proposal);
};

#endif //LUX_STORAGECONTROLLER_H