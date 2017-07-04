#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class Clock
{
private:
  int mHours; //(range 0-23)
  int mMinutes; //(range 0-59)
  int mSeconds; //(range 0-59)
public:
  Clock();
  Clock(int);
  Clock(int, int);
  Clock(int, int, int);
  int getHours()
	  {return mHours;}
  int getMinutes()
	  {return mMinutes;}
  int getSeconds()
	  {return mSeconds;}
  void setHours(int hourValue) {
	  if ( hourValue >= 0 && hourValue <23) {
		  mHours = hourValue;
	  } else if(hourValue<0) {
		  mHours = 0;
	  } else if(hourValue>12) {
		  mHours = 12;
	  }
  }
  void setMinutes(int minuteValue) {
	  if ( minuteValue >= 0 && minuteValue < 60) {
		  mMinutes = minuteValue;
	  } else {
		  mMinutes = 0;
	  }
  }
  void setSeconds(int secondValue) {
	  if ( secondValue >= 0 && secondValue < 60) {
		  mSeconds = secondValue;
	  } else {
		  mSeconds = 0;
	  }
  }
  bool isMorning();
  bool isAfternoon();
  bool isEvening();
  void tick();
  friend ostream &operator<<(ostream &, Clock&);
};
  ostream &operator<<(ostream &out, Clock &time){
	  string display;
	  if(time.mHours >12)
	  {	time.mHours = time.mHours - 12;
		  display = "PM";
	  }else{
		  display = "AM";
	  }


	  out << setfill('0') << setw(2)<<time.mHours;
	  out << ":"<<setfill('0')<<setw(2)<< time.mMinutes;
	  out << ":"<<setfill('0')<<setw(2)<< time.mSeconds
	  << " "<<display << endl;
	  return out;
}
  Clock::Clock() {
	  setHours(12);
	  setMinutes(0);
	  setSeconds(0);
	  }
  Clock::Clock(int hourValue)
	  {setHours(hourValue);}
  Clock::Clock(int hourValue, int minuteValue)
	  {setHours(hourValue);
	  setMinutes(minuteValue);}
  Clock::Clock(int hourValue, int minuteValue, int secondValue)
	  {setHours(hourValue);
	  setMinutes(minuteValue);
	  setSeconds(secondValue);}


  bool Clock::isMorning() {
	  if ( mHours < 12) {
		  return true;
	  }else{
		  return false;
	  }
  }
  bool Clock::isAfternoon() {
	  if ( mHours >= 12 && mHours < 18) {
		  return true;
	  }else{
		  return false;
	  }
  }
  bool Clock::isEvening() {
	  if ( mHours < 18) {
		  return false;
	  }else{
		  return true;
	  }
  }
  void Clock::tick() {
	  setSeconds((getSeconds() + 1 ) % 60);

	  if ( getSeconds()==0)
	  {
		  setMinutes((getMinutes() + 1) % 60);
		  if (getMinutes() == 0)
		  {
			  setHours((getHours() % 12) +1);
		  }
	  }
	  }

	  /*if (mSeconds < 60)
		  mSeconds = mSeconds + 1;
	  else
		  mSeconds = 0;
	  if (mMinutes < 60 && mSeconds == 0)
		  mMinutes = mMinutes + 1;
	  else
		  mMinutes = 0;
	  if (mHours < 24 && mMinutes == 0 && mSeconds == 0)
		  mHours = mHours + 1;
	  else
		  mHours = 0;*/


void printTimeMessage(Clock a) {

	  cout << "The current time is "<< a << endl;
}
