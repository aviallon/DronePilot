/** 
 * @file main.c
 * @brief This is a theoretical test to measure distance from the ground using a sonar module.
 * @author Antoine Viallon
 **/

#include <iostream>
#include <chrono>
#include <random>
#include <thread>
using namespace std;

/**
 * @brief Waits for the specified time in microsecs.
 * 
 * If threadSleep is set to true, the OS will put the thread in sleep during the waiting, so that it doesn't consume much ressources. But it is far less precise than default mode. Use it only for durations above 100ms.
 * @param microsecs Number of microseconds of sleeping.
 * @param threadSleep Uses the OS to pause the thread for the specified time. Less precise.
 */
void waitFor(int microsecs, bool threadSleep=false){
	using namespace std::chrono;
	if(threadSleep){
		this_thread::sleep_for(microseconds(microsecs));
	} else {
		auto t1 = high_resolution_clock::now();
		auto t2 = high_resolution_clock::now();
		duration<double, std::micro> dt = t2-t1;
		while(dt.count() < microsecs){
			t2 = high_resolution_clock::now();
			dt = t2-t1;
		}
	}
}

/**
 * @brief Makes a 10µs impulsion with the sonar.
 */
void impulsion(){
	// write(TRIGGER_PIN, HIGH);
	auto start = chrono::high_resolution_clock::now();
	waitFor(10, false);
	auto end = chrono::high_resolution_clock::now();
	// write(TRIGGER_PIN, LOW);
	chrono::duration<double, std::milli> elapsed = end-start;
	cout << "\n\tActual waiting : " << elapsed.count() << " ms\n";
}

/**
 * @brief For testing purpose, randomly returns true or false.
 * @return In therory, returns a bool eqal to true if an echo is detected by the sonar.
 */
bool isThereEcho(){
	random_device gen;
	uniform_int_distribution<int> distrib(0, 6);
	int dice = distrib(gen);
	if(dice == 3)
		return true;
	
	return false;
}

/**
 * @brief Mesures a distance using the sonar.
 * @return Returns the measured distance in meter as a float.
 */
float mesureDistance(){
	const float v = 340;
	
	impulsion();
	auto t = chrono::system_clock::now();
	while(!isThereEcho()){
		//cout << "." << endl;
	}
	chrono::duration<float> delay = chrono::system_clock::now() - t;
	float dt = delay.count()*1000;
	
	//cout << "dt = " << dt << endl;
	
	float d = v * dt / 2;
	
	return d;
}

/**
 * @brief Measures ten distances. Arguments are ignored.
 * @param argc Arguments number
 * @param argv Arguments values
 * @return Returns 0 on success and something else otherwise.
 */
int main(int argc, char **argv)
{
	for(int i=0; i<=10;i++){
		cout << "Distance : " << mesureDistance() << endl;
	}
	cin.get();
	return 0;
}
