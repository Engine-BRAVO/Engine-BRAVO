#include "NetworkServer.h"

#include "Network/INetworkBehaviour.h"
#include "Network/NetworkInformation.h"
#include "Network/NetworkObject.h"
#include "Network/NetworkSharedFunctions.h"
#include "Network/NetworkTransform.h"
#include "Network/NetworkVariable.h"

#include "Engine/EngineBravo.h"

#include <iostream>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <slikenet/PacketLogger.h>
#include <slikenet/peerinterface.h>
#include <stdexcept>
#include <string>

NetworkServer::NetworkServer(int aTickRate = 60)
    : mServer(SLNet::RakPeerInterface::GetInstance(), SLNet::RakPeerInterface::DestroyInstance),
      mLastSendPacketsTime(std::chrono::steady_clock::now()), mGameObjects(nullptr), mTickRate(aTickRate) {
    SLNet::SocketDescriptor sd(SERVER_PORT, 0);
    sd.socketFamily = AF_INET;
    SLNet::StartupResult result = mServer->Startup(10, &sd, 1);
    mServer->SetMaximumIncomingConnections(10);
    if (result != SLNet::RAKNET_STARTED) {
        throw std::runtime_error("Failed to start server");
    }
}

void NetworkServer::update(std::vector<GameObject*>& aGameObjects) {
    mGameObjects = &aGameObjects;
    if (!mServer->IsActive()) {
        throw std::runtime_error("Server is not running");
    }
    handleIncomingPackets();
    sendPackets();
}

bool NetworkServer::isConnected() const { return mServer->IsActive(); }

void NetworkServer::sendPlayerInstantiation(SLNet::RakNetGUID playerID) {
    std::cout << "Sending player instantiation message to all clients.\n";
    SLNet::BitStream bs;
    NetworkSharedFunctions::makeBitStream(bs);
    NetworkPacket networkPacket;
    networkPacket.messageID = (SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT;
    networkPacket.clientGUID = playerID;
    NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);

    sendToAllClients(bs);
}

void NetworkServer::handleIncomingPackets() {
    SLNet::Packet* packet;
    for (packet = mServer->Receive(); packet; mServer->DeallocatePacket(packet), packet = mServer->Receive()) {
        switch (packet->data[0]) {
        case ID_NEW_INCOMING_CONNECTION:
            std::cout << "A connection is incoming.\n";
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            std::cout << "Our connection request has been accepted.\n";
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            std::cout << "The server is full.\n";
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            std::cout << "A client has disconnected.\n";
            onClientDisconnected(packet->guid);
            break;
        case ID_CONNECTION_LOST:
            std::cout << "A client lost the connection.\n";
            break;
        case (SLNet::MessageID)NetworkMessage::ID_TRANSFORM_PACKET:
            handleTransform(packet);
            break;
        case (SLNet::MessageID)NetworkMessage::ID_PLAYER_INIT:
            std::cout << "Received player instantiation message.\n";
            spawnNewPlayer(packet);
            break;
        default:
            std::cout << "Message with identifier " << packet->data[0] << " has arrived.\n";
            break;
        }
    }
}

void NetworkServer::sendTransform() {
    if (!mGameObjects) {
        throw std::runtime_error("Game objects not set");
    }

    for (auto gameObject : *mGameObjects) {
        if (!gameObject->hasComponent<NetworkObject>()) {
            continue;
        }
        if (!gameObject->hasComponent<NetworkTransform>()) {
            continue;
        }

        Transform transform = gameObject->getTransform();
        NetworkTransform* networkTransform = gameObject->getComponents<NetworkTransform>()[0];
        SLNet::BitStream bs;
        NetworkSharedFunctions::makeBitStream(bs);
        NetworkPacket networkPacket;
        NetworkObject* networkObject = gameObject->getComponents<NetworkObject>()[0];
        networkPacket.messageID = NetworkMessage::ID_TRANSFORM_PACKET;
        networkPacket.networkObjectID = networkObject->getNetworkObjectID();
        NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);

        if (networkTransform->getSendPositionX()) {
            bs.Write(transform.position.x);
        }
        if (networkTransform->getSendPositionY()) {
            bs.Write(transform.position.y);
        }
        if (networkTransform->getSendRotation()) {
            bs.Write(transform.rotation);
        }
        if (networkTransform->getSendScaleX()) {
            bs.Write(transform.scale.x);
        }
        if (networkTransform->getSendScaleY()) {
            bs.Write(transform.scale.y);
        }

        sendToAllClients(bs);
    }
}

void NetworkServer::handleTransform(SLNet::Packet* aPacket) {
    if (!mGameObjects) {
        throw std::runtime_error("Game objects not set");
    }
    SLNet::BitStream bs(aPacket->data, aPacket->length, false);
    NetworkSharedFunctions::getBitStreamData(bs);

    for (auto gameObject : *mGameObjects) {
        NetworkObject* networkObject = gameObject->getComponents<NetworkObject>()[0];
        if (networkObject->getClientID() != aPacket->guid) {
            continue;
        }
        if (gameObject->hasComponent<NetworkTransform>()) {
            Transform transform = gameObject->getTransform();
            NetworkTransform* networkTransform = gameObject->getComponents<NetworkTransform>()[0];
            if (networkTransform->getSendPositionX()) {
                bs.Read(transform.position.x);
            }
            if (networkTransform->getSendPositionY()) {
                bs.Read(transform.position.y);
            }
            if (networkTransform->getSendRotation()) {
                bs.Read(transform.rotation);
            }
            if (networkTransform->getSendScaleX()) {
                bs.Read(transform.scale.x);
            }
            if (networkTransform->getSendScaleY()) {
                bs.Read(transform.scale.y);
            }
            gameObject->setTransform(transform);
        }
    }
}

