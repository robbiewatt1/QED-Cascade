#ifndef TENSORFLOWGRAPH_HH
#define TENSORFLOWGRAPH_HH

#include <string>
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"

using namespace tensorflow;

class TensorflowGraph
{
public:
	TensorflowGraph(std::string graphDir);

	~TensorflowGraph();

private:
	Session* tfSession;
};
#endif