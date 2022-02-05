#include <iostream>
#include <fstream>
#include <list>
#include "MinHeap.h"
#include <string>
#include <map>
#include <vector>
#include <limits>

using namespace std;

class Node //for storing the weights and creating the min heap
{
	int id; //vertex number 
	int weight;
	string type;
	int parent; //parent vertex
public:
	Node()
	{
		id = 0;
		weight = numeric_limits<int>::max();
		type = "";
		parent = -1;
	}
	bool operator >(const Node& obj)
	{
		return weight > obj.weight;
	}
	bool operator <(const Node& obj)
	{
		return weight < obj.weight;
	}
	bool operator ==(const Node& obj)
	{
		return id == obj.id;
	}
	friend ostream& operator<<(ostream& out, const Node& obj)
	{
		out << obj.id << "(" << obj.type << ")";
		return out;
	}
	friend class Graph;
};
class Graph
{
	pair < map <int, Node*>, map<int, list <Node*>> > graph;
	//first is set of vertices second is the adjacency list 
	int V; //total number of vertices 
	void printShorestpath(vector <int>order)
	{
		int index;
		int parent;
		cout << "Path starting from: " << *graph.first[order[0]] << endl << endl;
		for (int i = 1; i < order.size(); i++)
		{
			index = order[i];
			parent = graph.first[index]->parent;
			if (parent != -1)
			{
				cout << "From " << *graph.first[parent];
				cout << " to " << *graph.first[index];
				cout << " wire length is " << graph.first[index]->weight;
				cout << endl;
			}
		}
	}
public:
	Graph()
	{
		V = 0;
	}
	Graph(const Graph& g2)
	{
		V = g2.V;
		map<int, Node*>::iterator it;
		Node* obj;
		map<int, Node*>temp = g2.graph.first;
		for (it = temp.begin(); it != temp.end(); ++it)
		{
			Node* n = new Node;
			obj = it->second;
			n->id = obj->id;
			n->type = obj->type;
			n->weight = obj->weight;
			graph.first[it->first] = n;
		}
		list<Node*>::iterator lit;
		map <int, list<Node*>> l = g2.graph.second;
		for (it = graph.first.begin(); it != graph.first.end(); ++it)
		{
			for (lit = l[it->second->id].begin(); lit != l[it->second->id].end(); ++lit)
			{
				obj = *lit;
				Node* n = new Node;
				n->id = obj->id;
				n->type = obj->type;
				n->weight = obj->weight;
				graph.second[it->first].push_back(n);
			}
		}
	}
	void loadGraph(string name)
	{
		ifstream fin;
		fin.open(name);
		if (!fin)
		{
			cout << "Text File " << name << " not found\n";
			return;
		}
		fin >> V;
		for (int i = 1; i <= V; i++)
		{
			Node* o = new Node;
			fin >> o->id;
			fin.ignore();
			fin >> o->type;
			graph.first[o->id] = o;
		}
		int t_edge;
		fin >> t_edge;
		int start;
		int end;
		int weight;
		for (int i = 0; i < t_edge; i++)
		{
			Node* temp = new Node;

			fin >> start;
			fin.ignore();
			fin >> end;
			fin >> weight;

			temp->id = graph.first[end]->id;
			temp->type = graph.first[end]->type;
			temp->weight = weight;
			temp->parent = start;

			graph.second[start].push_back(temp);

			Node* temp1 = new Node;

			temp1->id = graph.first[start]->id;
			temp1->type = graph.first[start]->type;
			temp1->weight = weight;
			temp1->parent = end;

			graph.second[end].push_back(temp1);
		}
		cout << "Data loaded in graph\n";
		fin.close();
	}
	void createPath()
	{
		if (graph.first.empty()||graph.second.empty())
		{
			cout << "Path cannot be created\n";
			return;
		}
		
		MinHeap <Node> heap;
		graph.first[1]->weight = 0;
	
		heap.build_min_heap(graph.first);

		Node min;
		list<Node*>::iterator it;
		Node* edge;
		int pos;
		vector <int>order; //to store the spanning set

		while (!heap.empty())
		{
			min = heap.getMin(); //min contains the node with minimum weight 
			order.push_back(min.id);//storing the spanning set 
			heap.removeMin();
			for (it = graph.second[min.id].begin(); it != graph.second[min.id].end(); ++it)
			{
				edge = *it; //the adjacent vertex of the min 
				pos = heap.search(*edge);//searching the adjacent vertex in heap 
				if (pos != -1)//checking if the vertex is present in Heap or not 
				{
					if (edge->weight < graph.first[edge->id]->weight) //if weight of edge is less
					{
						graph.first[edge->id]->weight = edge->weight;//replace the vertex weight
						graph.first[edge->id]->parent = min.id; //assign the parent from which we have visited this vertex
						heap.decreasekey(*graph.first[edge->id], pos); //now updating the weight in heap aswell 
					}
				}
			}
		}
		printShorestpath(order); //printing the spanning set
	}
	~Graph()
	{
		//deleting all the dynamic Node objects from vertex as well as from list 
		map<int, Node*>::iterator it;
		Node* obj;
		for (it = graph.first.begin(); it != graph.first.end(); ++it)
		{
			obj = it->second;
			delete obj;
			obj = nullptr;
		}
		list<Node*>::iterator it1;
		for (it = graph.first.begin(); it != graph.first.end(); ++it)
		{
			for (it1 = graph.second[it->first].begin(); it1 != graph.second[it->first].end(); ++it1)
			{
				obj = *it1;
				delete obj;
				obj = nullptr;
			}
		}
		graph.first.clear();
		graph.second.clear();
	}
	void storeGraph()
	{
		if (graph.first.empty())
		{
			cout << "Graph is Empty\n";
			return;
		}
		ofstream fout;
		fout.open("graph.txt");
		map <int, Node*>::iterator it;
		Node* ptr;
		Node* lptr;
		fout << "Total vertices are: " << V << endl;
		for (it = graph.first.begin(); it!=graph.first.end(); ++it)
		{
			ptr = it->second;
			fout << ptr->id << " " << ptr->type << endl;
		}
		fout << endl;

		list <Node*>::iterator lt;
		fout << "Edges are:\n";
		fout << "First digit is starting vertex, second digit is ending vertex and third digit is weight.\n";
		for (it = graph.first.begin(); it != graph.first.end(); ++it)
		{
			ptr = it->second;
			for (lt = graph.second[ptr->id].begin(); lt != graph.second[ptr->id].end(); ++lt)
			{
				lptr = *lt;
				fout << ptr->id << " ";
				fout << lptr->id << " " << lptr->weight << " ";
				fout << endl;
			}
		}
		cout << "Graph store in file successfully\n";
		fout.close();
	}
	void print()
	{
		if (graph.first.empty())
		{
			cout << "Graph is Empty\n";
			return;
		}
		map <int, Node*>::iterator it;
		list <Node*>::iterator lt;
		Node* ptr;
		Node* lptr;
		cout << "Total vertices are: " << V << endl;
		for (it = graph.first.begin(); it != graph.first.end(); ++it)
		{
			ptr = it->second;
			cout << ptr->id << ": ";
			for (lt = graph.second[ptr->id].begin(); lt != graph.second[ptr->id].end(); ++lt)
			{
				lptr = *lt;
				cout << lptr->id << "->" << lptr->weight << " ";
			}
			cout << endl;
		}
	}
	bool addVertex(int id,string type)
	{
		if (graph.first.find(id) != graph.first.end())//making sure the vertex don't exist before
			return false;
		Node* temp = new Node;
		temp->id = id;
		temp->type = type;
		graph.first[id] = temp;
		V++;
		return true;
	}
	bool addEdge(int s, int e,int weight)
	{
		//making sure both the starting and ending vertices exist in graph
		if (graph.first.find(s) != graph.first.end() && graph.first.find(e) != graph.first.end())
		{
			Node* temp = new Node;
			temp->id = e;
			temp->weight = weight;
			temp->parent = s;
			graph.second[s].push_back(temp);
			temp = new Node;
			temp->id = s;
			temp->weight = weight;
			temp->parent = e;
			graph.second[e].push_back(temp);
			return true;
		}
		return false;
	}
};
int main()
{
	Graph obj;
	int n;
	do
	{
		cout << "\nPress 1 to Load Graph\nPress 2 to Print Graph\n";
		cout << "Press 3 to store graph in file\nPress 4 to add a Vertex\n";
		cout << "Press 5 to add an edge\n";
		cout << "Press 6 to print the shortest path\n";
		cout << "Press 7 to exit\n\n";
		cin >> n;
		while (n < 1 || n>7)
		{
			cout << "Invalid input\n";
			cin >> n;
		}
		if (n == 1)
		{
			obj.loadGraph("data.txt");
		}
		else if (n == 2)
		{
			obj.print();
		}
		else if (n == 3)
		{
			obj.storeGraph();
		}
		else if (n == 4)
		{
			int id;
			string t;
			ifstream fin;
			fin.open("vertex.txt");
			while(!fin.eof())
			{
				fin >> id;
				fin >> t;
				if (obj.addVertex(id,t))
					cout << "Vertex added\n";
				else
					cout << "Vertex already exists\n";
			}
			fin.close();
		}
		else if (n == 5)
		{
			ifstream fin;
			fin.open("edges.txt");
			int id1, id2;
			int weight;
			while (!fin.eof())
			{
				fin >> id1;
				fin >> id2;
				fin >> weight;
				if (obj.addEdge(id1, id2, weight))
					cout << "Edge added\n";
				else
					cout << "Edge not added\n";
			}
			fin.close();
		}
		else if (n == 6)
		{
			obj.createPath();
		}
	} while (n != 7);
	
	system("pause");
	return 0;
}