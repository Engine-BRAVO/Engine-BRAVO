// #include "NetworkManager.h"

// NetworkManager::NetworkManager() : mPeer(SLNet::RakPeerInterface::GetInstance()), mRole(CLIENT) {}

// NetworkManager::~NetworkManager()
// {
//     Shutdown();
// }

// void NetworkManager::StartServer(unsigned short port)
// {
//     SLNet::SocketDescriptor sd(port, 0);
//     mPeer->Startup(10, &sd, 1);
//     mPeer->SetMaximumIncomingConnections(10);
//     mRole = SERVER;
//     std::cout << "Server started on port " << port << std::endl;
// }

// void NetworkManager::StartClient(const char *serverIP, unsigned short serverPort)
// {
//     SLNet::SocketDescriptor sd;
//     mPeer->Startup(1, &sd, 1);
//     mPeer->Connect(serverIP, serverPort, nullptr, 0);
//     mRole = CLIENT;
//     std::cout << "Client started and connecting to server: " << serverIP << std::endl;
// }

// void NetworkManager::StartHost(unsigned short port)
// {
//     StartServer(port);
//     StartClient("127.0.0.1", port);
//     mRole = HOST;
//     std::cout << "Host started on port " << port << std::endl;
// }

// void NetworkManager::Shutdown()
// {
//     mPeer->Shutdown(300);
//     SLNet::RakPeerInterface::DestroyInstance(mPeer);
//     std::cout << "Network shutdown complete." << std::endl;
// }

// void NetworkManager::Update()
// {
//     SLNet::Packet *packet;
//     for (packet = mPeer->Receive(); packet; mPeer->DeallocatePacket(packet), packet = mPeer->Receive())
//     {
//         switch (packet->data[0])
//         {
//         case ID_NEW_INCOMING_CONNECTION:
//             std::cout << "A connection is incoming.\n";
//             break;
//         case ID_CONNECTION_REQUEST_ACCEPTED:
//             std::cout << "Our connection request has been accepted.\n";
//             break;
//         case ID_NO_FREE_INCOMING_CONNECTIONS:
//             std::cout << "The server is full.\n";
//             break;
//         case ID_DISCONNECTION_NOTIFICATION:
//             std::cout << "A client has disconnected.\n";
//             break;
//         case ID_CONNECTION_LOST:
//             std::cout << "A client lost the connection.\n";
//             break;
//         default:
//             std::cout << "Received packet id: " << (int)packet->data[0] << std::endl;
//             break;
//         }
//     }
// }

#include "NetworkManager.h"

NetworkManager::NetworkManager(GameObject &aDefaultPlayerPrefab) : mDefaultPlayerPrefab(aDefaultPlayerPrefab), mIsClient(false), mIsServer(false), mIsHost(false), mTickRate(60), mEnableSceneManagement(false)
{
}

void NetworkManager::startServer()
{
    if (mIsClient || mIsHost)
    {
        throw std::runtime_error("Cannot start server when client or host is already running");
    }
    mIsServer = true;
    mServer = std::make_unique<NetworkServer>();
}

void NetworkManager::startClient()
{
    if (mIsServer || mIsHost)
    {
        throw std::runtime_error("Cannot start client when server or host is already running");
    }
    mIsClient = true;
    mClient = std::make_unique<NetworkClient>();
}

void NetworkManager::startHost()
{
    if (mIsServer || mIsClient)
    {
        throw std::runtime_error("Cannot start host when server or client is already running");
    }
    mIsHost = true;
    mHost = std::make_unique<NetworkHost>();
}

void NetworkManager::shutdown()
{
    throw std::runtime_error("NetworkManager::shutdown() not implemented");
}

void NetworkManager::update()
{
    throw std::runtime_error("NetworkManager::update() not implemented");
}

bool NetworkManager::getIsServer() const
{
    return mIsServer;
}

bool NetworkManager::getIsClient() const
{
    return mIsClient;
}

bool NetworkManager::getIsHost() const
{
    return mIsHost;
}

void NetworkManager::setTickRate(int aTickRate)
{
    mTickRate = aTickRate;
}

int NetworkManager::getTickRate() const
{
    return mTickRate;
}

void NetworkManager::setEnableSceneManagement(bool aEnableSceneManagement)
{
    mEnableSceneManagement = aEnableSceneManagement;
}

bool NetworkManager::getEnableSceneManagement() const
{
    return mEnableSceneManagement;
}