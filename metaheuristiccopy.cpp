

#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

#define P_max 2000
#define U_max 640
#define P_s 0.000001
#define K_ 0.7
int initial_soln_count = 200;

vector<int> energyTimestamp;
long long finalprofitmax =0;

int Time;
double totalProfit = 0.0; // returns the totalprofit

vector<vector<int>> allocationmat;//to store initial solutions

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

long long hashed(vector<vector<int>>&data,int idx){
    long long hash = 0;
    for(int i = 0;i<data[idx].size();i++){
        hash+=(i*data[idx][i]);
    }
    return hash;
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
int nextMin_consumed_timeslot(int t){
    if(t == Time-1)return -1;//no next time slot as it is the last slot
    int mn = t;
    int val = energyTimestamp[t];
    for(int i=t+1;i<Time;i++){
        if(energyTimestamp[i]<val){
            val = energyTimestamp[i];
            mn = i;
            // break;
           
        }
    }
    return mn;//return index of next minimum consumed time slot
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
        if(pq.empty())break;
        timestamp = pq.top().second;
        pq.pop();
        if(timestamp==Time-1)continue;
        //cout<<" timestamp "<<timestamp<<" energy "<<energyTimestamp[timestamp]<<"\n";
        //newtimestamp = generateRandomNumber(timestamp, Time - 1);
       newtimestamp = nextMin_consumed_timeslot(timestamp);
       if(energyTimestamp[timestamp]<20)continue;
         powertransf = min(energyTimestamp[timestamp], 20);
        energyTimestamp[timestamp] -= powertransf;
        energyTimestamp[newtimestamp] += powertransf;
        runcount++;
        

    }
   // cout<<" \n ^^^^^^^^^^ runcount "<<runcount<<" "<<timestamp<<" "<<newtimestamp<<endl;
    populateUtilatTime();
    update_consumed_util();
    cout << "\n ************** Power now ********************* \n";
    for (auto &a : energyTimestamp)
    {
        cout << a << " ";
    }
    cout << " \n ************* end power ***************** \n";
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

        auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
        mt19937 rng(seed);
        shuffle(taskdetails.begin(), taskdetails.end(), rng);
        int taskid, arrival, deadlin;
         
        for (int i = 0; i < taskdetails.size(); i++)
        {
            taskid = taskdetails[i];
           // cout<<"itr "<<sol<<" "<<taskid<<endl;
            
            arrival = Task[taskid].arrival;
            deadlin = Task[taskid].deadline;
            // random timestamp
            //  Generate random timestamps within the range [arrival, deadline]
            vector<int> timestamps;
            for (int j = arrival; j <= deadlin; ++j)
            {
                timestamps.push_back(j);
            }
            auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
            mt19937 rng(seed);
            
            shuffle(timestamps.begin(), timestamps.end(), rng);
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

        allocationmat[sol] = taskscheduled;
        //allocationmat.push_back(taskscheduled);
        //cout<<" hash is "<<hashed(allocationmat,sol)<<endl;
       
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
//given a schedule or solution return the total profit gained from that schedule
long double calculateProfit(int idx,vector<vector<int>>&data){
    long double profitt = 0;
    for(int i=0 ;i < Task.size();i++){
        if(data[idx][i]!=-1){
            profitt+=Task[i].profit;
        }

    }
    return profitt;
}


void crossover(vector<vector<int>>&newsols,int &noNewsols){
    int previoussoln = initial_soln_count;//please keep in mind initial solution must always be even
    vector<int>temp(previoussoln);
    for(int i=0;i<previoussoln;i++){
        temp[i]=i;
    } 
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 rng(seed);
    shuffle(temp.begin(), temp.end(), rng);
    int firstidx,secondidx;
    
    int row = 0;
    for(int i = 0;i < previoussoln;i+=2){
        
        consumed_util.assign(Time,0);
        remain_util = initial_util;
        firstidx = temp[i];
        secondidx = temp[i+1];//we choose two solution to make the crossover
        int newtimeslot;
      //  cout<<firstidx<<" "<<secondidx<<endl;
        
        for(int taskid = 0 ;taskid < Task.size();taskid++){
            // Seed the random number generator with current time
            //cout<<"reached up "<<endl;

            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            mt19937 rng(seed);

            uniform_int_distribution<int> dist(0, 1);
            int random_number = dist(rng);
            // cout<<" reached here "<<endl;
             //if 0 try to take firstidx
             //if 1 try to take secondidx
            // random_number= 0;
             bool flag = false;
             if(random_number==0){
              //  cout<<"inside loop \n";
             // cout<<" random 0"<<endl;
                newtimeslot = allocationmat[firstidx][taskid];
              //  cout<<" below loop\n";
                bool flagg = isvalidplacement(newtimeslot,taskid);
              //  cout<<" flag is "<<flagg<<endl;
                if(flagg){
                    consumed_util[newtimeslot]+=Task[taskid].util;
                    remain_util[newtimeslot] = initial_util[newtimeslot] - consumed_util[newtimeslot];
                  //  cout<<" crossoover itr "<<i<<endl;
                    newsols[row][taskid]= newtimeslot;
                    flag = true;
                }

               }else if(random_number==1 || !flag){//take from second solutions
              // cout<<"random 1"<<endl;
                newtimeslot = allocationmat[secondidx][taskid];
                if(isvalidplacement(newtimeslot,taskid)){
                    consumed_util[newtimeslot]+=Task[taskid].util;
                    remain_util[newtimeslot] = initial_util[newtimeslot] - consumed_util[newtimeslot];
                    newsols[row][taskid]= newtimeslot;
                }
               }
            
        }
       // cout<<" hash data is "<<hashed(newsols,row)<<endl;
        row++;
       // cout<<"corros oeveri \n";

    }
   // cout<<" ************ ended crossover module ************ \n";
}
void arrayreset(vector<vector<int>>&data){
    int row = data.size();
    int col = data[0].size();
    data.assign(row,vector<int>(col,-1));
}

void tempstore(vector<vector<int>>&b,vector<vector<int>>&c){
    priority_queue<pair<double,int>>maxheap;
    int row = 0;
   // cout<<" total no of records is "<<allocationmat.size()<<endl;
    for(int i=0;i<allocationmat.size();i++){
        for(int j=0;j<allocationmat[i].size();j++){
            c[row][j]=allocationmat[i][j];
        }
        double prft = calculateProfit(row,c);
        maxheap.push({prft,row});
       // cout<<" in hash of allocation "<<hashed(c,row)<<"\n";
        row++;
        
       // cout<<" ******* reached temp store********* "<<i<< "\n";
    }
   // cout<<row<<endl;
    for(int i = 0;i<b.size();i++){
        for(int j=0;j<b[i].size();j++){
            c[row][j]=b[i][j];
        }
        double prft = calculateProfit(row,c);
        maxheap.push({prft,row});
      //  cout<<" in hash of schedule "<<hashed(c,row)<<" profit "<<prft<<"\n";
        row++;
       // cout<<" *******  temp store********* "<<i<<"\n";
    }
    int count=0;
    int newrow = 0;
    long double maxxprofit = 0;
    while(count != initial_soln_count){
        int idx =maxheap.top().second;
        if(count==0)maxxprofit = maxheap.top().first;
        maxheap.pop();
        for(int taskid = 0;taskid < Task.size();taskid++){
            allocationmat[newrow][taskid] = c[idx][taskid];
        }
       // cout<<" in hash of final top allocation "<<hashed(allocationmat,newrow)<<"\n";
        newrow++;
        count++;
       // cout<<" in hash of schedule "<<hashed(c,idx)<<"\n";
    }
    if(maxxprofit>finalprofitmax)finalprofitmax = maxxprofit;
    //finalprofitmax = max(maxxprofit,finalprofitmax);
    cout<<"maxprofit is "<<maxxprofit<<"\n";

}
void helper()
{

    powershift();
    allocationmat.assign(initial_soln_count,vector<int>(Task.size(),-1));

    initialsolution();
    cout<<"\n *********** ended inital solutions module ********** \n";
   // print2DVectorToSeparateFiles(allocationmat, "solution_");
    int noNewsols = initial_soln_count/2;
    vector<vector<int>>newsols(noNewsols,vector<int>(Task.size(),-1));
    vector<vector<int>>tempdata(initial_soln_count+noNewsols,vector<int>(Task.size(),-1));
    for(int itr  =0;itr<40;itr++){
        arrayreset(newsols);
        //arrayreset(tempstore);
       // cout<<" ****** itr "<<itr<<"\n";
        crossover(newsols,noNewsols);
        tempstore(newsols,tempdata);
        cout<<endl;
        
    }
    cout<<"final profit is "<<finalprofitmax<<endl;

    
}
int main()
{
    totalProfit = 0.0;
    filereader();
    helper();
    

    return 0;
}