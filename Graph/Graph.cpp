#include "Graph.hpp"

// PUBLIC METHODS

Graph::Graph(const string path, bool selfLoop) : GraphADT(path), selfLoop(selfLoop)
{
	TIMERSTART(build_matrix);
	if(path.find(".txt" )!= std::string::npos) {
		buildTxt();
		format = "txt";
	}
	else if(path.find(".bin")!= std::string::npos) {
		buildBin();
		format = "bin";
	}
	TIMERSTOP(build_matrix);
}

Graph::Graph(const string path) : GraphADT(path)
{
	TIMERSTART(build_matrix);
	if(path.find(".txt" )!= std::string::npos) {
		buildTxt();
		format = "txt";
	}
	else if(path.find(".bin")!= std::string::npos) {
		buildBin();
		format = "bin";
	}
	TIMERSTOP(build_matrix);
}

Graph::Graph() {}

Graph::~Graph()
{
	for (auto i : matrix)
		delete i;
}

string Graph::getPath()
{
	return path;
}

// funcion que agrega los nodos intermedios que no contienen ninguna arista
// para facilitar la busqueda de los nodos source
void Graph::standardize(vector<uInt>* aux)
{
	cout << "Standarize" << endl;
	vector<Node*> new_matrix;
	auto it = matrix.rbegin();
	uint64_t last_node = back()->getId();
	for (uint64_t cont = last_node; cont > 0; it++, cont--) {
		if (it != matrix.rend() && cont == (*it)->getId()) {
			//continue;
			new_matrix.push_back(*it);
			matrix.pop_back();
		} else {
			Node *tempNode = new Node(cont);
			new_matrix.push_back(tempNode);
			aux->push_back(cont-1);
			it--;
		}
	}

	for (size_t cont = last_node; cont > 0; cont--) {
		matrix.push_back(new_matrix.back());
		new_matrix.pop_back();
	}


}

void Graph::buildTxt()
{
	ifstream file;
	file.open(path);
	if (!file.is_open()) {
		cout << "No se puede leer fichero" << endl;
		cout << path << endl;
		exit(0);
	}
	string line;
	uInt id;
	getline(file, line); // num nodes
	num_nodes = atoi(line.c_str());
	while (!file.eof()) {
		getline(file, line);
		auto adjacents = splitString(line, " ");

		if (adjacents.empty()) {
			continue;
		}
		
		id = atoi(adjacents.at(0).c_str());

		Node *tempNode = new Node(id);
		if (selfLoop) {
			tempNode->setSelfLoop(true);
		}
		for (size_t i = 1; i < adjacents.size();i++) {
			uint64_t adjId = atoi(adjacents[i].c_str());
			tempNode->addAdjacent(adjId);
		}
		tempNode->shrinkToFit();
		tempNode->sort();
		insert(tempNode);
		num_edges += tempNode->edgesSize();
	}
	file.close();
	matrix.shrink_to_fit();

	cout << "nodes: " << num_nodes << ", edges: " << num_edges << endl;
}

void Graph::buildBin()
{
	ifstream binFile; 
    binFile.open(path, ios::in | ios::binary);    
	assert(binFile.is_open());

	Int* nodes = new Int(0); 
	binFile.read((char*)nodes, sizeof(Int)); //num_nodes
	//std::cout << "cantidad de nodos: " << *nodes << endl;
	num_nodes = *nodes;
    Int* buffer = new Int(0); 
	Node* tempNode = nullptr; 
    while(binFile.read((char*)buffer, sizeof(Int))) {
		//cout << *buffer << endl;
        if((*buffer) < 0) {
			uint64_t id = (*buffer) * -1;
			if (tempNode != nullptr) {
				insert(tempNode);
				tempNode->sort();
				tempNode->shrinkToFit();
			}
			tempNode = new Node(id);
			if (selfLoop) {
				tempNode->setSelfLoop(true);
			}
        } else {
			if (tempNode == nullptr) {
				continue; //num de aristas
			}
			tempNode->addAdjacent(*buffer);
        }
    }

	if (tempNode != nullptr) {
		insert(tempNode);
		tempNode->sort();
		tempNode->shrinkToFit();
	}
	delete buffer;
	delete nodes;
    binFile.close();
	
}


uint64_t Graph::size()
{
	return matrix.size();
}

Node* Graph::back(){
	return matrix.back();
}

