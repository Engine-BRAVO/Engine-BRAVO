#include "NetworkClient.h"
#include "Network/NetworkInformation.h"
#include "Network/NetworkObject.h"

#include "Engine/EngineBravo.h"

#include <iostream>
#include <regex>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>

NetworkClient::NetworkClient()
    : mClient(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance), mIsConnected(false),
      mIsConnecting(false), mServerAddress("0.0.0.0"), mGameObjects(nullptr) {
    // std::cout << "Client Address: " << mClient->GetLocalIP(0) << std::endl;
    // SLNet::SocketDescriptor sd(CLIENT_PORT, mClient->GetLocalIP(0));
    SLNet::SocketDescriptor sd(CLIENT_PORT, 0);
    sd.socketFamily = AF_INET;
    SLNet::StartupResult result = mClient->Startup(1, &sd, 1);
    if (result != SLNet::RAKNET_STARTED) {
        throw std::runtime_error("Failed to start client");
    }
    std::cout << "Client started\n";
}

NetworkClient::~NetworkClient() {
    if (mClient->IsActive()) {
        mClient->CloseConnection(mClient->GetMyGUID(), true);
        mClient->Shutdown(100);
    }
}

void NetworkClient::connectToServer() {
    if (mIsConnected || mIsConnecting) {
        return;
    }
    if (mServerAddress == "0.0.0.0") {
        throw std::runtime_error("Server address not set");
    }
    SLNet::ConnectionAttemptResult result = mClient->Connect(mServerAddress.c_str(), SERVER_PORT, nullptr, 0);
    if (result != SLNet::CONNECTION_ATTEMPT_STARTED) {
        throw std::runtime_error("Failed to start connection attempt");
    }
}

void NetworkClient::sendGameState() { throw std::runtime_error("NetworkClient::sendGameState() not implemented"); }

void NetworkClient::receiveGameState() {
    throw std::runtime_error("NetworkClient::receiveGameState() not implemented");
}

void NetworkClient::update(std::vector<GameObject*>& aGameObjects) {
    mGameObjects = &aGameObjects;
    if (!mClient->IsActive()) {
        throw std::runtime_error("Client is not running");
    }
    handleIncomingPackets();
}

void NetworkClient::discoverServers() {
    mClient->Ping("255.255.255.255", SERVER_PORT, false); // Send discovery ping to the broadcast address
    mServerAddresses.clear();
}

std::vector<std::string>& NetworkClient::getServerAddresses() { return mServerAddresses; }

void NetworkClient::setServerAddress(std::string aServerAddress) {
    const std::regex ipPattern(
        R"(\b((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\b)");
    if (!std::regex_match(aServerAddress, ipPattern)) {
        throw std::runtime_error("Invalid IP address");
    }

    mServerAddress = aServerAddress;
}

bool NetworkClient::isConnected() const { return mIsConnected; }

void NetworkClient::sendPackets() { sendTransform(); }

void NetworkClient::requestPlayerInstantiation() {
    SLNet::BitStream bs;
    bs.Write((SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT);
    mClient->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkClient::handleIncomingPackets() {
    SLNet::Packet* packet;
    for (packet = mClient->Receive(); packet; mClient->DeallocatePacket(packet), packet = mClient->Receive()) {
        SLNet::BitStream bs(packet->data, packet->length, false);
        switch (packet->data[0]) {
        case ID_CONNECTION_REQUEST_ACCEPTED:
            std::cout << "Connected to server.\n";
            mIsConnected = true;
            mIsConnecting = false;
            requestPlayerInstantiation();
            break;
        case ID_CONNECTION_ATTEMPT_FAILED:
            std::cout << "Connection attempt failed. Retrying...\n";
            mIsConnecting = false;
            mIsConnected = false;
            break;
        case ID_CONNECTION_LOST:
            std::cout << "Connection lost or disconnected.\n";
            mIsConnected = false;
            break;
        case ID_UNCONNECTED_PONG: {
            std::cout << "Got pong from " << packet->systemAddress.ToString() << std::endl;
            bs.IgnoreBytes(sizeof(SLNet::MessageID));
            std::string serverIp;
            serverIp = packet->systemAddress.ToString(false);
            mServerAddresses.push_back(serverIp);
            break;
        }
        case ID_UNCONNECTED_PING: {
            std::cout << "Got ping from " << packet->systemAddress.ToString() << std::endl;
        }
        case ID_UNCONNECTED_PING_OPEN_CONNECTIONS: {
            std::cout << "Got open connection ping from " << packet->systemAddress.ToString() << std::endl;
        }
        case (SLNet::MessageID)NetworkMessage::ID_TRANSFORM_PACKET:
            std::cout << "Received transform packet\n";
            handleTransform(packet);
            break;
        case (SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT:
            std::cout << "Received player init packet\n";
            handlePlayerInstantiation(packet);
            break;
        default:
            std::cout << "Message with identifier " << packet->data[0] << " has arrived.\n";
            break;
        }
    }
}

void NetworkClient::sendTransform() {
    // std::vector<NetworkObject*> networkObjects = EngineBravo::getInstance().getNetworkManager().getGameObjects();
}

void NetworkClient::handleTransform(SLNet::Packet* aPacket) {
    throw std::runtime_error("NetworkClient::handleTransform() not implemented");
    // SLNet::BitStream bs(aPacket->data, aPacket->length, false);
    // bs.IgnoreBytes(sizeof(SLNet::MessageID));
    // for (auto& gameObject : *mGameObjects) {
    //     bs.Read(gameObject->mPosition.x);
    //     bs.Read(gameObject->mPosition.y);
    //     bs.Read(gameObject->mPosition.z);
    // }
}

void NetworkClient::handlePlayerInstantiation(SLNet::Packet* aPacket) {
    SLNet::RakNetGUID playerID;
    SLNet::BitStream bs(aPacket->data, aPacket->length, false);
    bs.IgnoreBytes(sizeof(SLNet::MessageID));
    bs.Read(playerID);

    GameObject* player = EngineBravo::getInstance().getNetworkManager().instantiatePlayer(playerID); // Instantiate
                                                                                                     // client-side
                                                                                                     // player
    std::vector<NetworkObject*> networkObjects = player->getComponents<NetworkObject>();
    if (networkObjects.size() == 0) {
        throw std::runtime_error("Player does not have a NetworkObject component");
    }
    std::cout << "Player ID:" << playerID.ToString() << std::endl;
    std::cout << "Client ID:" << mClient->GetMyGUID().ToString() << std::endl;
    if (playerID == mClient->GetMyGUID()) {
        networkObjects[0]->setOwner(true); // This client owns the player object
    } else {
        networkObjects[0]->setOwner(false); // This client does not own the player object
    }
}