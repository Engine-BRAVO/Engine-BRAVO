#ifndef NETWORKSHAREDFUNCTIONS_H
#define NETWORKSHAREDFUNCTIONS_H

#include "Network/NetworkInformation.h"

#include <chrono>
#include <slikenet/BitStream.h>

class NetworkSharedFunctions {
public:
private:
    friend class NetworkServer;
    friend class NetworkClient;

private:
    static void makeBitStream(SLNet::BitStream& aBitStream);

    static NetworkPacket getBitStreamData(SLNet::BitStream& aBitStream);
    static void setBitStreamNetworkPacket(SLNet::BitStream& aBitStream, const NetworkPacket& aNetworkPacket);
};

#endif // NETWORKSHAREDFUNCTIONS_H