#include <stdexcept>

#include "TensorflowGraph.hh"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"

using namespace tensorflow;

TensorflowGraph::TensorflowGraph(std::string graphPath, int inputShape,
    const std::vector<std::string>& outputs):
m_inputShape(inputShape), m_outputs(outputs)
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

    // Create input and output tensors
    inputTens = Tensor(DT_DOUBLE, tensorflow::TensorShape({1, m_inputShape}));
}

TensorflowGraph::~TensorflowGraph()
{
    delete tfSession;
}

void TensorflowGraph::runGraph(const std::vector<double> &input,
    std::vector<tensorflow::Tensor> &finalOutput)
{
    auto input_tensor_mapped = inputTens.tensor<double, 2>();
    for(int i = 0; i < m_inputShape; i++)
    {
        input_tensor_mapped(0, i) = input[i];
    }
    std::vector<std::pair<string, tensorflow::Tensor>> feedDict = 
    {
        { "In", inputTens },
    };
    tensorflow::Status status = tfSession->Run(feedDict,
            m_outputs, {}, &finalOutput);
    if (!status.ok()) 
    {
        throw std::runtime_error("Error running graph! " + status.ToString());
    }
}
