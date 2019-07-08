#include <stdexcept>

#include "TensorflowGraph.hh"
#include <tensorflow/core/public/session.h>
#include <tensorflow/core/protobuf/meta_graph.pb.h>

using namespace tensorflow;

TensorflowGraph::TensorflowGraph(std::string graphDir)
{

	const std::string graphPath = graphDir + "/model.meta";
	const std::string weightPath = graphDir + "/weight";


	tfSession = NewSession(SessionOptions());
	if (tfSession == nullptr)
	{
    	throw std::runtime_error("Could not create Tensorflow session.");
    }

    Status status;
    MetaGraphDef graph_def;
    status = ReadBinaryProto(Env::Default(), graphPath, &graph_def);
	if (!status.ok())
	{
    	throw std::runtime_error("Error reading graph definition from " + graphPath + ": "
    		+ status.ToString());
	}

	status = tfSession->Create(graph_def.graph_def());
	if (!status.ok())
	{
    	throw std::runtime_error("Error creating graph: " + status.ToString());
	}

	Tensor checkpointPathTensor(DT_STRING, TensorShape());
	checkpointPathTensor.scalar<std::string>()() = weightPath;
	status = tfSession->Run(
        {{ graph_def.saver_def().filename_tensor_name(), checkpointPathTensor },},
        {},
        {graph_def.saver_def().restore_op_name()},
        nullptr);
	if (!status.ok())
	{
    	throw std::runtime_error("Error loading checkpoint from " + weightPath + ": "
    		+ status.ToString());
	}
}

TensorflowGraph::~TensorflowGraph()
{
	delete tfSession;
}