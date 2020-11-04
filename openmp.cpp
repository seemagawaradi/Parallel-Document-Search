 #include<bits/stdc++.h>
using namespace std;

// bool comp(vector<float> a,vector<float> b)
// {
//     if(a[a.size()-1]>b[b.size()-1])
//     return true;
//     else return false;
// }
// bool comp1(vector<float> a,vector<float> b)
// {
    
//     if(a[a.size()-1]>b[b.size()-1])
//     //#pragma omp task
//     return true;
//     else 
//     //#pragma omp task
//     return false;
// }
void similarity_function(vector<vector<float>> &input1,vector<float> query)
{
    for(int i=0;i<input1.size();i++)
    {
        float sum=0;
        //cout<<i<<endl;
        for(int j=1;j<input1[0].size();j++)
        {
            sum+=pow(input1[i][j],query[j-1]);
        }
        
        input1[i].push_back(sum);
    }
}
void similarity_function_parallel(vector<vector<float>> &input2,vector<float> query)
{
    #pragma omp parallel for schedule(guided,10)
    for(int i=0;i<input2.size();i++)
    {
        float sum=0;
        for(int j=1;j<input2[0].size();j++)
        {
            sum+=pow(input2[i][j],query[j-1]);
        }
        input2[i].push_back(sum);
    }
}
void sort_serial(vector<vector<float>> &input1)
{
    for(int i=0;i<input1.size()-1;i++)
    {
        for(int j=0;j<input1.size()-i-1;j++)
        {
            if(input1[j][input1[j].size()-1]<input1[j+1][input1[j+1].size()-1])
            {
                vector <float> a=input1[j];
                input1[j]=input1[j+1];
                input1[j+1]=a;
            }
        }
    }
    
}
void sort_parallel(vector<vector<float>> &input2)
{
    
    for(int i=0;i<input2.size()-1;i++)
    {
        int first = i % 2; 
		#pragma omp parallel for default(none),shared(input2,first)
        for(int j=first;j<input2.size()-1;j++)
        {
            if(input2[j][input2[j].size()-1]<input2[j+1][input2[j+1].size()-1])
            {
                vector <float> a=input2[j];
                input2[j]=input2[j+1];
                input2[j+1]=a;
            }
        }
    }
    
}


int main()
{
    int k;
    cout<<" Enter value of k: where k is top k number of related documents."<<endl;
    cin>>k;
    clock_t begin_time,begin1;
    clock_t end_time,end1;
    double sequential_time=0,parallel_time=0;
    
    ifstream fin("input.csv");
    cout<<"Reading input file"<<endl;
    vector<vector<float>> input;
    string line,word;
    while(fin)
    {
        vector<float> row;
        getline(fin,line);
        stringstream s(line);
        while(getline(s,word,','))
        {
            row.push_back(stof(word));
        }        
        input.push_back(row);
        
    }
    ifstream query_f("query.csv");
    cout<<"Reading query file"<<endl;
    vector<float> query;
    getline(query_f,line);
    stringstream s1(line);
        while(getline(s1,word,','))
        {
            query.push_back(stof(word));
        }        
    
    cout<<"Dictionary size is: "<<input[0].size()-1<<endl;
    cout<<"Number of documents is: "<<input.size()<<endl;

    cout<<"Displaying query: "<<endl;
    for(int i=0;i<query.size();i++)
    cout<<query[i]<<" ";
    cout<<endl;

    cout<<query.size()<<endl;
    cout<<"Calculating similarity funtion for each query.."<<endl;
    vector<vector<float>> input1,input2;
    input1=input;
    input2=input;
    begin_time = clock();
    similarity_function(input1,query);
    sort_serial(input1);
    //sort(input.begin(),input.end(),comp);
    cout<<"Displaying top k related document ids: "<<endl;

    for(int i=0;i<k;i++)
    {
        cout<<input1[i][0]<<endl;
    }
    end_time = clock();
    sequential_time  = ((double)(end_time-begin_time))/CLOCKS_PER_SEC;
    cout<<"Sequential Part execution time : "<<sequential_time<<"s"<<endl;

    begin1 = clock();
    //cout<<"parallel similarity\n";
    similarity_function_parallel(input2,query);
    //cout<<"sort parallel\n";
    sort_parallel(input2);
    //cout<<"sorted\n";
    //sort(input.begin(),input.end(),comp);
    cout<<"Displaying top k related document ids: "<<endl;

    for(int i=0;i<k;i++)
    {
        cout<<input2[i][0]<<endl;
    }
    end1 = clock();
    parallel_time  = ((double)(end1-begin1))/CLOCKS_PER_SEC;
    cout<<"parallel Part execution time : "<<parallel_time<<"s"<<endl;
    
}