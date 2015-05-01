#include <iostream>
#include <string>
#include <stdio.h>
#include <bitset>
#include <math.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

double double_map(string phone){
	if (phone=="AA") return 0;
	if (phone=="AE") return 1;
	if (phone=="AH") return 2;
	if (phone=="AO") return 3;
	if (phone=="AW") return 4;
	if (phone=="AY") return 5;
	if (phone=="B") return 6;
	if (phone=="CH") return 7;
	if (phone=="D") return 8;
	if (phone=="DH") return 9;
	if (phone=="EH") return 10;
	if (phone=="ER") return 11;
	if (phone=="EY") return 12;
	if (phone=="F") return 13;
	if (phone=="G") return 14;
	if (phone=="HH") return 15;
	if (phone=="IH") return 16;
	if (phone=="IY") return 17;
	if (phone=="JH") return 18;
	if (phone=="K") return 19;
	if (phone=="L") return 20;
	if (phone=="M") return 21;
	if (phone=="N") return 22;
	if (phone=="NG") return 23;
	if (phone=="OW") return 24;
	if (phone=="OY") return 25;
	if (phone=="P") return 26;
	if (phone=="R") return 27;
	if (phone=="S") return 28;
	if (phone=="SH") return 29;
	if (phone=="T") return 30;
	if (phone=="TH") return 31;
	if (phone=="UH") return 32;
	if (phone=="UW") return 33;
	if (phone=="V") return 34;
	if (phone=="W") return 35;
	if (phone=="Y") return 36;
	if (phone=="Z") return 37;
	if (phone=="ZH") return 38;
	if (phone=="SIL") return 39;
	if (phone==".") return 40;
}

string phone_map(double i){
    if (i == 0) return "AA";
    if (i == 1) return "AE";
    if (i == 2) return "AH";
    if (i == 3) return "AO";
    if (i == 4) return "AW";
    if (i == 5) return "AY";
    if (i == 6) return "B";
    if (i == 7) return "CH";
    if (i == 8) return "D";
    if (i == 9) return "DH";
    if (i == 10) return "EH";
    if (i == 11) return "ER";
    if (i == 12) return "EY";
    if (i == 13) return "F";
    if (i == 14) return "G";
    if (i == 15) return "HH";
    if (i == 16) return "IH";
    if (i == 17) return "IY";
    if (i == 18) return "JH";
    if (i == 19) return "K";
    if (i == 20) return "L";
    if (i == 21) return "M";
    if (i == 22) return "N";
    if (i == 23) return "NG";
    if (i == 24) return "OW";
    if (i == 25) return "OY";
    if (i == 26) return "P";
    if (i == 27) return "R";
    if (i == 28) return "S";
    if (i == 29) return "SH";
    if (i == 30) return "T";
    if (i == 31) return "TH";
    if (i == 32) return "UH";
    if (i == 33) return "UW";
    if (i == 34) return "V";
    if (i == 35) return "W";
    if (i == 36) return "Y";
    if (i == 37) return "Z";
    if (i == 38) return "ZH";
    if (i == 39) return "SIL";
    if (i == 40) return ".";
}


void convertoutput(string i, double output[]){
	for(int j=0; j< 78; j++){
		output[j] = 0;
	}
	for(int j=0; j<i.length(); j++){
		std::bitset<5> bar (i[j]-'A'+1);
		for(int k=0; k<5; k++) output[j*5+k] = bar[k];
	}
}

float sigmoid(double t){
	float k = 1/(1+exp(-1*t));
	return k;
}

	int m = 6;
	int n = 5;
	double tr = 0.2; 		//training rate
	int l = 13;				//no of characters in a word
	
	int hiddensize = m*l;	//size of the hidden layer 			//TRIAL AND ERROR

	double input[78];
	double output[65];
	double prediction[78];
	double hiddenlayer[78];		
	double w1[78][78];
	double w2[65][78];
	
