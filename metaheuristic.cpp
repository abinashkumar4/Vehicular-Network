

#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

#define P_max 2000
#define U_max 640
#define P_s 0.000001
#define K_ 0.7
int initial_soln_count = 10;

vector<int> energyTimestamp;

int Time;
double totalProfit = 0.0; // returns the totalprofit

vector<vector<int>> allocationmat;

vector<double> remain_util;
vector<double> consumed_util;
vector<double> initial_util;
struct job
{
    int id;
    int arrival;
    int deadline;
    double util;
    double profit;
    job(int i, int a, int d, double u, double p)
    {
        id = i;
        arrival = a;
        deadline = d;
        util = u;
        profit = p;
    }
};

vector<job> Task;

void taskPopulate(vector<vector<string>> &task_dump)

{
    int id;
    int arrival;
    int deadline;
    double util;
    double profit;
    int row = task_dump.size();
    for (int i = 1; i < row; i++)
    {

        id = stoi(task_dump[i][0]);
        arrival = stoi(task_dump[i][1]);
        deadline = stoi(task_dump[i][2]);
        util = stod(task_dump[i][3]);
        profit = stod(task_dump[i][4]);
        // profit = (double)(util*1.0f/deadline)*(util*(1.000f)/deadline);
        job t(id, arrival, deadline, util, profit);
        Task.push_back(t);
    }
    //taskscheduled.assign(Task.size(), -1);
}

void printTask()
{
    for (int i = 0; i < Task.size(); i++)
    {

        cout << "id " << Task[i].id << " arrival " << Task[i].arrival << " deadline " << Task[i].deadline << " util " << Task[i].util << " Profit " << Task[i].profit << "\n";
    }
}

double cuberoot(double x)
{
    return cbrt(x);
}

/*
Input : Power Pi
Output : Utilisation U
*/
double calculateU(double P)
{
    if (P == 0.0)
        return 0;

    double num = cuberoot((((P * 1.0) / (P_max * 1.0)) - P_s) / (K_ * 1.0)) * (U_max * 1.0);
    return num;
}

// Function to parse a CSV line and return a vector of strings
vector<string> parseCSVLine(const string &line)
{
    vector<string> tokens;
    // cout<<"called";
    char *token = strtok(const_cast<char *>(line.c_str()), ",");

    while (token != nullptr)
    {
        tokens.push_back(token);
        token = strtok(nullptr, ",");
    }

    return tokens;
}
void csv2vector(FILE *f_task, vector<vector<string>> &task_dump)
{
    char line[1024]; // Adjust the size based on your needs

    // Read and parse each line in the CSV file
    while (fscanf(f_task, "%1023[^\n]%*c", line) == 1)
    {
        // Call the function to parse each line and store it in the 2D vector
        std::vector<std::string> row = parseCSVLine(line);
        task_dump.push_back(row);
    }
}

void populateUtilatTime()
{

    cout << "\n time is " << Time << endl;
    for (int i = 0; i < Time; i++)
    {
        double p = energyTimestamp[i];
        // cout<<p<<" ";
        double getu = calculateU(p);
        initial_util.push_back(getu);
    }
    remain_util = initial_util;
    consumed_util.assign(Time, 0.0); // nothing is consumed intially
}

void show_consumed_util()
{

    cout << "\n &&&&&&&&&&&&&&&&&&&&&& Displaying consumed status at every time stamp &&&&&&&&&&&&&&&&&&&&&&&\n";
    for (auto &a : consumed_util)
    {
        cout << a << " ";
    }
    cout << "\n &&&&&&&&&&&&&&&&&&&&& End of consumed util &&&&&&&&&&&&&&&&&&&\n";
}

// this function will update the consumed util at every time stamp
void update_consumed_util()
{

    for (int i = 0; i < Time; i++)
    {
        consumed_util[i] = initial_util[i] - remain_util[i];
    }
}
void showutil()
{
    cout << " ********** Displaying  Initial Util Data *********** \n";
    for (auto &a : remain_util)
    {
        cout << a << " ";
    }
    cout << "\n *************** Util data end ****************** \n";
}
void storeenergytimestamp(vector<vector<string>> &energy_dump)
{

    int count = 0;
    cout << "\n ************* Showing Initial Power ********************\n";
    for (int i = 1; i < energy_dump.size(); i++)
    {

        energyTimestamp.push_back(stoi(energy_dump[i][1]));
        cout << stoi(energy_dump[i][1]) << " ";
    }
    cout << " \n ********************************* Initial End ********************* \n";

    Time = energyTimestamp.size();

    showutil();
}

void filereader()
{
    FILE *f_pow = fopen("power_reallife_seconddata.csv", "r");
    FILE *f_task = fopen("task_reallife_seconddata.csv", "r");
    // Check if the file was successfully opened
    if (f_task == nullptr)
    {
        cerr << "Error opening file: "
             << "task.csv" << std::endl;
        //  return 1;
    }
    if (f_pow == nullptr)
    {
        cerr << "Error opening file: "
             << "pow.csv" << std::endl;
        // return 1;
    }

    vector<vector<string>> task_dump;
    vector<vector<string>> energy_dump;

    // fseek(f_task, 0, SEEK_SET);
    csv2vector(f_task, task_dump);
    csv2vector(f_pow, energy_dump);
    cout << "\n energy dump size is " << energy_dump.size() << endl;
    storeenergytimestamp(energy_dump);

    // read from vector to struct task
    taskPopulate(task_dump);
    // printTask();
}

bool comp(struct job &a, struct job &b)
{
    return a.deadline > b.deadline;
}

bool ismoreprocessallowed(int util, int timestamp)
{
    int r_util = (int)remain_util[timestamp - 1];
    return (int)util <= r_util;
}

