#ifndef NE_NODEGROUP_HPP
#define NE_NODEGROUP_HPP

#include "NE_NodeEngineTypes.hpp"
#include "NE_Serializable.hpp"
#include "NE_NodeCollection.hpp"

#include <memory>
#include <unordered_map>

namespace NE
{

class NodeGroup : public DynamicSerializable
{
	SERIALIZABLE;

public:
	NodeGroup ();
	~NodeGroup ();

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

	static NodeGroupPtr		Clone (const NodeGroupConstPtr& nodeGroup);
};

}

#endif
