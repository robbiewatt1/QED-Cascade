#include <stdexcept>

#include "TensorflowGraph.hh"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"

using namespace tensorflow;

TensorflowGraph::TensorflowGraph(std::string graphPath)
{
    // Set up the tendorfloe seesion
    tfSession = NewSession(SessionOptions());
    if (tfSession == nullptr)
    {
        throw std::runtime_error("Could not create Tensorflow session.");
    }

    // Read the graph
    Status status;
    GraphDef graph_def;
    status = ReadBinaryProto(Env::Default(), graphPath, &graph_def);
    if (!status.ok())
    {
        throw std::runtime_error("Error reading graph definition from " + graphPath + ": "
            + status.ToString());
    }

    // Add the graph to the session
    status = tfSession->Create(graph_def);
    if (!status.ok())
    {
        throw std::runtime_error("Error adding graph! " + status.ToString());
    }
}

TensorflowGraph::~TensorflowGraph()
{
    delete tfSession;
}

void TensorflowGraph::runGraph()
{
    Tensor a(DT_FLOAT, TensorShape());
    a.scalar<float>()() = 3.0;

    Tensor b(DT_FLOAT, TensorShape());
    b.scalar<float>()() = 2.0;

    std::vector<std::pair<string, tensorflow::Tensor>> inputs = 
    {
        { "a", a },
        { "b", b },
    };
    std::vector<tensorflow::Tensor> outputs;

    Status status;
    status = tfSession->Run({inputs}, {"c"}, {}, &outputs);
/*
    if (!status.ok()) 
    {
        throw std::runtime_error("Error running graph! " + status.ToString());

    }
    auto output_c = outputs[0].scalar<float>();
    std::cout << typeid(output_c).name() << std::endl;
    std::cout << outputs[0].DebugString() << "\n"; // Tensor<type: float shape: [] values: 30>
    std::cout << typeid(output_c()).name() << "\n"; // 30
*/
}
