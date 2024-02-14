// C++ program for implementation of Ford Fulkerson
// algorithm
#include<bits/stdc++.h>
using namespace std;

// Number of vertices in given graph
int V;
int Time;
/* Returns true if there is a path from source 's' to sink
't' in residual graph. Also fills parent[] to store the
path */
//vector<vector<int>>(V,vector<int>(v))
vector<int>energyTimestamp;
bool bfs(vector<vector<int>>  &rGraph, int s, int t, vector<int> &parent)
{
	// Create a visited array and mark all vertices as not
	// visited
	
    vector<bool>visited(V,0);
	

	// Create a queue, enqueue source vertex and mark source
	// vertex as visited
	queue<int> q;
	q.push(s);
	visited[s] = true;
	parent[s] = -1;

	// Standard BFS Loop
	while (!q.empty()) {
		int u = q.front();
		q.pop();

		for (int v = 0; v < V; v++) {
			if (visited[v] == false && rGraph[u][v] > 0) {
				// If we find a connection to the sink node,
				// then there is no point in BFS anymore We
				// just have to set its parent and can return
				// true
				if (v == t) {
					parent[v] = u;
					return true;
				}
				q.push(v);
				parent[v] = u;
				visited[v] = true;
			}
		}
	}

	// We didn't reach sink in BFS starting from source, so
	// return false
	return false;
}

// Returns the maximum flow from s to t in the given graph
int fordFulkerson(vector<vector<int>> &graph, int s, int t)
{
	int u, v;

	// Create a residual graph and fill the residual graph
	// with given capacities in the original graph as
	// residual capacities in residual graph
	 // Residual graph where rGraph[i][j]
				// indicates residual capacity of edge
				// from i to j (if there is an edge. If
				// rGraph[i][j] is 0, then there is not)

    vector<vector<int>>rGraph(V,vector<int>(V));
	for (u = 0; u < V; u++)
		for (v = 0; v < V; v++)
			rGraph[u][v] = graph[u][v];

	vector<int>parent(V); // This array is filled by BFS and to
				// store path

	int max_flow = 0; // There is no flow initially

	// Augment the flow while there is path from source to
	// sink
	while (bfs(rGraph, s, t, parent)) {
		// Find minimum residual capacity of the edges along
		// the path filled by BFS. Or we can say find the
		// maximum flow through the path found.
		int path_flow = INT_MAX;
		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			path_flow = min(path_flow, rGraph[u][v]);
		}

		// update residual capacities of the edges and
		// reverse edges along the path
		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			rGraph[u][v] -= path_flow;
			rGraph[v][u] += path_flow;
		}

		// Add path flow to overall flow
		max_flow += path_flow;
	}

	// Return the overall flow
	return max_flow;
}


int countRows(FILE *file) {
    int numRows = 0;  // Variable to store the number of rows
    int ch;           // Variable to store each character read from the file

    // Iterate through each character in the file
    while ((ch = fgetc(file)) != EOF) {
        // Check for newline character
        if (ch == '\n') {
            numRows++;
        }
    }

    return numRows-1;//first line is header
}

// Function to parse a CSV line and return a vector of strings
vector<string> parseCSVLine(const string &line) {
    vector<string> tokens;
    //cout<<"called";
    char* token = strtok(const_cast<char*>(line.c_str()), ",");

    while (token != nullptr) {
        tokens.push_back(token);
        token = strtok(nullptr, ",");
    }

    return tokens;
}
void csv2vector(FILE *f_task,vector<vector<string>> &task_dump){
    char* line = nullptr;
    size_t len = 0;
    // Read and parse each line in the CSV file
    while (getline(&line, &len, f_task) != -1) {
        // Call the function to parse each line and store it in the 2D vector
        std::vector<std::string> row = parseCSVLine(line);
        task_dump.push_back(row);
    }
    
    // for(int i=0;i<task_dump.size();i++){
    //     for(int j=0;j<task_dump[i].size();j++){
    //         cout<<task_dump[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }

}

void populate_data(vector<vector<int>>&graph,int &num_task,vector<vector<string>> &task_dump){
    
    for(int i = 0; i < V ;i++){

        //case 1
        if(i > 0 && i <= num_task){
            graph[0][i] = 1;
        }

        //case 2

    }
    int ai,di;
    for( int i = 1 ; i <= num_task ; i++ ){
        ai = stoi(task_dump[i][1]);//arrival time
        di = stoi(task_dump[i][2]);//deadline

        for( int j = num_task + ai ; j <= num_task + di ; j++ ){
            graph[i][j] = 1;
        }
    }
    
    
    for( int i = (1 + num_task) ; i <= ( num_task + Time) ; i++)
    { 
        graph[i][i + Time] = energyTimestamp[i-(1 + num_task)];

    }

   
    for( int i = (1 +Time + num_task) ; i <= ( num_task + Time*2) ; i++)
    { 
        graph[i][V-1] = energyTimestamp[i-(1 + num_task+ Time)];

    }




    // cout<<"full matrix \n";
    // for(int i = 0; i < V ;i++){
    //     for(int j=0;j<V;j++){
    //         cout<<graph[i][j]<<" ";
    //     }
    //     cout<<endl;

        

    // }

}

void storeenergytimestamp(vector<vector<string>> &energy_dump){

     cout<<"hello "<<energy_dump.size()<<endl;
    int count = 0;
    for(int i=1;i < energy_dump.size();i++){
        
            energyTimestamp.push_back(stoi(energy_dump[i][1]));


        
    }
    // cout<<"energy time stamp\n";
    // for(auto &a : energyTimestamp){
    //     cout<<a <<" ";
    // }
    cout<<endl;
    Time = energyTimestamp.size();
    
}

// Driver program to test above functions
int main()
{
	// Let us create a graph shown in the above example

    FILE *f_pow = fopen("ES_pow.csv","r");
    FILE *f_task = fopen("task.csv","r");
       // Check if the file was successfully opened
    if (f_task == nullptr) {
        cerr << "Error opening file: " << "task.csv" << std::endl;
        return 1;
    }
    if (f_pow == nullptr) {
        cerr << "Error opening file: " << "pow.csv" << std::endl;
        return 1;
    }
    int num_task = countRows(f_task);
    cout<<"no of row "<<num_task<<endl;
    vector<vector<string>> task_dump;
    vector<vector<string>> energy_dump;
    fseek(f_task, 0, SEEK_SET);
    csv2vector(f_task,task_dump);
    csv2vector(f_pow,energy_dump);
   
    storeenergytimestamp(energy_dump);
    
    int temp = 1 + num_task + 2 * Time + 1;
    V = temp;

	vector<vector<int>>graph(V,vector<int>(V,0));

	
    populate_data(graph , num_task,task_dump);



    cout << "The maximum possible flow is "
		<< fordFulkerson(graph, 0, V-1);
    
    // for(int i=0;i<num_task ; i++){
    //     cout<<graph[0][i]<<" ";
    // }
    fclose(f_pow);
    fclose(f_task);
	return 0;
}