// function to check whether the task is available at current timestamp or not
bool istaskavailable(int arrival, int deadline, int curr_time)
{
    int cur_time = curr_time + 1;
    return ((arrival <= cur_time) && (cur_time <= deadline));
}

/*

Input : U
Output : P

*/
double UtoPOW(int U)
{

    double Pi = P_max * (P_s + 1.0 * K_ * pow((U * 1.0) / U_max, 3));
    return Pi;
}

void show_init_util()
{
    cout << "\n ******************* Initial Util ****************************\n";
    for (auto &a : initial_util)
    {
        cout << a << " ";
    }
    cout << "\n ***************** End of Initial Util ***************************** \n";
}
int generateRandomNumber(int x, int y)
{
    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());

    // Define distribution
    uniform_int_distribution<> distrib(x + 1, y);

    // Generate random number within specified range
    return distrib(gen);
}
void powershift()
{
    priority_queue<pair<double, int>> pq;
    for (int i = 0; i < Time; i++)
    {
        pq.push({energyTimestamp[i], i});
    }
    int count = 250;
    int timestamp, newtimestamp;
    double powertransf;
    int runcount = 0;
    for (int i = 0; i < count; i++)
    {
        timestamp = pq.top().second;
        pq.pop();
        if(timestamp==Time-1)continue;
        newtimestamp = generateRandomNumber(timestamp, Time - 1);
         powertransf = min(energyTimestamp[timestamp], 20);
        energyTimestamp[timestamp] -= powertransf;
        energyTimestamp[newtimestamp] += powertransf;
        runcount++;
        

    }
    cout<<" \n ^^^^^^^^^^ runcount "<<runcount<<" "<<timestamp<<" "<<newtimestamp<<endl;
    populateUtilatTime();
    update_consumed_util();
    // cout << "\n ************** Power now ********************* \n";
    // for (auto &a : energyTimestamp)
    // {
    //     cout << a << " ";
    // }
    // cout << " \n ************* end power ***************** \n";
}

bool isvalidplacement(int timeslot, int taskid)
{
    double utiltobeadded = Task[taskid].util;
    return initial_util[timeslot] >= (consumed_util[timeslot] + utiltobeadded);
}

void initialsolution()
{
    int count =0;
    for (int sol = 0; sol < initial_soln_count; sol++)
    {
        
        vector<int> taskdetails;
        vector<int> taskscheduled(Task.size(),-1); // flag whethere task is scheduled or not
        consumed_util.assign(Time,0);
        remain_util = initial_util;
        // cout<<" \n *********** initial utl ************* \n";
        // for(auto &a : initial_util){
        //     cout<<a<<" ";
        // }
        // cout<<"\n ********* end init ********************\n";


        int count = 0;
        for (auto &a : Task)
        {
            taskdetails.push_back(a.id);
           
        }
        random_device rd;
        mt19937 gen(rd());
        shuffle(taskdetails.begin(), taskdetails.end(), gen);
        int taskid, arrival, deadlin;
         
        for (int i = 0; i < taskdetails.size(); i++)
        {
            taskid = taskdetails[i];
            
            arrival = Task[taskid].arrival;
            deadlin = Task[taskid].deadline;
            // random timestamp
            //  Generate random timestamps within the range [arrival, deadline]
            vector<int> timestamps;
            for (int j = arrival; j <= deadlin; ++j)
            {
                timestamps.push_back(j);
            }

            random_device rd;
            mt19937 gen(rd());
            shuffle(timestamps.begin(), timestamps.end(), gen);
           // cout<<" $$$$$$$$$$$$$$$ sol "<<sol<<" "<<i<<"\n";
            for (auto &a : timestamps)
            {
                bool flag = isvalidplacement(a,taskid);
                //cout<<" flag is "<<flag<< " " <<a<<endl;
                
                if (flag)
                {
                    consumed_util[a] += Task[taskid].util;
                   // cout<<consumed_util[a]<<" consumed "<<endl;
                    remain_util[a] = initial_util[a] - consumed_util[a];
                   // cout<<consumed_util[a]<<" consumed "<<endl;
                    taskscheduled[taskid] = a;
                    
                    
                    break;
                   
                }
            }
           
           
        }

        //allocationmat[sol] = taskscheduled;
        allocationmat.push_back(taskscheduled);
       
    }
    cout<<"\n ********* count is "<<count<<endl;
    cout<<"\n\n task size is "<<Task.size()<<endl;
}
void printRowToFile(const vector<int>& row, const string& filename) {
    ofstream outfile(filename);

    if (!outfile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    for (const auto& element : row) {
        outfile << element << " ";
    }

    outfile.close();
}

void print2DVectorToSeparateFiles(const vector<vector<int>>& allocationmat, const string& prefix) {
    for (size_t i = 0; i < allocationmat.size(); ++i) {
        string filename = prefix + to_string(i) + ".txt";
        printRowToFile(allocationmat[i], filename);
    }
}


pair<int,int> choosetworandomindex(int noofsoln){
    int firstidx = generateRandomNumber(-1,noofsoln-1);
    int secondidx = generateRandomNumber(-1,noofsoln-1);
    while(firstidx ==  secondidx){ //two generate two distinct index because we want crossover of two different solutions
        secondidx = generateRandomNumber(-1,noofsoln-1);
    }

    return {firstidx , secondidx};

}

void crossover(){
    int previoussoln = initial_soln_count;
    int randindexfirst = 

}
void helper()
{

    powershift();
   // allocationmat.assign(initial_soln_count,vector<int>());

    initialsolution();
    print2DVectorToSeparateFiles(allocationmat, "solution_");
    
}
int main()
{
    totalProfit = 0.0;
    filereader();
    helper();
    

    return 0;
}