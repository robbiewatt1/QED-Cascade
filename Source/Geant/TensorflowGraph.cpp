#include <stdexcept>

#include "TensorflowGraph.hh"

//using namespace tensorflow;

TensorflowGraph::TensorflowGraph(std::string graphPath, int inputShape,
    const std::vector<std::string>& outputs, int threads):
m_inputShape(inputShape), m_outputs(outputs)
{
    // Set up the tendorflow seesion
    tensorflow::SessionOptions options;
    tensorflow::ConfigProto & config = options.config;
    config.set_inter_op_parallelism_threads(threads);
    config.set_intra_op_parallelism_threads(threads);
    config.set_use_per_session_threads(false);  
    tfSession = tensorflow::NewSession(options);

    if (tfSession == nullptr)
    {
        throw std::runtime_error("Could not create Tensorflow session.");
    }

    // Read the graph
    tensorflow::Status status;
    tensorflow::GraphDef graph_def;
    status = tensorflow::ReadBinaryProto(tensorflow::Env::Default(), graphPath, &graph_def);
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
}

void TensorflowGraph::runGraph(const std::vector<double> &input,
    std::vector<tensorflow::Tensor> &finalOutput)
{

    tensorflow::Tensor inputTens = tensorflow::Tensor(tensorflow::DT_DOUBLE,
            tensorflow::TensorShape({1, m_inputShape}));
    auto input_tensor_mapped = inputTens.tensor<double, 2>();
    for(int i = 0; i < m_inputShape; i++)
    {
        input_tensor_mapped(0, i) = input[i];
    }
    std::vector<std::pair<std::string, tensorflow::Tensor>> feedDict = 
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
