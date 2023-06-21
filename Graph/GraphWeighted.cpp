#include "GraphWeighted.hpp"

// PUBLIC METHODS

GraphWeighted::GraphWeighted(const string path, bool selfLoops) : GraphADT(path, selfLoops)
{
	TIMERSTART(build_matrix);
	format = ".txt";
	buildTxt();
	TIMERSTOP(build_matrix);
	//print();
}

GraphWeighted::GraphWeighted(const string path) : GraphADT(path, false) {
	TIMERSTART(build_matrix);
	format = ".txt";
	buildTxt();
	TIMERSTOP(build_matrix);
}

GraphWeighted::GraphWeighted() {}

GraphWeighted::~GraphWeighted()
{
	for (auto i : matrix)
		delete i;
}

void GraphWeighted::addBicliques(string pathBiclique){

}
// g++ checker.cpp AdjacencyMatrix.cpp Node.cpp Utils.cpp -o checker
/*void GraphWeighted::addBicliques(string path)
{
	cout << "Read File Bicliques" << endl;
	ifstream file;
	file.open(path);
	if (!file.is_open())
	{
		cout << "No se puede leer fichero" << endl;
		cout << path << endl;
		exit(0);
	}
	vector<uInt> newNodesPos;
	if( size() != back()->getId())
		standardize(&newNodesPos);

	string S;
	string C;

	cout << "Add Bicliques" << endl;
	while (!file.eof())
	{
		getline(file, S);
		if (S == "")
			break;
		getline(file, C);

		std::vector<std::string> sources = splitString(S, " ");
		std::vector<std::string> centers = splitString(C, " ");
		sources.erase(sources.begin());
		centers.erase(centers.begin());

		for (auto node : sources)
		{
			uint64_t id = atoi(node.c_str());
			for (auto adjacent : centers)
			{
				uint64_t adjId = atoi(adjacent.c_str());
				matrix[id - 1]->addAdjacent(adjId);
			}
		}
	}
	cout << "Sorting" << endl;
	for (auto it = matrix.begin(); it != matrix.end(); it++)
		(*it)->sort();

	cout << "Delete aux Nodes" << endl;
	for (size_t i = 0; i < newNodesPos.size(); i++){	
		delete matrix[newNodesPos[i]];
		matrix[newNodesPos[i]] = nullptr;
	}
	newNodesPos.clear();
}
*/
// funcion que agrega los nodos intermedios que no contienen ninguna arista
// para facilitar la busqueda de los nodos source
void GraphWeighted::standardize(vector<uInt>* aux)
{
	cout << "Standarize" << endl;
	vector<NodeWeighted*> new_matrix;
	auto it = matrix.rbegin();
	uint64_t last_node = back()->getId();
	for (uint64_t cont = last_node; cont > 0; it++, cont--)
	{
		if (it != matrix.rend() && cont == (*it)->getId())
		{
			//continue;
			new_matrix.push_back(*it);
			matrix.pop_back();
		}
		else
		{
			auto *tempNode = new NodeWeighted(cont);
			new_matrix.push_back(tempNode);
			aux->push_back(cont-1);
			it--;
		}
	}

	for (size_t cont = last_node; cont > 0; cont--){
		matrix.push_back(new_matrix.back());
		new_matrix.pop_back();
	}


}

void GraphWeighted::buildTxt()
{
	ifstream file; 
	file.open(path);
	assert(file.is_open());
	string line;
	NodeWeighted* temp = nullptr;
	while(getline(file,line)){
		if(line.front() == '%') continue; 
		auto content = splitString(line, " ");
		uInt id = atoi(content[0].c_str());
		uInt adj = atoi(content[1].c_str());
		uInt weight = atoi(content[2].c_str());
		if(temp == nullptr or temp->getId() != id){
			if(temp != nullptr) {
				temp->shrinkToFit();
				//temp->sort();
			}
			temp = new NodeWeighted(id);
			matrix.push_back(temp);
		}
		temp->addAdjacent(adj, weight); 
	}
	temp->shrinkToFit();
	//temp->sort();
	
}

void GraphWeighted::buildBin(){

}

string GraphWeighted::getPath(){
	return path;
}

uint64_t GraphWeighted::size()
{
	return matrix.size();
}

NodeWeighted* GraphWeighted::back(){
	return matrix.back();
}

uint64_t GraphWeighted::all_edges_size()
{
	uint64_t size = 0;
	for (auto it : matrix)
	{
		size += it->edgesSize();
	}
	return size;
}

void GraphWeighted::insert(NodeWeighted *node)
{
	matrix.push_back(node);
}

void GraphWeighted::print()
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		matrix[i]->print();
	}
}

void GraphWeighted::writeAdjacencyList(string path_write_)
{
	
}

void GraphWeighted::restoreNodes()
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		matrix[i]->restore();
	}
}

GraphWeightedIterator GraphWeighted::begin()
{
	return matrix.begin();
}

GraphWeightedIterator GraphWeighted::end()
{
	return matrix.end();
}

NodeWeighted* GraphWeighted::at(uInt pos){
	return matrix.at(pos);
}

NodeWeighted* GraphWeighted::find(uInt node_id){
	if( size() == num_nodes )
		return at(node_id-1);
	return binarySearch(0, size()-1,node_id);
}

NodeWeighted* GraphWeighted::binarySearch(uInt l, uInt r, uInt node_id){

    if (r >= l) {
        uInt mid = l + (r - l) / 2;
 
        if (matrix[mid]->getId() == node_id)
            return matrix[mid];

        if (matrix[mid]->getId() > node_id){
			if(mid == 0)
				return nullptr;
            return binarySearch(l, mid - 1, node_id);
		}
 
        return binarySearch(mid + 1, r, node_id);
    }
    return nullptr;
}