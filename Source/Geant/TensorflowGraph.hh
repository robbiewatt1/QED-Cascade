#ifndef TENSORFLOWGRAPH_HH
#define TENSORFLOWGRAPH_HH

#include <string>
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"


class TensorflowGraph
{
public:
    TensorflowGraph(std::string graphPath, int inputShape,
        const std::vector<std::string>& outputs,
        int threads = 1);

    ~TensorflowGraph();

    void runGraph(const std::vector<double> &input,
        std::vector<tensorflow::Tensor> &finalOutput);

private:

    tensorflow::Session* tfSession;

    int m_inputShape;
    std::vector<std::string> m_outputs;

};
#endif