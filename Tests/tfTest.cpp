#include "TensorflowGraph.hh"
#include <iostream>

int main(int argc, char* argv[])
{
	TensorflowGraph graph = TensorflowGraph("/home/robbie/Documents/QED-Cascade/Tests/python/SaveFiles/frozen_graph.pb");

	for(long unsigned int i=0; i<1e5; i++)
	{
		graph.runGraph();
	}
	return 0;

}
