#ifndef NETWORKVARIABLE_H
#define NETWORKVARIABLE_H

#include "Network/NetworkInformation.h"

class INetworkBehaviour;
class NetworkVariableBase {
public:
    virtual ~NetworkVariableBase() = default;

    virtual bool IsDirty() const = 0;
    virtual void MarkClean() = 0;
};

template <typename INetworkSerializableTemplate> class NetworkVariable : public NetworkVariableBase {
public:
    // NetworkVariable(WritePermission aWritePermission);
    NetworkVariable(INetworkBehaviour* aOwner, INetworkSerializableTemplate aValue = INetworkSerializableTemplate())
        : mValue(std::move(aValue)), mDirty(false) {
        if (aOwner) {
            aOwner->RegisterNetworkVariable(this);
        }
    }

    INetworkSerializableTemplate getValue() const { return mValue; }

    void setValue(INetworkSerializableTemplate aValue) {
        if (mValue != aValue) {
            mValue = aValue;
            mDirty = true;
        }
    }

    bool isDirty() const { return mDirty; }

    void setClean() { mDirty = false; }

    // void serialize(SLNet::BitStream& stream) const;
    // void deserialize(SLNet::BitStream& stream);

private:
    INetworkSerializableTemplate mValue;
    bool mDirty;
};

#endif // NETWORKVARIABLE_H