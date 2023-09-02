#include "GraphWeighted.hpp"

// PUBLIC METHODS

GraphWeighted::GraphWeighted(const string path) : GraphADT(path)
{
	TIMERSTART(build_matrix);
	format = ".txt";
	buildTxt();
	TIMERSTOP(build_matrix);
	//print();
}


GraphWeighted::GraphWeighted() 
{
	format = ".txt";
}

GraphWeighted::~GraphWeighted()
{
	for (auto i : matrix)
		if(i != nullptr) delete i;
}

void GraphWeighted::buildBin()
{
	
}


void GraphWeighted::buildTxt()
{
	cout << "format: " << "id ady weight" << endl;
	ifstream file; 
	file.open(path);
	assert(file.is_open());
	string line;
	Node* temp = nullptr;
	int x = 0;
	while(getline(file,line)){
		if(line.front() == '%') continue; 
		auto content = splitString(line, " ");
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
		temp->addAdjacent(adj, weight); 
		//cout << id << " " << adj << " " << weight << endl;
	}
	insert(temp);
	temp->shrinkToFit();
	temp->sort();
	matrix.shrink_to_fit();
	sort();
	file.close();
	//print();
}


string GraphWeighted::getPath(){
	return path;
}

uint64_t GraphWeighted::maxValueEdge()
{
    return maxEdge;
}

uint64_t GraphWeighted::size()
{
	return matrix.size();
}

Node* GraphWeighted::back(){
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

void GraphWeighted::insert(Node* node)
{
	matrix.push_back(node);
	if (node->getBackAdjacent() > maxEdge) {
		maxEdge = node->getBackAdjacent();
	}
}

void GraphWeighted::print()
{
	for (size_t i = 0; i < matrix.size(); i++) {
		matrix[i]->print();
	}
}


void GraphWeighted::printAsMatrix()
{
	for (size_t i = 0; i < matrix.size(); i++) {
		if(i < matrix[i]->getId() - 1) {
			int temp = i; 
			while(temp < matrix[i]->getId()) {
				cout << "Node " << temp << ": " << 0 << endl;
				temp++; 
			}
		}
		matrix[i]->printBinary();
	}
}

void GraphWeighted::writeAdjacencyList(string path_write_)
{
	ofstream file;
	string pathFile = path_write_ + ".txt";
	cout << "Save: " << pathFile << endl;
	file.open(pathFile, std::ofstream::out | std::ofstream::trunc); 
	assert(file.is_open());
	for(auto i : matrix){
		for(auto j = i->wAdjacentsBegin(); j != i->wAdjacentsEnd(); j++){
			file << i->getId() << " " << j->first << " " << j->second << endl;
		}
	}
	file.close();
}

void GraphWeighted::restoreNodes()
{
	for (size_t i = 0; i < matrix.size(); i++) {
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

Node* GraphWeighted::at(uInt pos){
	return matrix.at(pos);
}

Node* GraphWeighted::find(uInt node_id){
	if(matrix.empty()) {
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
        if (matrix[mid]->getId() == node_id){
            return matrix[mid];
		}
        if (matrix[mid]->getId() > node_id) {
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
	std::sort(matrix.begin(), matrix.end(), bind(&GraphWeighted::compareNodes, this, placeholders::_1, placeholders::_2));
}


bool GraphWeighted::compareNodes(Node *a, Node *b)
{
    return a->getId() < b->getId();
}
