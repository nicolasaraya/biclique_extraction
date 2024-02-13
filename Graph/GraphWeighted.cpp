#include "GraphWeighted.hpp"

// PUBLIC METHODS

using namespace std;

GraphWeighted::GraphWeighted(const string path) : GraphADT(path)
{
	matrix = new vector<Node*>();
	TIMERSTART(build_matrix);
	if(path.find(".txt" ) != std::string::npos) {
		buildTxt();
		format = "txt";
	} else if(path.find(".bin") != std::string::npos) {
		buildBin();
		format = "bin";
	}
	TIMERSTOP(build_matrix);
	//print();
}


GraphWeighted::GraphWeighted() 
{
	matrix = new vector<Node*>();
}

GraphWeighted::~GraphWeighted()
{
	for (auto it = matrix->begin(); it != matrix->end(); it++) {
		delete (*it);
	}
	matrix->clear();
	delete matrix;
}

void GraphWeighted::buildBin_alt()
{
	ifstream file;
	file.open(path, ios::in | ios::binary);
	file.seekg (0, file.beg);
	assert(file.is_open());

	uInt* buffer = new uInt(0);
	Node* temp = nullptr;
	while (not file.eof()) {
		file.read((char*)buffer, sizeof(uInt));
		uInt id = *buffer;
		file.read((char*)buffer, sizeof(uInt));
		uInt adj = *buffer;
		file.read((char*)buffer, sizeof(uInt));
		uInt w = *buffer;
		if (temp == nullptr or temp->getId() != id) {
			if (temp != nullptr) {
				insert(temp);
				temp->shrinkToFit();
				temp->sort();
			}
			temp = nullptr;
			temp = find(id);
			if(temp == nullptr) temp = new Node(id, true);
		} 
		temp->addAdjacent(adj, w);
		//if (matrix->size() > 400000 ) break;
	}
	if(temp->edgesSize() > 0) {
		//insert(temp);
		temp->shrinkToFit();
		temp->sort();
	} else delete temp;
	matrix->shrink_to_fit();
	sort();
	delete buffer;
	file.close();
}


void GraphWeighted::buildBin()
{
	ifstream file;
	file.open(path, ios::in | ios::binary);
	file.seekg (0, file.beg);
	assert(file.is_open());

	Int* buffer = new Int(-1);
	Node* temp = nullptr;

	while (not file.eof()) {
		file.read((char*)buffer, sizeof(Int));
		if(*buffer < 0) {
			if (temp != nullptr) {
				temp->shrinkToFit();
				temp->sort();
			}
			temp = nullptr;
			temp = find(-(*buffer));
			if (temp == nullptr) {
				temp = new Node(-(*buffer), true);
				insert(temp);
			}
		} else {
			if(file.eof()) break;
			uInt adj = *buffer;
			file.read((char*)buffer, sizeof(Int));
			uInt weight = *buffer;
			temp->addAdjacent(adj, weight);
		}
		//temp->print();
		//if (matrix->size() > 400000 ) break;
	}
	if(temp->edgesSize() > 0) {
		temp->shrinkToFit();
		temp->sort();
		//insert(temp);
	} else delete temp;
	matrix->shrink_to_fit();
	sort();
	delete buffer;
	file.close();
}


void GraphWeighted::buildTxt()
{
	cout << "format: " << "id ady weight" << endl;
	ifstream file; 
	file.open(path);
	assert(file.is_open());
	string line;
	Node* temp = nullptr;

	//int x = 0;
	while(getline(file,line)){
		if(line.front() == '%') continue; 
		auto content = splitString(line, " ");
		if (content.size() <= 1) continue;
		uInt id = atoi(content[0].c_str());
		uInt adj = atoi(content[1].c_str());
		uInt weight = atoi(content[2].c_str());
		if(temp == nullptr or temp->getId() != id){
			if(temp != nullptr) {
				insert(temp);
				temp->shrinkToFit();
				temp->sort();
			}
			temp = nullptr;
			temp = find(id);
			if(temp == nullptr) temp = new Node(id, true);
		}
		num_edges++;
		temp->addAdjacent(adj, weight); 
		//cout << id << " " << adj << " " << weight << endl;
	}
	if(temp->edgesSize() > 0) {
		insert(temp);
		temp->shrinkToFit();
		temp->sort();
	} else delete temp;

	matrix->shrink_to_fit();
	sort();
	file.close();
	num_nodes = matrix->size();
	cout << "nodes: " << num_nodes << ", edges: " << num_edges << endl;
	//print();
}

void GraphWeighted::transpose()
{
	vector<Node*> t_matrix; 

	for(auto i : *matrix) {
		
		if (i->edgesSize() == 0){
			delete i;
			continue;
		} 

		while (t_matrix.size() <= (i->getBackWeighted())->first) {
			t_matrix.push_back(new Node(t_matrix.size(), true));
		}

		for (auto it = i->wAdjacentsBegin(); it != i->wAdjacentsEnd(); it++) {
			t_matrix.at((*it).first)->addAdjacent(i->getId(), (*it).second);
		}
		delete i;
		
	}
	matrix->clear();

	for (auto i : t_matrix) {
		if (i->edgesSize() > 0) {
			matrix->push_back(i);
		} else {
			delete i;
		}
	}
	t_matrix.clear();

	sort();
	
	transposed = not transposed; 
}

bool GraphWeighted::isTransposed()
{
	return transposed;
}

string GraphWeighted::getPath(){
	return path;
}

