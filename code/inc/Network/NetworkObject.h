#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

#include "Components/Component.h"

#include <slikenet/types.h>

#include <vector>

class NetworkManager;
class INetworkBehaviour;

class NetworkObject : public Component {
public:
    NetworkObject(std::string aTag = "defaultNetworkObject");
    ~NetworkObject() = default;

    NetworkObject(const NetworkObject& other);
    NetworkObject& operator=(const NetworkObject& other);

    NetworkObject(NetworkObject&& other) noexcept;
    NetworkObject& operator=(NetworkObject&& other) noexcept;

    std::unique_ptr<Component> clone() const override;

	void setClientGUID(SLNet::RakNetGUID aClientID);
	SLNet::RakNetGUID getClientGUID() const;

	void setOwner(bool aIsOwner);
	bool isOwner() const;

    bool isPlayer() const;

	int getNetworkObjectID() const;

private:
    bool mIsOwner;
    int mNetworkObjectID;
	SLNet::RakNetGUID mClientGUID;

	friend class NetworkManager;
	bool mIsPlayer;

	static int networkObjectIDCounter;

private:
    void setPlayer(bool aIsPlayer);
	void setNetworkObjectID(int aNetworkObjectID);
};

#endif // NETWORKOBJECT_H