#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

typedef struct VOTE * VOTEPTR;

char vote_file[] = "Ballot.txt";
char cand_list[] = "Candidates.txt";
int N = 0;//number of candidates

struct VOTE{
	int candidate[50];
	VOTEPTR NEXT;
};

VOTEPTR vote_list = NULL; // list of votes;

//PROTOTYPES
void build_pool();
int cand_count();
void tally_votes(int totals[]);
void recalculate(int totals[]);
void print_results(int totals[]);

int cand_count(){ //counts the number of candidates from input file	
	int count = 0;
	int numb;
	string name;
	ifstream infile;
	infile.open(cand_list);
	
	if (infile.fail()){
		cout << " OPEN FILE ERROR (cand_count)" << endl;
		exit(1);
	}
	
	while(!infile.fail() && !infile.eof()){
		infile >> numb;
		getline(infile, name, '\r');
		//cout << numb << "	"<< name << endl;
		count++;
	}
	
	infile.close();
	
	//cout << count << endl;
	
	return count;
}


void build_pool(){ // builds a linked-list of the votes casted
	
	VOTEPTR temp_vote = vote_list;
	int x;
	int prior;
	int cand_numb;
	ifstream infile;
	infile.open(vote_file);
	
	if (infile.fail()){
		cout << " OPEN FILE ERROR (cand_count)" << endl;
		exit(1);
	}

	if( vote_list == NULL){
		vote_list = new VOTE;
		for(x=0; x < N; x++){
			infile >> cand_numb;
			infile >> prior;
				
			vote_list -> candidate[x] = prior;
			//cout << vote_list -> candidate[x] << endl;				
			//cout << vote_list -> candidate[x];
		}
		vote_list->NEXT = NULL;
	}
	temp_vote = vote_list;
	while(!infile.fail() && !infile.eof()){	
		//temp_vote = vote_list;
		temp_vote -> NEXT = new VOTE;
		temp_vote = temp_vote -> NEXT;
			for(x=0; x < N; x++){
				infile >> cand_numb;
				infile >> prior;
				temp_vote -> candidate[x] = prior; 
				//cout << temp_vote -> candidate[x]<<endl;
			}
			temp_vote ->NEXT = NULL;
	}
}


void tally_votes(int totals[]){ //traverses the vote_list, searching for candidates with a priority of 1; it then increments the total for that candidate
	VOTEPTR temp_vote = vote_list;
	int x;
	
	while(temp_vote != NULL){
		for(x=0;x<N;x++){
			//cout << temp_vote -> candidate[x] << endl;
			if(temp_vote -> candidate[x] == 1){
				totals[x]++;
				//print_results(totals);
				//cout << endl;
				}
			}
	
		temp_vote = temp_vote -> NEXT;
	}
}

void print_results(int totals[]){//name says it all
	int x;
	int num;
	string name;
	
	ifstream infile;
	
	infile.open(cand_list);
	
	
	for(x = 0; x < N; x++){
		infile >> num;
		getline(infile, name, '\r');
		cout << name << "	"<< totals[x] << endl;
	}
	
	infile.close();
}

void recalculate(int totals[]){// searches for the person with the least amount of votes, then decrements all the priorities in that vote
	int least_liked = 50000;
	int x;
	
	for(x=0;x<N;x++){
		if(totals[x] != 0){
			if(totals[x] < totals[least_liked])
				least_liked = x;
		}
	}
	//cout << least_liked << endl;
	
	VOTEPTR temp_vote = vote_list;
	
	while(temp_vote != NULL){
		if(temp_vote -> candidate[least_liked] == 1){
			for(x=0;x<N;x++){
				if(temp_vote -> candidate[x] != 0){
					temp_vote -> candidate[x]--;
				}
			}
		}
		temp_vote->candidate[least_liked] = 0;
		temp_vote = temp_vote -> NEXT;
	}
	for(x=0;x<N;x++){
		totals[x] = 0;
	}
}

int main(void){

	
	int x;
	N = cand_count();
	int totals[N];// holds the total votes
	int remaining_cand = N;
	build_pool();
	for(x=0;x<N; x++){
		totals[x] = 0;
	}

	tally_votes(totals);
	
	cout<<"First Round Results"<< endl;
	print_results(totals);
	cout << endl;
	x = 2;
	while(remaining_cand >= 2){
		//cout << remaining_cand << endl;
		recalculate(totals);
		cout << "Round " << x << " Results" << endl;
		tally_votes(totals);
		print_results(totals);
		remaining_cand--;
		x++;
		cout << endl;
	}
	return 0;
}