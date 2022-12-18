#include <iostream>
using namespace std;
float GetFF_QCD(float pt, int cat){
if(cat==4){
		if(pt<35.000) return 0.774135;
		else if(pt<40.000) return 0.850124;
		else if(pt<45.000) return 0.970383;
		else if(pt<50.000) return 1.137509;
		else if(pt<60.000) return 1.409416;
		else if(pt<75.000) return 1.845016;
		else if(pt<90.000) return 2.286533;
		else if(pt<105.000) return 2.619870;
		else if(pt<120.000) return 2.956064;
		else if(pt<140.000) return 3.175008;
		else if(pt<160.000) return 3.528523;
		else if(pt<200.000) return 3.366476;
		else return 3.094847;
		}
else if(cat==5){
		if(pt<35.000) return 0.857803;
		else if(pt<40.000) return 1.103906;
		else if(pt<45.000) return 1.460895;
		else if(pt<50.000) return 2.073817;
		else if(pt<60.000) return 2.939033;
		else if(pt<70.000) return 4.073989;
		else if(pt<80.000) return 5.145240;
		else if(pt<100.000) return 7.091122;
		else if(pt<120.000) return 9.002205;
		else if(pt<140.000) return 10.946866;
		else if(pt<180.000) return 13.610109;
		else return 16.978230;
		}
else {cout << "something wrong" << endl; return 0;}
}

float GetFF_QCD_up(float pt, int cat){
if(cat==4){
		if(pt<35.000) return 0.814867;
		else if(pt<40.000) return 0.902282;
		else if(pt<45.000) return 1.034500;
		else if(pt<50.000) return 1.221074;
		else if(pt<60.000) return 1.526872;
		else if(pt<75.000) return 2.005908;
		else if(pt<90.000) return 2.467215;
		else if(pt<105.000) return 2.842234;
		else if(pt<120.000) return 3.178285;
		else if(pt<140.000) return 3.421419;
		else if(pt<160.000) return 3.775233;
		else if(pt<200.000) return 3.632308;
		else return 3.309208;
	}
else if(cat==5){
		if(pt<35.000) return 0.893832;
		else if(pt<40.000) return 1.166100;
		else if(pt<45.000) return 1.570987;
		else if(pt<50.000) return 2.249678;
		else if(pt<60.000) return 3.183584;
		else if(pt<70.000) return 4.440575;
		else if(pt<80.000) return 5.698879;
		else if(pt<100.000) return 7.786727;
		else if(pt<120.000) return 9.910455;
		else if(pt<140.000) return 12.340617;
		else if(pt<180.000) return 14.914247;
		else return 18.384532;
	}
else {cout << "something wrong" << endl; return 0;}
}

float GetFF_QCD_dn(float pt, int cat){
if(cat==4){
		if(pt<35.000) return 0.733404;
		else if(pt<40.000) return 0.797967;
		else if(pt<45.000) return 0.906265;
		else if(pt<50.000) return 1.053945;
		else if(pt<60.000) return 1.291960;
		else if(pt<75.000) return 1.684125;
		else if(pt<90.000) return 2.105851;
		else if(pt<105.000) return 2.397505;
		else if(pt<120.000) return 2.733843;
		else if(pt<140.000) return 2.928597;
		else if(pt<160.000) return 3.281813;
		else if(pt<200.000) return 3.100645;
		else return 2.880486;
	}
else if(cat==5){
		if(pt<35.000) return 0.821773;
		else if(pt<40.000) return 1.041712;
		else if(pt<45.000) return 1.350803;
		else if(pt<50.000) return 1.897955;
		else if(pt<60.000) return 2.694482;
		else if(pt<70.000) return 3.707403;
		else if(pt<80.000) return 4.591600;
		else if(pt<100.000) return 6.395517;
		else if(pt<120.000) return 8.093955;
		else if(pt<140.000) return 9.553115;
		else if(pt<180.000) return 12.305972;
		else return 15.571927;
	}
else {cout << "something wrong" << endl; return 0;}
}