uint64_t Graph::all_edges_size()
{
	uint64_t size = 0;
	for (auto it : matrix) {
		size += it->edgesSize();
	}
	return size;
}

void Graph::insert(Node *node)
{
	matrix.push_back(node);
	//cout << node->getId() << " " <<node->getBackAdjacent() << endl; 
	if(node->getBackAdjacent() > maxEdge) {
		maxEdge = node->getBackAdjacent();
	}
}

uint64_t Graph::maxValueEdge()
{
	return maxEdge;
}

void Graph::print()
{
	for (size_t i = 0; i < matrix.size(); i++) {
		matrix[i]->print();
	}
}

void Graph::printAsMatrix()
{
	for (size_t i = 0; i < matrix.size(); i++) {
		if(i < matrix[i]->getId() - 1) {
			uInt temp = i; 
			while(temp < matrix[i]->getId()) {
				cout << "Node " << temp << ": " << 0 << endl;
				temp++; 
			}
		}
		matrix[i]->printBinary();
	}
}

void Graph::writeAdjacencyList()
{
	if (size() == 0){
		cout << "Matrix Empty" << endl;
		return;
	}
	ofstream file;
	//int count = 0;
	uint64_t matrix_size = size();
	string pathFile = modify_path(path, 4 ,".txt");
	if (compressed) {
		pathFile = modify_path(path, 4 ,"_compressed.txt");
	} 
	cout << "Writing: " << pathFile<< endl;
	file.open(pathFile, ofstream::out | ofstream::trunc); // limpia el contenido del fichero

	file << num_nodes << endl;

	for (uint64_t i = 0; i < matrix_size ; i++) {
		if(matrix[i] == nullptr) {
			continue; 
		}

		file << matrix[i]->getId() << ":";

		for (auto adj = matrix[i]->adjacentsBegin(); adj != matrix[i]->adjacentsEnd(); adj++) {
			//if (not matrix[i]->hasSelfLoop() and *adj == matrix[i]->getId()) { //si no tiene un selfloop natural 
			//	continue;
			//}
			file << " " << *adj;
		}
		file << endl;
	}
	file.close();
}

void Graph::writeBinaryFile()
{
	ofstream file;
	string pathFile = modify_path(path, 4 ,".bin");
	if (compressed) {
		pathFile = modify_path(path, 4 ,"_compressed.bin");
	} 
	cout << "Writing: " << pathFile << endl;
	file.open(pathFile, ios::out | ios::binary |ios::trunc); 
	assert(file.is_open());
	
	Int size = matrix.size();
	file.write((char*)&size, sizeof(Int));
	Int count_nodes = 0;
	Int count_edges = 0;
	for(auto i : matrix){
		if (i->edgesSize() == 0) {
			continue;
		}
		count_nodes++;
		Int id = i->getId() * -1;
		file.write((char*)&(id), sizeof(Int));
		count_edges += i->edgesSize();
		for(auto j = i->adjacentsBegin(); j != i->adjacentsEnd(); j++){
			//file << i->getId() << " " << j->first << " " << j->second << endl;
			file.write((char*)&(*j), sizeof(Int));
		}
	}

	file.seekp(0);
	file.write((char*)&size, sizeof(Int));
	//file.write((char*)&count_edges, sizeof(Int));
	file.close();
}

void Graph::restoreNodes()
{
	for (size_t i = 0; i < matrix.size(); i++) {
		matrix[i]->restore();
	}
}

AdjMatrixIterator Graph::begin()
{
	return matrix.begin();
}

AdjMatrixIterator Graph::end()
{
	return matrix.end();
}

Node* Graph::at(uInt pos){
	return matrix.at(pos);
}

Node* Graph::find(uInt node_id){
	if( size() == num_nodes ) {
		return at(node_id - 1);
	}
	return binarySearch(0, size()-1,node_id);
}

Node* Graph::binarySearch(uInt l, uInt r, uInt node_id){

    if (r >= l) {
        uInt mid = l + (r - l) / 2;
 
        if (matrix[mid]->getId() == node_id) return matrix[mid];

        if (matrix[mid]->getId() > node_id) {
			if(mid == 0) return nullptr;
            return binarySearch(l, mid - 1, node_id);
		}
        return binarySearch(mid + 1, r, node_id);
    }
    return nullptr;
}