#include "TensorflowGraph.hh"
#include <iostream>

int main(int argc, char* argv[])
{
	TensorflowGraph graph = TensorflowGraph("/home/robbie/Documents/MachineLearning/models/graph.pb");
	
	for(long unsigned int i=0; i<100000; i++)
	{
		graph.runGraph();
	}
	return 0;

}