//graphene to phoneme
void training(){
	for(int i=0; i<hiddensize; i++){
		for(int j=0; j<m*l; j++) w1[i][j]=rand()%20 - 10;//2*static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 1;
	}
	for(int i=0; i<n*l; i++){
		for(int j=0; j<hiddensize; j++) w2[i][j]=rand()%20 - 10;//2*static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 1;
	}
	
	ifstream file("training.data", ios::in);

	while(1){
		string grapheme;
		file >> grapheme;
		if (grapheme == "--") break;
		for(int k=0; k<78; k++) input[k]=0;
		for(int j=0; j<grapheme.length(); j++){
			string phoneme;
			file >> phoneme;
			std::bitset<6> bar(double_map(phoneme));
			for(int k=0; k<6; k++) input[j*6+k] = bar[k];
		}		
		convertoutput(grapheme,output);	
		
		double delta1[hiddensize];
		double delta2[n*l];
	
		//layer2 and layer3 are made 0
		for(int i=0; i<hiddensize; i++) hiddenlayer[i]=0;
		for(int i=0; i<n*l; i++) prediction[i]=0;
		
		for(int z=0; z<10; z++){
		
		//FEED FORWARD
		for(int i=0; i<hiddensize; i++){									//layer2 values calculated
			for(int j=0; j<m*l; j++) hiddenlayer[i]+= w1[i][j]*input[j];
			hiddenlayer[i] = sigmoid(hiddenlayer[i]);
		}
		for(int i=0; i<n*l; i++){											//layer3 values calculated
			for(int j=0; j<hiddensize; j++) prediction[i]+= w2[i][j]*hiddenlayer[j];
			prediction[i] = sigmoid(prediction[i]);
		}
		
		//BACK PROPAGATION
		for(int i=0; i<n*l; i++){
			delta2[i] = (output[i]-prediction[i])*prediction[i]*(1-prediction[i]); 				//delta<w> b/w layer 2 and layer 3
		}
		for(int i=0; i<hiddensize; i++){	
			////cout << "here" <<endl;										//delta<w> b/w layer 1 and layer 2
			delta1[i]=0;
			for(int j=0; j<n*l; j++) delta1[i] += hiddenlayer[i]*(1-hiddenlayer[i])*delta2[j]*w2[j][i];
		}
		
		//final updation in costs
		for(int j=0; j<n*l; j++){
			for(int i=0; i<hiddensize; i++) {
				w2[j][i] += tr*delta2[j]*hiddenlayer[i];
			}
		}
		for(int j=0; j<hiddensize; j++){
			for(int i=0; i<m*l; i++)
				w1[j][i] += tr*delta1[j]*input[i];
		}
	}
		//~ for(int j=0; j<hiddensize; j++){
			//~ for(int i=0; i<m*l; i++){
				//~ //cout << w1[j][i] << endl;
			//~ }
		//~ }
	}
}	

int main(){
	training();

	ifstream file("testing.data", ios::in);

	while(1){
		double t;
		file >> t;
		//cout<<"T: "<<t<<endl;

		if (t == 0) break;

		double testinput[78];
		for(int k=0; k<78; k++) testinput[k]=0;
		//cout<<"checkpost 4: "<<endl;
		for(int i=0; i<t; i++){
			//cout<<"1--"<<endl;
			string p;
			//cout<<"2--"<<endl;
			file >> p;
			//cout<<"3--"<<endl;
			
			std::bitset<6> bar(double_map(p));

			for(int k=0; k<6; k++) testinput[i*6+k] = bar[k];

		}

		//cout<<"Check put 1: "<<endl;
		
		//FEED FORWARD
		for(int i=0; i<hiddensize; i++){									//layer2 values calculated
			for(int j=0; j<m*l; j++) hiddenlayer[i]+= w1[i][j]*testinput[j];
			hiddenlayer[i] = sigmoid(hiddenlayer[i]);
		}

		//cout <<"checkpost 2: "<<endl;

		for(int i=0; i<n*l; i++){											//layer3 values calculated
			for(int j=0; j<hiddensize; j++) prediction[i]+= w2[i][j]*hiddenlayer[j];
			prediction[i] = sigmoid(prediction[i]);
			if(prediction[i]>0.5) prediction[i] = 1;
			else prediction[i] = 0;
		}

		//cout << "checkpost 3: "<<endl;

		double testoutput[65];
		string graph;
		file >> graph;
		//cout << "Graph: "<<graph << endl;
		convertoutput(graph,testoutput);
		int match = 0;
		for(int i=0; i<65; i++){
			if(testoutput[i]==prediction[i])match++;
			
		}
		cout << match << endl;
	}
		
}
