/**
 * @file INetworkBehaviour.h
 * @brief Defines the INetworkBehaviour class for network-related behaviour scripts.
 */

#ifndef NETWORKBEHAVIOUR_H
#define NETWORKBEHAVIOUR_H

#include "Components/IBehaviourScript.h"

#include <string>
#include <vector>

class NetworkVariableBase;

/**
 * @class INetworkBehaviour
 * @brief Base class for network behaviour scripts.
 */
class INetworkBehaviour : public IBehaviourScript
{
public:
	INetworkBehaviour(std::string aTag = "defaultNetworkBehaviour");
	virtual ~INetworkBehaviour() = default;

	virtual void serverRpc();
	virtual void clientRpc();
	virtual void onNetworkSpawn();

	void RegisterNetworkVariable(NetworkVariableBase* variable);
	std::vector<std::reference_wrapper<NetworkVariableBase>> GetNetworkVariables();

	bool isOwner();

	void destroy();

	uint8_t getNetworkBehaviourID() const;

private:
	bool mIsOwner;						  ///< Indicates if the current object is the owner.
	bool mIsOwnerSet;					  ///< Indicates if the owner status has been set.
	uint8_t mNetworkBehaviourID;		  ///< The network behaviour ID.
	static int networkBehaviourIDCounter; ///< Counter for generating unique network behaviour IDs.

protected:
	std::vector<std::reference_wrapper<NetworkVariableBase>> mNetworkVariables; ///< List of network variables.
};

#endif // NETWORKBEHAVIOUR_H