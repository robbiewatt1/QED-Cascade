#ifndef TENSORFLOWGRAPH_HH
#define TENSORFLOWGRAPH_HH

#include <string>
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"

using namespace tensorflow;

class TensorflowGraph
{
public:
    TensorflowGraph(std::string graphPath, int inputShape);

    ~TensorflowGraph();

    void runGraph(std::vector<double> input, std::vector<tensorflow::Tensor> finalOutput);

private:
    Session* tfSession;
    Tensor inputTens;

    int m_inputShape;
};
#endif