void GraphWeighted::setPath(string path)
{
    this->path = path;
}

uint64_t GraphWeighted::maxValueEdge()
{
    return maxEdge;
}

uint64_t GraphWeighted::size()
{
	return matrix->size();
}

Node* GraphWeighted::back(){
	return matrix->back();
}

uint64_t GraphWeighted::all_edges_size()
{
	uint64_t size = 0;
	for (auto it : *matrix)
	{
		size += it->edgesSize();
	}
	return size;
}

void GraphWeighted::insert(Node* node)
{
	matrix->push_back(node);
	if (node->getBackAdjacent() > maxEdge) {
		maxEdge = node->getBackAdjacent();
	}
}

void GraphWeighted::print()
{
	for (size_t i = 0; i < matrix->size(); i++) {
		if(matrix->at(i)->edgesSize() > 0) matrix->at(i)->print();
	}
}


void GraphWeighted::printAsMatrix()
{
	uInt temp = 0;
	for (size_t i = 0; i < matrix->size(); i++) {
		while(temp < matrix->at(i)->getId() ) {
			cout << "Node " << temp << ": " << 0 << endl;
			temp++; 
		}
		temp++;
		
		matrix->at(i)->printBinary();
	}
}

void GraphWeighted::writeAdjacencyList()
{
	ofstream file;
	string pathFile = modify_path(path, 4 ,".txt");
	if (compressed) {
		pathFile = modify_path(path, 4 ,"_compressed.txt");
	} 	
	cout << "Save: " << pathFile << endl;
	file.open(pathFile, std::ofstream::out | std::ofstream::trunc); 
	assert(file.is_open());
	for(auto i : *matrix){
		for(auto j = i->wAdjacentsBegin(); j != i->wAdjacentsEnd(); j++){
			file << i->getId() << " " << (*j).first << " " << (*j).second << endl;
		}
	}
	file.close();
}

void GraphWeighted::writeBinaryFile_alt()
{
	ofstream file;
	string pathFile = modify_path(path, 4 ,".bin");
	if (compressed) {
		pathFile = modify_path(path, 4 ,"_compressed.bin");
	}
	cout << "Save: " << pathFile << endl;
	file.open(pathFile, ios::out | ios::binary |ios::trunc); 
	assert(file.is_open());

	for(auto i : *matrix){
		uInt id = i->getId();
		for(auto j = i->wAdjacentsBegin(); j != i->wAdjacentsEnd(); j++){
			//file << i->getId() << " " << j->first << " " << j->second << endl;
			file.write((char*)&(id), sizeof(uInt));
			file.write((char*)&((*j).first), sizeof(uInt));
			file.write((char*)&((*j).second), sizeof(uInt));
		}
	}
	file.close();
}

void GraphWeighted::writeBinaryFile()
{
	ofstream file;
	string pathFile = modify_path(path, 4 ,".bin");
	if (compressed) {
		pathFile = modify_path(path, 4 ,"_compressed.bin");
	}
	cout << "Save: " << pathFile << endl;
	file.open(pathFile, ios::out | ios::binary |ios::trunc); 
	assert(file.is_open());

	for(auto i : *matrix){
		if (i->edgesSize() == 0) continue; 
		Int id = -(i->getId());
		file.write((char*)&(id), sizeof(Int));
		for(auto j = i->wAdjacentsBegin(); j != i->wAdjacentsEnd(); j++){
			//file << i->getId() << " " << j->first << " " << j->second << endl;
			//file.write((char*)&(id), sizeof(uInt));
			file.write((char*)&((*j).first), sizeof(uInt));
			file.write((char*)&((*j).second), sizeof(uInt));
		}
	}
	file.close();
}

void GraphWeighted::restoreNodes()
{
	for (size_t i = 0; i < matrix->size(); i++) {
		matrix->at(i)->restore();
	}
}

double GraphWeighted::getAverageDegree()
{
	double res = 0;
	uint64_t count = 0;

	for (auto i : *matrix) {
		if (i->edgesSize() > 0) {
			res += i->edgesSize();
			count++;
		}
	}

	res = res / double(count); 

    return res;
}

GraphWeightedIterator GraphWeighted::begin()
{
	return matrix->begin();
}

GraphWeightedIterator GraphWeighted::end()
{
	return matrix->end();
}

Node* GraphWeighted::at(uInt pos){
	return matrix->at(pos);
}

Node* GraphWeighted::find(uInt node_id){
	if(matrix->empty()) {
		return nullptr;
	}

	if (size() == num_nodes) {
		return at(node_id-1);
	}
	return binarySearch(0, size()-1,node_id);
}

Node* GraphWeighted::binarySearch(uInt l, uInt r, uInt node_id){

    if (r >= l) {
        uInt mid = l + (r - l) / 2;
        if (matrix->at(mid)->getId() == node_id){
            return matrix->at(mid);
		}
        if (matrix->at(mid)->getId() > node_id) {
			if (mid == 0) {
				return nullptr;
			}
            return binarySearch(l, mid - 1, node_id);
		}
        return binarySearch(mid + 1, r, node_id);
    }
    return nullptr;
}

void GraphWeighted::sort() 
{
	std::sort(matrix->begin(), matrix->end(), bind(&GraphWeighted::compareNodes, this, placeholders::_1, placeholders::_2));
}


bool GraphWeighted::compareNodes(Node *a, Node *b)
{
    return a->getId() < b->getId();
}
