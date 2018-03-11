#include <iostream>
#include <chrono>
#include <cmath>
#include <sstream>
#include <cstdio>
#include <string>
#include <fstream>
#include <thread>

using namespace std;
using namespace std::chrono;

class Vec{
private:
	
public:
	double m_x = 0;
	double m_y = 0;
	double m_z = 0;

	Vec(double x=0, double y=0, double z=0){
		m_x = x;
		m_y = y;
		m_z = z;
	}
	
	Vec operator+(const Vec& v){
		return Vec(m_x+v.m_x, m_y+v.m_y, m_z+v.m_z);
	}
	
	Vec operator-(const Vec& v){
		return Vec(m_x-v.m_x, m_y-v.m_y, m_z-v.m_z);
	}
	
	Vec operator*(const double& d){
		return Vec(m_x*d, m_y*d, m_z*d);
	}
	
	Vec operator/(const double& d){
		return Vec(m_x/d, m_y/d, m_z/d);
	}
	
	Vec& operator+=(const Vec& v){
		this->m_x += v.m_x;
		this->m_y += v.m_y;
		this->m_z += v.m_z;
		return *this;
	}
	
	double operator|(const Vec& v){
		return m_x*v.m_x + m_y*v.m_y + m_z*v.m_z;
	}
	
	operator string(){
		stringstream ss;
		ss << "Vec( " << m_x << ", " << m_y << ", " << m_z << " )";
		return ss.str();
	}
	
	double getLength(){
		return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
	}
	
	Vec normalize(){
		return (*this/this->getLength());
	}
};

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

Vec precAccel(0, 0, 0);
//Vec speed(5, 120/3.6, 0);
Vec speed(130/3.6, 0, 0);
Vec pos(0, 0, 0);
double mass = 80; //kg


bool getSurfaceContact(Vec surfacePos, Vec pos, Vec speed, Vec surfaceNormal){
	//cout << "\tSurfContact : " << (surfaceNormal|speed) << endl;
	return ((surfaceNormal|speed) < 0);
}

Vec getNormalReaction(Vec surfaceNormal, Vec force){
	return surfaceNormal*(force|surfaceNormal)*(-1);
}

Vec weight(double mass){
	return Vec(0, -9.81*mass, 0);
}

Vec fluidFriction(double coeff, Vec speed, int degree=1){
	return speed*pow(speed.getLength(), degree-1)*(-coeff);
}

Vec solidFriction(double coeff, Vec speed, Vec normReaction, Vec surface){
	Vec u = (surface*(speed|surface)).normalize();
	if((speed|surface) < 10e-1)
		u = u*0.01;
	return u*coeff*normReaction.getLength()*-1;
}

Vec getAccel(){
	Vec normReac(0,0,0);
	Vec force = weight(mass) + fluidFriction(1, speed, 2) + solidFriction(0.6, speed, Vec(0, 9.81*mass, 0), Vec(1, 0, 0));
	
	precAccel = force/mass;
	return force/mass;
}

time_point<high_resolution_clock> tSpeed = high_resolution_clock::now();
void updateSpeed(){
	time_point<high_resolution_clock> t2 = high_resolution_clock::now();
	duration<double, std::milli> dt = t2-tSpeed;
	tSpeed = t2;
	
	speed += getAccel() * (dt.count()/1000);
}

Vec getSpeed(){
	updateSpeed();
	
	return speed;
}

time_point<high_resolution_clock> tPos = high_resolution_clock::now();
void updatePos(){
	time_point<high_resolution_clock> t2 = high_resolution_clock::now();
	duration<double, std::milli> dt = t2-tPos;
	tPos = t2;
	
	pos += getSpeed() * (dt.count()/1000);
}

Vec getPos(){
	updatePos();
	
	return pos;
}

int main(int argc, char **argv)
{
	ofstream file;
	file.open("simulation.csv", ios::out);
	if(file.bad()){
		cout << "Bad !" << endl;
		return 1;
	}
	file << "t;x;y" << "\n";
	time_point<high_resolution_clock> t0 = high_resolution_clock::now();
	time_point<high_resolution_clock> t = high_resolution_clock::now();
	duration<double, std::milli> dt = t-t0;
	for(int i=0; i<=10*10/* *10 */; i++){
		t = high_resolution_clock::now();
		dt = t-t0;
		Vec p = getPos();
		file << dt.count()/1000 << ";" << p.m_x << ";" << p.m_y << "\n";
		waitFor((1000*100), true); // wait for 100ms
	}
	file.close();
	cout << "Done !" << endl;
//	while(cin.get() != 'q'){
//		cout << "Accel = " << (string)getAccel() << endl;
//		cout << "Speed = " << (string)getSpeed() << " m/s" << endl;
//		cout << "Position : " << (string)getPos() << " in meters" << endl;
//	}
	
	return 0;
}
