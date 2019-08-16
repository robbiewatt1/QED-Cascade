#ifndef TENSORFLOWGRAPH_HH
#define TENSORFLOWGRAPH_HH

#include <string>
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"

using namespace tensorflow;

class TensorflowGraph
{
public:
    TensorflowGraph(std::string graphPath, int inputShape,
        const std::vector<std::string>& outputs);

    ~TensorflowGraph();

    void runGraph(const std::vector<double> &input,
        std::vector<tensorflow::Tensor> &finalOutput);

private:
    Session* tfSession;
    Tensor inputTens;

    int m_inputShape;
    std::vector<std::string> m_outputs;
};
#endif