void NetworkServer::handleCustomSerialize(SLNet::Packet* aPacket) {
    SLNet::BitStream bs(aPacket->data, aPacket->length, false);
    NetworkPacket networkPacket = NetworkSharedFunctions::getBitStreamData(bs);

    for (auto gameObject : *mGameObjects) {
        NetworkObject* networkObject = gameObject->getComponents<NetworkObject>()[0];
        if (aPacket->guid != networkObject->getClientID()) { // check client ID
            continue;
        }
        if (networkPacket.networkObjectID != networkObject->getNetworkObjectID()) { // check network object ID
            continue;
        }
        for (auto networkBehaviour : gameObject->getComponents<INetworkBehaviour>()) {
            if (networkBehaviour->getNetworkBehaviourID() != networkPacket.networkBehaviourID) { // check network
                                                                                                 // behaviour ID
                continue;
            }
            if (networkBehaviour->GetNetworkVariables().at(networkPacket.networkVariableID)->getTypeId() !=
                networkPacket.ISerializableID) { // check network variable ID
                continue;
            }
            networkBehaviour->GetNetworkVariables().at(networkPacket.networkVariableID)->deserialize(bs);
        }
    }
}

void NetworkServer::spawnNewPlayer(SLNet::Packet* aPacket) {
    SLNet::RakNetGUID clientID = aPacket->guid;
    // Instantiate player on the server for this client
    std::vector<GameObject*> persistantObjects =
        EngineBravo::getInstance().getSceneManager().getCurrentScene()->getPersistentGameObjects();
    for (auto object : persistantObjects) {
        if (object->hasComponent<NetworkObject>()) {
            NetworkObject* networkObject = object->getComponents<NetworkObject>()[0];
            if (networkObject->isPlayer() && !(networkObject->getClientID() == clientID)) {
                sendPlayerInstantiation(networkObject->getClientID());
            }
        }
    }
    EngineBravo::getInstance().getNetworkManager().instantiatePlayer(clientID); // Server-side
                                                                                // instantiation
    sendPlayerInstantiation(clientID);
}

void NetworkServer::onClientDisconnected(SLNet::RakNetGUID clientID) {
    EngineBravo::getInstance().getNetworkManager().destroyPlayer(clientID);

    SLNet::BitStream bs;
    NetworkSharedFunctions::makeBitStream(bs);
    NetworkPacket networkPacket;
    networkPacket.messageID = NetworkMessage::ID_PLAYER_DESTROY;
    networkPacket.clientGUID = clientID;
    NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);
    sendToAllClients(bs);
}

void NetworkServer::sendToAllClients(SLNet::BitStream& aBitStream) {
    DataStructures::List<SLNet::SystemAddress> addresses;
    DataStructures::List<SLNet::RakNetGUID> guids;
    mServer->GetSystemList(addresses, guids);
    for (int i = 0; i < addresses.Size(); i++) {
        mServer->Send(&aBitStream, PacketPriority::MEDIUM_PRIORITY, PacketReliability::RELIABLE, 0, guids[i], false);
    }
}

void NetworkServer::sendPackets() {
    auto now = std::chrono::steady_clock::now();
    auto frameTime = std::chrono::milliseconds(1000 / mTickRate);
    if (now - mLastSendPacketsTime < frameTime) {
        return;
    }
    sendTransform();
    mLastSendPacketsTime = now;
}

void NetworkServer::sendCustomSerialize() {
    for (GameObject* gameObject : *mGameObjects) {
        if (!gameObject->hasComponent<INetworkBehaviour>()) {
            continue;
        }
        for (INetworkBehaviour* networkBehaviour : gameObject->getComponents<INetworkBehaviour>()) {
            for (int i = 0; i < networkBehaviour->GetNetworkVariables().size(); i++) {
                SLNet::BitStream bs;
                NetworkSharedFunctions::makeBitStream(bs);
                NetworkPacket networkPacket;
                networkPacket.messageID = (SLNet::MessageID)NetworkMessage::ID_CUSTOM_SERIALIZE;
                networkPacket.networkObjectID = gameObject->getComponents<NetworkObject>()[0]->getNetworkObjectID();
                networkPacket.ISerializableID = networkBehaviour->GetNetworkVariables().at(i)->getTypeId();
                networkPacket.SetTimeStampNow();
                networkPacket.clientGUID = gameObject->getComponents<NetworkObject>()[0]->getClientID();

                networkBehaviour->GetNetworkVariables().at(i)->serialize(bs);
                NetworkSharedFunctions::setBitStreamNetworkPacket(bs, networkPacket);
                sendToAllClients(bs);
            }
        }
    }
}
